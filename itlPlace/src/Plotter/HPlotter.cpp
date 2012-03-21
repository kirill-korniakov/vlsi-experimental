#include "HPlotter.h"
#include "PlotterData.h"
#include "Utils.h"

HPlotter::~HPlotter()
{
  Destroy();
  DestroyHistogramWindow();
  delete m_Data;
}

void HPlotter::Initialize()
{
  if (_IsEnabled())
  {
    m_isDestroyed = false;

    m_Data->drawingsNum = 0;
    m_Data->autoRefreshStep
      = m_Data->autoRefreshStepDefault
      = m_hd.cfg.ValueOf("plotter.autoRefreshStep", 200);
    m_Data->borderSpace = m_hd.cfg.ValueOf("plotter.borderSpace", 5);

    //create window
    m_Data->windowName = m_hd.cfg.ValueOf("plotter.windowName", "itlPlace");
    cvNamedWindow(m_Data->windowName.c_str());

    //create image
    CvSize imgSize;
    m_Data->textSpaceHeight = m_hd.cfg.ValueOf("plotter.textSpaceHeight", 200);
    imgSize.height = m_hd.cfg.ValueOf("plotter.windowHeight", 700);
    int userHeight = imgSize.height - m_Data->textSpaceHeight;
    imgSize.width = (int)(userHeight * m_hd.Circuit.Width() / m_hd.Circuit.Height());
    IMG = cvCreateImage(imgSize, IPL_DEPTH_8U, 3);
    IMG->origin = 1;

    //mark text field
    CvPoint startTextLine, finishTextLine;
    startTextLine.x = 0;
    startTextLine.y = m_Data->textSpaceHeight;
    finishTextLine.x = imgSize.width;
    finishTextLine.y = m_Data->textSpaceHeight;
    cvLine(IMG, startTextLine, finishTextLine, GetCvColor(Color_Black), 1);

    //calculate coordinate mapping
    m_Data->x_ratio = (imgSize.width - m_Data->borderSpace * 2) / m_hd.Circuit.Width();
    m_Data->y_ratio = (userHeight - m_Data->borderSpace * 2) / m_hd.Circuit.Height();

    m_Data->x_offset = -m_hd.Circuit.MinX();
    m_Data->y_offset = -m_hd.Circuit.MinY() + m_Data->textSpaceHeight / m_Data->y_ratio ;

    //start video writing
    if (m_hd.cfg.ValueOf("plotter.createVideo", true) == true)
      StartVideoWriting("", ".\\video\\");
  }
}

void HPlotter::Destroy()
{
  if (!m_isDestroyed)
  {
    cvReleaseImage(&IMG);
    cvDestroyWindow(m_Data->windowName.c_str());

    StopVideoWriting();

    m_isDestroyed = true;
  }
}

bool HPlotter::IsEnabled()
{
  return !m_isDestroyed && _IsEnabled();
}

void HPlotter::Clear()
{
  if (IsEnabled())
  {
    cvSet(IMG, cvScalar(255.0, 255.0, 255.0));
  }
}

void HPlotter::Refresh(WaitTime waitTime)
{
  if (IsEnabled())
  {
    cvShowImage(m_Data->windowName.c_str(), IMG);

    cvWaitKey(waitTime);

    if ((m_hd.cfg.ValueOf("plotter.createVideo", true) == true))
      WriteCurrentFrame();

    if ((m_hd.cfg.ValueOf("plotter.saveImages", true) == true))
      SaveImage("", m_hd.cfg.ValueOf("plotter.pixDirectory"));
  }
}

void HPlotter::Refresh(const string& cfgPath)
{
  if (IsEnabled())
    Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(cfgPath, 1));
}

void HPlotter::SetAutoRefreshMinFrequency(int freq)
{
  if (freq > m_Data->autoRefreshStepDefault)
    m_Data->autoRefreshStep = freq;
  m_Data->drawingsNum = 0;
}

void HPlotter::ResetAutoRefreshFrequency()
{
  m_Data->autoRefreshStep = m_Data->autoRefreshStepDefault;
  Refresh();
}

bool HPlotter::_IsEnabled()
{
  return m_hd.cfg.ValueOf("plotter.enabled", false);
}

/*void HPlotter::_AutoRefresh()
{
  if (!IsEnabled())
    return;

  if (m_Data->drawingsNum++ % m_Data->autoRefreshStep == 0)
  {
    Refresh(NO_WAIT);
  }
}*/

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