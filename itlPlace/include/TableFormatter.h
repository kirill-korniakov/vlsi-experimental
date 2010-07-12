#ifndef __TABALEFORMATTER_H__
#define __TABALEFORMATTER_H__

#include "stdTypes.h"
#include "Grow.h"
#include <stdlib.h>
#include "Logging.h"

class TableFormatter
{
public:
  enum Align
  {
    Align_Default,
    Align_Left,
    Align_Right,
    Align_Fill
  };
private:
  enum RowType
  {
    RowType_Header,
    RowType_Border,
    RowType_Default
  };
  struct Cell
  {
    string Value;
    int Colspan;
    Align TextAlign;
    Cell(): Colspan(1), TextAlign(Align_Default) {}
  };

  struct Row
  {
    Cell* Cells;
    int Size;
    RowType RType;
    bool PrintedToConsole;
    bool PrintedToHTML;
    Row() : Cells(0), Size(0), RType(RowType_Default), PrintedToConsole(false), PrintedToHTML(false) {}

    void AllocateCells(int count)
    {
      ::Grow(&Cells, Size, count);
      Size = count;
    }

    ~Row()
    {
      AllocateCells(0);
    }
  };

  struct Column
  {
    Align TextAlign;
    int Width;
    int Precision;
    Column(): TextAlign(Align_Default), Width(0), Precision(6) {}
  };

  typedef std::list<Row> RowsList;

  RowsList m_Rows;
  std::vector<Column> m_Columns;
  Row* m_LastRow;

  string ConvertToString(int val, int colIdx)
  {
    char buf[12];
    _itoa(val, buf, 10);
    return buf;
  }

  string ConvertToString(double val, int colIdx)
  {
    char buf[64];
    setlocale(LC_ALL, "");
    sprintf(buf, "%.*f", m_Columns[colIdx].Precision, val);
    setlocale(LC_ALL, "C");
    return buf;
  }

  string ConvertToString(string val, int colIdx)
  {
    return val;
  }

  string ConvertToString(const char* val, int colIdx)
  {
    return val;
  }

  string ConvertToString(char val, int colIdx)
  {
    return string(&val, 1);
  }

  int SumWidth(int startColIdx, int count)
  {
    int sum = 0;
    for (int i = 0; i < count; ++i)
      sum += m_Columns[i + startColIdx].Width + 1;
    return max(0, sum - 1);
  }

  void IncreaseWidth(int startColIdx, int count, int increment)
  {
    for (int i = 0; i < count; ++i)
      m_Columns[i + startColIdx].Width += increment;
  }

  void IncreaseShortestWidth(int startColIdx, int count)
  {
    int shortestInd = startColIdx;
    int shortestLen = m_Columns[startColIdx].Width;
    for (int i = 1; i < count; ++i)
      if (shortestLen > m_Columns[i + startColIdx].Width)
      {
        shortestInd = i + startColIdx;
        shortestLen = m_Columns[i + startColIdx].Width;
      }

    m_Columns[shortestLen].Width++;
  }

  void UpdateLayout()
  {
    {//first pass
      for (RowsList::iterator row = m_Rows.begin(); row != m_Rows.end(); ++row)
        for (int col = 0; col < row->Size; col += row->Cells[col].Colspan)
        {
          if (row->Cells[col].Colspan + col > NumOfColumns())
            row->Cells[col].Colspan = NumOfColumns() - col;
          if (row->Cells[col].Colspan <= 1)
            SetColumnMinWidth(col, (int)row->Cells[col].Value.length());
        }
    }
    {//second pass
      for (RowsList::iterator row = m_Rows.begin(); row != m_Rows.end(); ++row)
      {
        for (int col = 0; col < row->Size; col += row->Cells[col].Colspan)
        {
          int cols = row->Cells[col].Colspan;
          if (cols < 2) continue;

          int size = (int)row->Cells[col].Value.length();
          int space = SumWidth(col, cols);
          if (space < size)
          {//balance
            int each = (size - space) / cols;
            if (each > 0)
              IncreaseWidth(col, cols, each);
            for (int remainder = size - (each * cols + space); remainder > 0; --remainder)
              IncreaseShortestWidth(col, cols);
          }//if (space < size)
        }
      }
    }//second pass
  }

