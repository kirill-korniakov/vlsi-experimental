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
    Row() : Cells(0), Size(0) {}

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
    Column(): TextAlign(Align_Default), Width(0) {}
  };

  typedef std::list<Row> RowsList;

  RowsList m_Rows;
  std::vector<Column> m_Columns;
  Row* m_LastRow;

  string ConvertToString(int val)
  {
    char buf[12];
    _itoa(val, buf, 10);
    return buf;
  }

  string ConvertToString(string val)
  {
    return val;
  }

  string ConvertToString(const char* val)
  {
    return val;
  }

  string ConvertToString(char val)
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
        for (int col = 0; col < NumOfColumns(); col += row->Cells[col].Colspan)
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
        for (int col = 0; col < NumOfColumns(); col += row->Cells[col].Colspan)
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

  void PrintRow(const Row& row)
  {
    for (int col = 0; col < row.Size; col += row.Cells[col].Colspan)
    {
      WRITE(col == 0 ? "%s" : " %s", GetAlignedText(row, col).c_str());
    }
    WRITELINE("");
  }

public:

  TableFormatter(int columnsCount): m_LastRow(0)
  {
    m_Columns.resize(columnsCount);
  }

  int NumOfColumns() const {return (int)m_Columns.size();}

  void NewRow()
  {
    m_Rows.push_back(Row());
    m_LastRow = &m_Rows.back();
    m_LastRow->AllocateCells(NumOfColumns());
    for (int i = 0; i < m_LastRow->Size; ++i)
      m_LastRow->Cells[i].TextAlign = m_Columns[i].TextAlign;
  }

  void SetColumnMinWidth(int colIdx, int width)
  {
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    m_Columns[colIdx].Width = max(m_Columns[colIdx].Width, width);
  }

  void SetColumnAlign(int colIdx, Align align)
  {
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    m_Columns[colIdx].TextAlign = align;
  }

  template <class T>
  void SetCell(int colIdx, T value, int colspan = 1, Align textAlign = Align_Default)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    m_LastRow->Cells[colIdx].Value = ConvertToString(value);
    m_LastRow->Cells[colIdx].TextAlign = textAlign;
    m_LastRow->Cells[colIdx].Colspan = colspan;
  }

  template <class T>
  void SetCell(int colIdx, T value, Align textAlign, int colspan = 1)
  {
    if (colIdx == -1) return;
    ASSERT(colIdx >= 0 && colIdx < NumOfColumns());
    m_LastRow->Cells[colIdx].Value = ConvertToString(value);
    m_LastRow->Cells[colIdx].TextAlign = textAlign;
    m_LastRow->Cells[colIdx].Colspan = colspan;
  }

  void Print()
  {
    UpdateLayout();
    for (RowsList::iterator row = m_Rows.begin(); row != m_Rows.end(); ++row)
      PrintRow(*row);
  }

};

#endif //__TABALEFORMATTER_H__