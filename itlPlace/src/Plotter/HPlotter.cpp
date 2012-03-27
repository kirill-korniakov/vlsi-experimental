#include "HPlotter.h"
#include "PlotterData.h"
#include "Utils.h"

void HPlotter::Refresh(const string& cfgPath)
{
  if (IsEnabled())
    Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(cfgPath, 1));
}

bool HPlotter::_IsEnabled()
{
  return m_hd.cfg.ValueOf("plotter.enabled", false);
}

Color HPlotter::_GetCellColor(HCell plotCell)
{
  HCellWrapper cell = m_hd[plotCell];

  static TemplateTypes<HMacroType>::vector bufferType;
  static bool onlyOnce = true;
  if (onlyOnce)
  {
    //get types of buffers
    const char* bufferName = m_hd.cfg.ValueOf("GlobalPlacement.bufferName", (const char*)"INVX1");
    bufferType.push_back(Utils::FindMacroTypeByName(m_hd, bufferName));

    string sBufferList = m_hd.cfg.ValueOf("New_Buffering.BufferList", "");
    unsigned int n = m_hd.cfg.ValueOf("New_Buffering.BufferListLength", 0);
    string* bufferList = NULL;
    if (n > 0)
    {
      bufferList = new string [n];     
      for (unsigned int i = 0, j = 0, t = 0; (i < sBufferList.length()) && (j < n); i++, t++)
      {
        if(sBufferList[i] != ',')
          bufferList[j].push_back(sBufferList[i]);
        else
        {
          t = -1;
          j++;
        }
      }

      string macro;

      for (HMacroTypes::EnumeratorW macroTypeEW = m_hd.MacroTypes.GetEnumeratorW(); macroTypeEW.MoveNext();)
      {
        if (macroTypeEW.Type() == MacroType_BUF)
        {
          macro = macroTypeEW.Name();
          bool isbuf = false;
          for (unsigned int i = 0; (i < n) && !isbuf; i++)
            if (macro == bufferList[i])
              isbuf = true;
          if (!isbuf)
            continue;

          bufferType.push_back(Utils::FindMacroTypeByName(m_hd, macro));
        }
      }
    }

    onlyOnce = false;
  }

  static Color combCellBackColor = Color_LightBlue;
  static Color specCellBackColor = Color_LightGreen;
  static Color seqCellBackColor  = Color_Aqua;
  static Color bufferColor       = Color_Yellow;

  HMacroType cellType = m_hd.Cells.Get<HCell::MacroType, HMacroType>(cell);
  if (find(bufferType.begin(), bufferType.end(), cellType) < bufferType.end())
    return bufferColor;
  else
    return cell.IsSpecial() ? specCellBackColor : cell.IsSequential() ? seqCellBackColor :  combCellBackColor;
}