  Align GetCellAlign(const Row& row, int colIdx)
  {
    if (row.Cells[colIdx].TextAlign == Align_Default)
      if (m_Columns[colIdx].TextAlign == Align_Default)
        return Align_Right;
      else
        return m_Columns[colIdx].TextAlign;
    else
      return row.Cells[colIdx].TextAlign;
  }

  string GetAlignedText(const Row& row, int colIdx)
  {
    int width = SumWidth(colIdx, row.Cells[colIdx].Colspan);
    int len = (int)row.Cells[colIdx].Value.length();
    switch (GetCellAlign(row, colIdx))
    {
    case Align_Left:
      return row.Cells[colIdx].Value + string(width - len, ' ');
    case Align_Right:
      return string(width - len, ' ') + row.Cells[colIdx].Value;
    case Align_Fill:
      if (len == 0)
        return string(width, ' ');
      if (len == 1)
        return string(width, row.Cells[colIdx].Value[0]);
      else
      {
        string result;
        result.reserve(width);
        while ((int)result.length() < width)
          result.append(row.Cells[colIdx].Value, 0,  min(len, width - (int)result.length()));
        return result;
      }
    default:
      return string(width, ' ');
    };
  }

  void PrintRow(Row& row, Logger* logger, bool writeToConsole, bool writeToHTML)
  {
    writeToHTML &= logger->HasHTMLStream() && row.RType != RowType_Border;
    if (writeToHTML) logger->WriteToHTMLStream(false, "<tr>");
    for (int col = 0; col < row.Size; col += row.Cells[col].Colspan)
    {
      if (writeToHTML)
      {
        const char* tag = row.RType == RowType_Header ? "th" : "td";
        if (row.Cells[col].Colspan > 1)
        {
          switch(GetCellAlign(row, col))
          {
          case Align_Right:
            logger->WriteToHTMLStream(false, "<%s colspan=\"%d\" align=\"right\">", tag, row.Cells[col].Colspan);
            break;
          case Align_Fill:
            logger->WriteToHTMLStream(false, "<%s colspan=\"%d\" align=\"center\">", tag, row.Cells[col].Colspan);
            break;
          case Align_Left:
          default:
            logger->WriteToHTMLStream(false, "<%s colspan=\"%d\">", tag, row.Cells[col].Colspan);
            break;
          }
        }
        else
        {
          switch(GetCellAlign(row, col))
          {
          case Align_Right:
            logger->WriteToHTMLStream(false, "<%s align=\"right\">", tag);
            break;
          case Align_Fill:
            logger->WriteToHTMLStream(false, "<%s align=\"center\">", tag);
            break;
          case Align_Left:
          default:
            logger->WriteToHTMLStream(false, "<%s>", tag);
            break;
          }
        }
      }

      if (writeToConsole)
        logger->WriteIgnoringHTML(col == 0 ? "%s" : " %s", GetAlignedText(row, col).c_str());

      if (writeToHTML)
      {
        if (row.Cells[col].Value.empty())
          logger->WriteToHTMLStream(false, "&nbsp;");
        else
          logger->WriteToHTMLStream(true, "%s", GetAlignedText(row, col).c_str());
        logger->WriteToHTMLStream(false, row.RType == RowType_Header ? "</th>" : "</td>");
      }
    }
    if (writeToHTML)
    {
      logger->WriteToHTMLStream(false, "</tr>\n");
      row.PrintedToHTML = true;
    }
    if (writeToConsole)
    {
      logger->WriteLine();
      row.PrintedToConsole = true;
    }
  }

public:

  //public fields
  string Caption;

  //constructors
  TableFormatter(int columnsCount = 0): m_LastRow(0)
  {
    m_Columns.resize(columnsCount);
  }

  TableFormatter(const string& caption, int columnsCount = 0): m_LastRow(0), Caption(caption)
  {
    m_Columns.resize(columnsCount);
  }

  //methods
  int NumOfColumns() const {return (int)m_Columns.size();}

  void NewRow()
  {
    m_Rows.push_back(Row());
    m_LastRow = &m_Rows.back();
    m_LastRow->AllocateCells(NumOfColumns());
    for (int i = 0; i < m_LastRow->Size; ++i)
      m_LastRow->Cells[i].TextAlign = m_Columns[i].TextAlign;
  }

  void NewHeaderRow()
  {
    NewRow();
    m_LastRow->RType = RowType_Header;
  }

  void NewBorderRow()
  {
    NewRow();
    m_LastRow->RType = RowType_Border;
  }

  void SetColumnMinWidth(int colIdx, int width)
  {
    ASSERT(colIdx >= 0);
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      if (m_LastRow != 0)
        m_LastRow->AllocateCells(colIdx + 1);
    }
    m_Columns[colIdx].Width = max(m_Columns[colIdx].Width, width);
  }

  void SetColumnAlign(int colIdx, Align align)
  {
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      if (m_LastRow != 0)
        m_LastRow->AllocateCells(colIdx + 1);
    }
    m_Columns[colIdx].TextAlign = align;
  }

  void SetColumnPrecision(int colIdx, int precision)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      if (m_LastRow != 0)
        m_LastRow->AllocateCells(colIdx + 1);
    }
    m_Columns[colIdx].Precision = precision;
  }

  template <class T>
  void SetCell(int colIdx, T value, int colspan = 1, Align textAlign = Align_Default)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0);
    ASSERT(m_LastRow != 0);
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      m_LastRow->AllocateCells(colIdx + 1);
    }
    m_LastRow->Cells[colIdx].Value = ConvertToString(value, colIdx);
    m_LastRow->Cells[colIdx].TextAlign = textAlign;
    m_LastRow->Cells[colIdx].Colspan = colspan;
  }

  template <class T>
  void SetCell(int colIdx, T value, Align textAlign, int colspan = 1)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0);
    ASSERT(m_LastRow != 0);
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      m_LastRow->AllocateCells(colIdx + 1);
    }
    m_LastRow->Cells[colIdx].Value = ConvertToString(value, colIdx);
    m_LastRow->Cells[colIdx].TextAlign = textAlign;
    m_LastRow->Cells[colIdx].Colspan = colspan;
  }

  template <class T>
  void SetCell(int colIdx, T value, const string& prefix, const string& suffix, int colspan = 1, Align textAlign = Align_Default)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0);
    ASSERT(m_LastRow != 0);
    if (colIdx >= NumOfColumns())
    {
      m_Columns.resize(colIdx + 1);
      m_LastRow->AllocateCells(colIdx + 1);
    }
    m_LastRow->Cells[colIdx].Value = prefix + ConvertToString(value, colIdx) + suffix;
    m_LastRow->Cells[colIdx].TextAlign = textAlign;
    m_LastRow->Cells[colIdx].Colspan = colspan;
  }

  void Print(Logger* logger = 0, bool writeToConsole = true, bool writeToHTML = true)
  {
    UpdateLayout();

    if (logger == 0) logger = &Logger::Global;
    
    if (writeToHTML && logger->HasHTMLStream())
      logger->WriteToHTMLStream(false, "<div class=\"tableFormatter\"><table class=\"tbl\">\n");

    if (!Caption.empty())
    {
      if (writeToHTML && logger->HasHTMLStream())
      {
        logger->WriteToHTMLStream(false, "<caption>");
        logger->WriteToHTMLStream(true, "%s", Caption.c_str());
        logger->WriteToHTMLStream(false, "</caption>");
      }
      if (writeToConsole)
      {
        logger->WriteIgnoringHTML("%s", Caption.c_str());
        logger->WriteLine();
      }
    }

    bool headClosed = true;
    if (writeToHTML && logger->HasHTMLStream())
      if (m_Rows.front().RType == RowType_Header)
      {
        logger->WriteToHTMLStream(false, "<thead>\n");
        headClosed = false;
      }
      else
        logger->WriteToHTMLStream(false, "<tbody>\n");

    for (RowsList::iterator row = m_Rows.begin(); row != m_Rows.end(); ++row)
    {
      if (!headClosed && writeToHTML && logger->HasHTMLStream() && row->RType != RowType_Header)
      {
        logger->WriteToHTMLStream(false, "</thead>\n<tfoot></tfoot>\n<tbody>\n");
        headClosed = true;
      }
      PrintRow(*row, logger, writeToConsole && !row->PrintedToConsole, writeToHTML && !row->PrintedToHTML);
    }

    if (writeToHTML && logger->HasHTMLStream())
      logger->WriteToHTMLStream(false, "</tbody>\n</table>\n</div>");
  }

  void FlushLastRow(bool toConsole = true, bool toHTML = false, Logger* logger = 0)
  {
    UpdateLayout();
    if (logger == 0) logger = &Logger::Global;
    PrintRow(*m_LastRow, logger, toConsole, toHTML);
  }

};

#endif //__TABALEFORMATTER_H__