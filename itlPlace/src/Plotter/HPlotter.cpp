#include <time.h>
#include <io.h>
#include "Base64.h"

#include "HPlotter.h"
#include "PlotterData.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "AdaptiveRoute.h"
#include "VanGinnekenData.h"
#include "HDPGrid.h"
#include "TileGrid.h"
#include "OptimizationContext.h"


static void SavePlotterImage(libconfig::ConfigExt& cfg, const std::string& fileName, const CvArr* image, bool compress, int paletteSize)
{
  const char* compressor = cfg.ValueOf("plotter.PNGCompressor", "pngquant.exe");
  if (compress && (0 == fileName.compare(fileName.length() - 4, 4, ".png")) && Aux::FileExists(compressor))
  {
    cvSaveImage("temp.png", image);
    char cmdline[128];
    sprintf(cmdline, "%s  -force -quiet %d temp.png", compressor, paletteSize);
    system(cmdline);
    if (0 != rename("temp-fs8.png", fileName.c_str()))
    {
      if (0 != rename("temp.png", fileName.c_str()))
      {
        remove("temp.png");
        cvSaveImage(fileName.c_str(), image);
      }
      remove("temp-fs8.png");
    }
    else
      remove("temp.png");
  }
  else
    cvSaveImage(fileName.c_str(), image);
}

HPlotter::HPlotter(HDesign& design):
m_hd(design)
{
  m_data = new PlotterData();
  IMG = 0;
  m_Data->vw = NULL;
  m_isDestroyed = true;
  m_isHistogramDestroyed = true;
}

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


void HPlotter::SaveImage(string fileName, string dirName)
{
  if (!IsEnabled())
    return;

  string ImageFileName;
  if (fileName == "")
  {
    ImageFileName = Aux::CreateCoolFileName(dirName, m_hd.Circuit.Name(), "png");
  }
  else
  {
    ImageFileName = Aux::CreateCoolFileName(dirName, fileName, "png");
  }
  //cvSaveImage(ImageFileName.c_str(), m_Data->img);
  SavePlotterImage(m_hd.cfg, ImageFileName, m_Data->img,
    m_hd.cfg.ValueOf("plotter.CompressImages", false),
    m_hd.cfg.ValueOf("plotter.ImagesPaletteSize", 256));
}

void HPlotter::SaveMilestoneImage(string fileSuffix, bool addToHtmlLog)
{
  if (!IsEnabled())
    return;

  if (m_hd.cfg.ValueOf("plotter.saveMilestoneImages", false))
  {
    string fileName = Aux::CreateCoolFileName(m_hd.cfg.ValueOf("plotter.milestonePixDirectory", "."), m_hd.Circuit.Name() + "_" + fileSuffix, "png");
    //cvSaveImage(fileName.c_str(), m_Data->img);
    SavePlotterImage(m_hd.cfg, fileName, m_Data->img,
      m_hd.cfg.ValueOf("plotter.CompressMilestoneImages", true),
      m_hd.cfg.ValueOf("plotter.MilestonePaletteSize", 16));

    if (Logger::Global.HasHTMLStream() && addToHtmlLog)
    {
      bool embeed = m_hd.cfg.ValueOf("plotter.embeedMilestones", false);
      Logger::Global.WriteToHTMLStream(false, "%s", "<div class=\"milestone\"><img alt=\"");
      Logger::Global.WriteToHTMLStream(true, "%s", fileName.c_str());
      if (embeed && Aux::FileExists(fileName))
      {
        int fsize = Aux::GetFileSize(fileName);
        unsigned char* buffer = new unsigned char[fsize];
        try
        {
          FILE* fimg = fopen(fileName.c_str(), "rb");
          if (fimg != 0)
          {
            fsize = fread(buffer, 1, fsize, fimg);
            fclose(fimg);
          }
          Logger::Global.WriteToHTMLStream(false, "\" src=\"data:image/jpeg;base64,%s\" /></div>\n", Base64::Encode(buffer, fsize).c_str());
        }
        catch(...)
        {
          delete[] buffer;
          throw;
        }
        delete[] buffer;
      }
      else
      {
        for (size_t i = 0; i < fileName.length(); ++i)
          if (fileName[i] == '\\')
            fileName[i] = '/';
        Logger::Global.WriteToHTMLStream(false, "\" src=\"../%s\" /></div>\n", fileName.c_str());
      }
    }
  }
}

void HPlotter::StartVideoWriting(string fileName, string dirName)
{
  CvSize movieFrameSize = cvSize(m_Data->img->width, m_Data->img->height);
  string movieFileName;
  if (fileName == "")
  { 
    movieFileName = Aux::CreateCoolFileName(dirName,  m_hd.Circuit.Name() ,".avi");
  }
  else
    movieFileName = Aux::CreateCoolFileName(dirName, fileName, ".avi");

  char codecName[] = "xvid";
  double fps = m_hd.cfg.ValueOf("plotter.fps", 1.0);
  m_Data->vw = cvCreateVideoWriter(movieFileName.c_str(), 
    (CV_FOURCC(codecName[0], codecName[1], codecName[2], codecName[3])),
    fps, movieFrameSize);

  //TODO: check if writer created
}

void HPlotter::WriteCurrentFrame()
{
  //TODO: check if writer created
  cvWriteFrame(m_Data->vw, IMG);
}


void HPlotter::StopVideoWriting()
{
  if (m_Data->vw)
  {
    cvReleaseVideoWriter(&m_Data->vw);
  }
}

//Show methods
void HPlotter::ShowPlacement(WaitTime waitTime)
{
  Clear();
  PlotPlacement();
  Refresh(waitTime);
}

void HPlotter::AutoShowPlacement(WaitTime waitTime)
{
  if (!IsEnabled())
    return;

  static int i = 0;
  if (i++ % m_Data->autoRefreshStep == 0)
  {
    ShowPlacement(waitTime);
  }
}

void HPlotter::ShowGlobalPlacement(bool plotWires, int nBinRows, int nBinCols, WaitTime waitTime)
{
  Clear();
  PlotBinGrid(nBinRows, nBinCols);
  if (plotWires)
    PlotFullWires();
  PlotPlacement();
  Refresh(waitTime);
}

void HPlotter::VisualizeState(int nClusters, int nBinRows, int nBinCols, int nNets,
                              double* x, double* gLSE, double* gSOD, double* gLR, double* gQS, double* g)
{
  double scaling = m_hd.cfg.ValueOf("GlobalPlacement.gradientScaling", 1.0);
  int waitTime = m_hd.cfg.ValueOf("GlobalPlacement.plotWait", 1);

  Clear();
  PlotBinGrid(nBinRows, nBinCols);
  PlotPlacement();
  PlotGradients(nClusters, (double*)x, gLSE, scaling, Color_Red);
  PlotGradients(nClusters, (double*)x, gSOD, scaling, Color_Brown);
  PlotGradients(nClusters, (double*)x, gLR, scaling, Color_LimeGreen);
  PlotGradients(nClusters, (double*)x, gQS, scaling, Color_Orange);
  PlotGradients(nClusters, (double*)x, g, scaling, Color_Black);
  Refresh((HPlotter::WaitTime)waitTime);
}

void HPlotter::ShowLegalizationState(WaitTime waitTime, bool drawSites)
{
  Clear();
  if (drawSites)
    PlotSites();
  PlotPlacement();
  Refresh(waitTime);
}

void HPlotter::ShowSteinerForest(Color color, WaitTime waitTime)
{
  if (!IsEnabled()) return;

  for (HNets::ActiveNetsEnumeratorW anew = m_hd.Nets.GetActiveNetsEnumeratorW(); anew.MoveNext();)
  {
    AdaptiveRoute(m_hd, anew);
    PlotNetSteinerTree(anew, color);
  }

  Refresh(waitTime);
  SaveMilestoneImage("SteinerForest");
}

void HPlotter::ShowNetSteinerTree(HNet net, Color color, bool isShowPlacement, WaitTime waitTime, double textSize)
{
  if (!IsEnabled()) 
    return;

  if (isShowPlacement)
    ShowPlacement();

  PlotText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
  PlotNetSteinerTree(net, color);  
  Refresh(waitTime);
}

void HPlotter::ShowVGTree(HNet net, VanGinnekenTreeNode* tree, Color LineColor, bool isShowPlacement, WaitTime waitTime, Color VGNodeColor, double textSize)
{  
  if (!IsEnabled()) 
    return;

  if (isShowPlacement)
    ShowPlacement();

  PlotText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
  PlotVGTree(tree, LineColor, VGNodeColor); 
  Refresh(waitTime);
}

void HPlotter::ShowNet(HNet net, bool isShowPlacement, WaitTime waitTime, Color color, double textSize)
{
  if (!IsEnabled()) 
    return;

  if (isShowPlacement)
    ShowPlacement();

  PlotText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
  PlotNet(net, color);
  Refresh(waitTime);
}

void HPlotter::ShowFillBinGrid(AppCtx* context, bool isShowPlacement, WaitTime waitTime)
{
  if (!IsEnabled()) 
    return;

  if (isShowPlacement)
    ShowPlacement();

  PlotFillBinGrid(context);
  Refresh(waitTime);
}

//Plot methods
void HPlotter::PlotPlacement()
{
  if (!IsEnabled())
    return;

  CvPoint center;
  CvPoint start, finish;

  for (HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    PlotCell(cell, _GetCellColor(cell));
  }

  CvScalar terminalColor = cvScalar(0.0, 0.0, 255.0);
  CvScalar terminalBackColor = cvScalar(64.0, 0.0, 0.0);

  for (HCells::TerminalsEnumeratorW terminal = m_hd.Cells.GetTerminalsEnumeratorW(); terminal.MoveNext(); )
  {
    //draw border of cells
    start.x = DesignX2ImageX(terminal.X());
    start.y = DesignY2ImageY(terminal.Y());
    finish.x = DesignX2ImageX(terminal.X() + terminal.Width());
    finish.y = DesignY2ImageY(terminal.Y() + terminal.Height());
    if (start.x + 1 < finish.x && start.y + 1 < finish.y)
    {
      cvRectangle(IMG, start, finish, terminalColor);
      //fill
      start.x++; start.y++;
      finish.x--; finish.y--;
      if (start.x < finish.x && start.y < finish.y)
        cvRectangle(IMG, start, finish, terminalBackColor, CV_FILLED);
    }
    else
      cvCircle(IMG, start, 2, terminalBackColor, CV_FILLED);
  }

  //draw primary pins
  CvScalar PIColor = cvScalar(0.0, 255.0, 0.0); //green
  CvScalar POColor = cvScalar(0.0, 0.0, 255.0); //red
  for (HPins::PrimariesEnumeratorW pin = m_hd.Pins.GetEnumeratorW(); pin.MoveNext(); )
  {
    if (!::IsNull(pin.Net()))
    {
      center.x = DesignX2ImageX(pin.X());
      center.y = DesignY2ImageY(pin.Y());
      if (pin.IsPrimaryInput())
        cvCircle(IMG, center, 3, PIColor, CV_FILLED);
      else
        cvCircle(IMG, center, 3, POColor, CV_FILLED);
    }
  }

  //draw placement region
  CvScalar plBorderColor = cvScalar(0.0, 0.0, 255.0);

  start.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
  start.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
  finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
  finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());
  cvDrawLine(IMG, start, finish, plBorderColor);

  start = finish;
  finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());
  finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());
  cvDrawLine(IMG, start, finish, plBorderColor);

  start = finish;
  finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());
  finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
  cvDrawLine(IMG, start, finish, plBorderColor);

  start = finish;
  finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
  finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
  cvDrawLine(IMG, start, finish, plBorderColor);

  //draw placement center
  CvScalar centerColor = cvScalar(0.0, 0.0, 0.0);
  center.x = DesignX2ImageX((m_hd.Circuit.PlacementMinX() + m_hd.Circuit.PlacementMaxX()) / 2.0);
  center.y = DesignY2ImageY((m_hd.Circuit.PlacementMinY() + m_hd.Circuit.PlacementMaxY()) / 2.0);
  cvCircle(IMG, center, 4, centerColor, 1);

  CvPoint startTextLine, finishTextLine;
  startTextLine.x = 0;
  startTextLine.y = m_Data->textSpaceHeight;
  finishTextLine.x = IMG->width;
  finishTextLine.y = m_Data->textSpaceHeight;
  cvLine(IMG, startTextLine, finishTextLine, GetCvColor(Color_Black), 1);
}

void HPlotter::PlotPath(HDesign& design, HCriticalPath path, int pathNumber)
{
    ASSERT(design.CanDoTiming());
    char fileName[32];
    char signature[256];
    sprintf(fileName, "critical_path_%d", pathNumber);
    sprintf(signature, "critical path #%d %f", pathNumber, design.GetDouble<HCriticalPath::Criticality>(path));

    design.Plotter.Clear();
    design.Plotter.PlotPlacement();
    design.Plotter.PlotCriticalPath(path);
    design.Plotter.PlotText(signature);
    design.Plotter.Refresh("CriticalPaths.plotWait");
    design.Plotter.SaveImage(fileName,
        design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_critical_path\\");
}

void HPlotter::PlotBinGrid(int nBinRows, int nBinCols)
{
  if (IsEnabled())
  {
    CvPoint start, finish;

    Color color = Color_GrayText;
    start.y  = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
    finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());

    //vertical lines
    for (int i = 1; i < nBinCols; i++)
    {
      start.x = finish.x
        = DesignX2ImageX(m_hd.Circuit.PlacementMinX() + i * m_hd.Circuit.PlacementWidth() / nBinCols);
      cvDrawLine(IMG, start, finish, GetCvColor(color));
    }

    start.x  = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
    finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());

    //horizontal lines
    for (int i = 1; i < nBinRows; i++)
    {
      start.y = finish.y
        = DesignY2ImageY(m_hd.Circuit.PlacementMinY() + i * m_hd.Circuit.PlacementHeight() / nBinRows);
      cvDrawLine(IMG, start, finish, GetCvColor(color));
    }
  }
}
void HPlotter::PlotFillBinGrid(AppCtx* context)
{

  /*
  CvPoint start, finish;
  start.x = DesignX2ImageX(x1);
  start.y = DesignY2ImageY(y1);
  finish.x = DesignX2ImageX(x2);
  finish.y = DesignY2ImageY(y2);
  cvRectangle(IMG, start, finish, GetCvColor(col), 1);
  */

  if (IsEnabled())
    return;

  CvPoint start, finish;
  CvScalar color;

  for (int i = 0; i < context->sprData.binGrid.nBinRows; i++)
    for (int j = 0; j < context->sprData.binGrid.nBinCols; j++)
    {
      start.x = DesignX2ImageX(context->sprData.binGrid.bins[i][j].xCoord);
      start.y = DesignY2ImageY(context->sprData.binGrid.bins[i][j].yCoord);
      finish.x = DesignX2ImageX(context->sprData.binGrid.bins[i][j].xCoord + context->sprData.binGrid.binWidth);
      finish.y = DesignY2ImageY(context->sprData.binGrid.bins[i][j].yCoord + context->sprData.binGrid.binHeight);
      /*color.val[0] = context->sprData.binGrid.bins[i][j].sumPotential;
      color.val[1] = 0;
      color.val[2] = 0;
      color.val[3] = 0;*/
      color = cvColorToScalar(context->sprData.binGrid.bins[i][j].sumPotential, 1);
      //cvRectangle(IMG, start, finish, color, 10);
      char str[255];
      sprintf(str, "%.1f", context->sprData.binGrid.bins[i][j].sumPotential);
      PlotTextInPoint(string(str), context->sprData.binGrid.bins[i][j].xCoord, context->sprData.binGrid.bins[i][j].yCoord, 0.5);

    }
}

void HPlotter::PlotGradients(int nClusters, double* coordinates, double* gradients, 
                             double scaling, Color color)
{
  if (IsEnabled())
  {
    CvPoint start, finish;

    for (int i = 0; i < nClusters; i++)
    {
      start.x = DesignX2ImageX(coordinates[2*i+0]);
      start.y = DesignY2ImageY(coordinates[2*i+1]);
      finish.x = DesignX2ImageX(coordinates[2*i+0] + scaling*gradients[2*i+0]);
      finish.y = DesignY2ImageY(coordinates[2*i+1] + scaling*gradients[2*i+1]);
      cvDrawLine(IMG, start, finish, GetCvColor(color));
      cvDrawCircle(IMG, finish, 1, GetCvColor(color));
    }
  }
}

void HPlotter::PlotCell(HCell cell, Color col)
{
  if (!IsEnabled())
    return;

  CvPoint start, finish;
  CvScalar cellBorderColor = cvScalar(255.0, 0.0, 0.0);

  //draw border of cells
  start.x = DesignX2ImageX(m_hd.GetDouble<HCell::X>(cell));
  start.y = DesignY2ImageY(m_hd.GetDouble<HCell::Y>(cell));
  finish.x = DesignX2ImageX(m_hd.GetDouble<HCell::X>(cell) + m_hd.GetDouble<HCell::Width>(cell));
  finish.y = DesignY2ImageY(m_hd.GetDouble<HCell::Y>(cell) + m_hd.GetDouble<HCell::Height>(cell));

  cvRectangle(IMG, start, finish, cellBorderColor);

  //fill the cell
  start.x++; start.y++;
  finish.x--; finish.y--;
  CvScalar fillColor = GetCvColor(col);
  if (start.x <= finish.x && start.y <= finish.y)
  {
    cvRectangle(IMG, start, finish, fillColor, CV_FILLED);
  }
}

void HPlotter::PlotSites()
{
  if (IsEnabled())
  {
    CvScalar siteColor = cvScalar(0.0, 0.0, 255.0);
    CvPoint start, finish;
    for (HPlacementRows::EnumeratorW row = m_hd.PlacementRows.GetEnumeratorW(); row.MoveNext(); )
    {
      for (int i = 0; i < row.VertSitesCount(); ++i)
        for (int j = 0; j < row.HorizSitesCount(); ++j)
        {
          start.x = DesignX2ImageX(row.X() + row.SiteWidth() * j);
          start.y = DesignY2ImageY(row.Y() + row.SiteHeight() * i);
          finish.x = DesignX2ImageX(row.X() + row.SiteWidth() * (j + 1));
          finish.y = DesignY2ImageY(row.Y() + row.SiteHeight() * (i + 1));

          cvRectangle(IMG, start, finish, siteColor, 1);
        }
    }
  }
}

void HPlotter::PlotCriticalPath(HCriticalPath aPath)
{
  if (IsEnabled())
  {
    HCriticalPathWrapper criticalPathW = m_hd[aPath];

    HCriticalPath::PointsEnumeratorW i = criticalPathW.GetEnumeratorW();
    i.MoveNext();

    CvPoint start, finish;
    start.x = 10;
    start.y = 10;
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 1, 1, 0.0, 1, 1 ); 

    HPinWrapper pin1 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())];
    start.x = DesignX2ImageX(pin1.X());
    start.y = DesignY2ImageY(pin1.Y());
    cvCircle(IMG, start, 3, GetCvColor(Color_Peru), 3);
    int index = 0;
    for(; i.MoveNext(); index++)
    {
      HPinWrapper pin2 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())]; 

      start.x = DesignX2ImageX(pin1.X());
      start.y = DesignY2ImageY(pin1.Y());
      finish.x = DesignX2ImageX(pin2.X());
      finish.y = DesignY2ImageY(pin2.Y());

      if (i.SignalDirection() == SignalDirection_Fall)
        cvLine(IMG, start, finish, GetCvColor(Color_Black), m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
      else
        cvLine(IMG, start, finish, GetCvColor(Color_Red), m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
      if (index <=1 )
        cvCircle(IMG, finish, 2, GetCvColor(Color_Plum), 2);
      else
        cvCircle(IMG, finish, 2, GetCvColor(Color_Yellow), 2);
      pin1 = pin2;
    }
  }
}

void HPlotter::PlotNetSteinerTree(HNet net, Color color)
{
  if (!IsEnabled()) return;

  AdaptiveRoute(m_hd, net);

  HNetWrapper netw = m_hd[net];
  HPinWrapper src =  m_hd[netw.Source()];
  HSteinerPointWrapper srcPoint = m_hd[m_hd.SteinerPoints[src]];
  HSteinerPointWrapper nextPoint = srcPoint;

  m_hd.Plotter.PlotCircle(srcPoint.X(), srcPoint.Y(), 4, color);

  TemplateTypes<HSteinerPoint>::stack points;
  points.push(srcPoint);
  while (!points.empty())
  {
    srcPoint = points.top();
    points.pop();

    if (srcPoint.HasLeft())
    {
      nextPoint = srcPoint.Left();
      m_hd.Plotter.PlotLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
      points.push(nextPoint);

      if (srcPoint.HasRight())
      {
        nextPoint = srcPoint.Right();
        m_hd.Plotter.PlotLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
        points.push(nextPoint);
      }
    }
    else
    {
      m_hd.Plotter.PlotCircle(srcPoint.X(), srcPoint.Y(), 1, color);
    }
  }
}

void HPlotter::PlotVGTree(VanGinnekenTreeNode* tree, Color LineColor, Color VGNodeColor)
{
  if (!IsEnabled()) return;

  VanGinnekenTreeNode* srcPoint = tree;
  VanGinnekenTreeNode* nextPoint = srcPoint;

  m_hd.Plotter.PlotCircle(srcPoint->GetX(), srcPoint->GetY(), 4, LineColor);

  TemplateTypes<VanGinnekenTreeNode*>::stack points;
  points.push(srcPoint);
  while (!points.empty())
  {
    srcPoint = points.top();
    points.pop();

    if (srcPoint->HasLeft())
    {
      nextPoint = srcPoint->GetLeft();
      m_hd.Plotter.PlotLine(srcPoint->GetX(), srcPoint->GetY(), nextPoint->GetX(), nextPoint->GetY(), LineColor);
      m_hd.Plotter.PlotCircle(srcPoint->GetX(), srcPoint->GetY(), 1, VGNodeColor);
      //m_hd.Plotter.DrawRectangle(nextPoint->GetX(), nextPoint->GetY(), nextPoint->GetX() + DesignX2ImageX(5), nextPoint->GetY() + DesignY2ImageY(5),  Color(color + 1));
      points.push(nextPoint);

      if (srcPoint->HasRight())
      {
        nextPoint = srcPoint->GetRight();
        m_hd.Plotter.PlotLine(srcPoint->GetX(), srcPoint->GetY(), nextPoint->GetX(), nextPoint->GetY(), LineColor);
        m_hd.Plotter.PlotCircle(srcPoint->GetX(), srcPoint->GetY(), 1, VGNodeColor);
        //m_hd.Plotter.DrawRectangle(nextPoint->GetX(), nextPoint->GetY(), nextPoint->GetX() + DesignX2ImageX(5), nextPoint->GetY() + DesignY2ImageY(5),  Color(color + 1));
        points.push(nextPoint);
      }
    }
    else
    {
      m_hd.Plotter.PlotCircle(srcPoint->GetX(), srcPoint->GetY(), 1, LineColor);
    }
  }
}

void HPlotter::PlotMostCriticalPaths(HDesign& design, int n)
{
    Utils::IterateMostCriticalPaths(design, n, (Utils::CriticalPathHandler)HPlotter::PlotPath);
}

void HPlotter::PlotText(string text, double textSize)
{
  if (!IsEnabled())
    return;

  if (textSize == -1)
    textSize = m_hd.cfg.ValueOf("plotter.textSize", 1.0);

  CvFont font;  
  cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, textSize, textSize , 0.0, 1, 1);

  int start = 0;
  int t = 1;
  char* str = new char[text.length() * 4];
  string bufText = text + text;
  for (unsigned int i = 0; i < text.length(); i++)
  {
    if ((text[i] == '\n') || (i == text.length() - 1))
    {
      for (unsigned int j = start; j <= i; j++)
        bufText[j - start] = text[j];
      bufText[i - start + 1] = '\0';

      int j2 = 0;
      for (unsigned int j = 0; j < bufText.length(); j++)
      {
        if (bufText[j] == '\t')
        {
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
        }
        else
          str[j2] = bufText[j];
        j2++;
      }

      CvPoint point;
      point.x = DesignX2ImageX(10);
      point.y = int(m_Data->textSpaceHeight - 30 * t * textSize);
      cvPutText(IMG, str, point, &font, GetCvColor(Color_Black));
      start = i+1;
      t++;
    }
  }

  delete [] str;
}

void HPlotter::PlotTextInPoint(string text, double x, double y, double textSize )
{

  if (!IsEnabled())
    return;

  if (textSize == -1)
    textSize = m_hd.cfg.ValueOf("plotter.textSize", 1.0);

  CvFont font;  
  cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, textSize, textSize , 0.0, 1, 1);

  int start = 0;
  int t = 1;
  char* str = new char[text.length() * 4];
  string bufText = text + text;
  for (unsigned int i = 0; i < text.length(); i++)
  {
    if ((text[i] == '\n') || (i == text.length() - 1))
    {
      for (unsigned int j = start; j <= i; j++)
        bufText[j - start] = text[j];
      bufText[i - start + 1] = '\0';

      int j2 = 0;
      for (unsigned int j = 0; j < bufText.length(); j++)
      {
        if (bufText[j] == '\t')
        {
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
          j2++;
          str[j2] = ' ';
        }
        else
          str[j2] = bufText[j];
        j2++;
      }

      CvPoint point;
      point.x = DesignX2ImageX(x);
      point.y = DesignY2ImageY(y) - int(30 * t * textSize);
      cvPutText(IMG, str, point, &font, GetCvColor(Color_Black));
      start = i+1;
      t++;
    }
  }

  delete [] str;

}

void HPlotter::PlotNet(HNet net, Color color)
{
  CvPoint start, finish;
  HPinWrapper pin1 = m_hd[m_hd[net].Source()];
  start.x = DesignX2ImageX(pin1.X());
  start.y = DesignY2ImageY(pin1.Y());
  cvCircle(IMG, start, 1, GetCvColor(color), 3);
  for (HNet::SinksEnumeratorW sew = m_hd[net].GetSinksEnumeratorW(); sew.MoveNext(); )
  {
    start.x = DesignX2ImageX(pin1.X());
    start.y = DesignY2ImageY(pin1.Y());
    finish.x = DesignX2ImageX(sew.X());
    finish.y = DesignY2ImageY(sew.Y());
    cvCircle(IMG, finish, 1, GetCvColor(Color_Yellow), 2);
    cvLine(IMG, start, finish, GetCvColor(Color_Red), 1, 1);
  }
}

void HPlotter::PlotFullWires()
{
  if (IsEnabled())
  {
    for ( HNets::ActiveNetsEnumeratorW net = m_hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    {
      PlotNet(net);
    }
  }
}

void HPlotter::Plot2PinNets()
{
  if (IsEnabled())
  {
    for ( HNets::ActiveNetsEnumeratorW net = m_hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    {
      if (net.PinsCount() == 2)
      {
        PlotNet(net);
      }
    }
  }
}

void HPlotter::PlotCircle(double x, double y, int radius, Color col)
{
  if (IsEnabled())
  {
    CvPoint center;
    center.x = DesignX2ImageX(x);
    center.y = DesignY2ImageY(y);
    cvCircle(IMG, center, radius, GetCvColor(col), 2);
  }
}

void HPlotter::PlotLine(double x1, double y1, double x2, double y2, Color col)
{
  if (IsEnabled())
  {
    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    cvLine(IMG, start, finish, GetCvColor(col), 1);
  }
}

void HPlotter::PlotKi(int nClusters, int nNets, double* x, Color color)
{
  double maxKi = m_hd.cfg.ValueOf("GlobalPlacement.bufferCountUpperBound", 0.0);
  if (!IsEnabled() || maxKi <= 0.0) 
    return;

  //FIXME: find proper place
  InitializeHistogramWindow();

  CvPoint start, finish;
  double meanK = 0.0;

  for (int i = 0; i < nNets; i++)
  {
    meanK += x[2*nClusters + i];
    start.x = HNormalX2ImageX((double)i/(double)nNets);
    start.y = HNormalY2ImageY(0.0);
    finish.x = start.x;
    finish.y = HNormalY2ImageY(x[2*nClusters + i]/maxKi);
    cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(color));
  }

  //plot one line
  start.x  = HNormalX2ImageX(0.0);
  finish.x = HNormalX2ImageX(1.0);
  start.y  = HNormalY2ImageY(1.0/maxKi);
  finish.y = HNormalY2ImageY(1.0/maxKi);
  cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(Color_Black), 1);

  //plot mean ki
  meanK /= nNets;
  start.x  = HNormalX2ImageX(0.0);
  finish.x = HNormalX2ImageX(1.0);
  start.y  = HNormalY2ImageY(meanK/maxKi);
  finish.y = HNormalY2ImageY(meanK/maxKi);
  cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(Color_Black), 1);
}

//Draw methods
void HPlotter::DrawLine(double x1, double y1, double x2, double y2, Color col)
{
  if (IsEnabled())
  {
    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    cvLine(IMG, start, finish, GetCvColor(col), 1);
    _AutoRefresh();
  }
}

void HPlotter::DrawCircle(double x, double y, int radius, Color col)
{
  if (IsEnabled())
  {
    CvPoint center;
    center.x = DesignX2ImageX(x);
    center.y = DesignY2ImageY(y);
    cvCircle(IMG, center, radius, GetCvColor(col), 2);
    _AutoRefresh();
  }
}

void HPlotter::DrawRectangle(double x1, double y1, double x2, double y2, Color col)
{
  if (IsEnabled())
  {
    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    cvRectangle(IMG, start, finish, GetCvColor(col), 1);
    _AutoRefresh();
  }
}

void HPlotter::DrawBar(double x1, double y1, double x2, double y2, Color col)
{
  if (IsEnabled())
  {
    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    cvRectangle(IMG, start, finish, GetCvColor(col), CV_FILLED);
    _AutoRefresh();
  }
}

void HPlotter::DrawTileWires(double x1, double y1, double x2, double y2, int nLines, int nMaxLines)
{
  if (!IsEnabled())
    return;

  CvPoint start, finish;
  start.x = DesignX2ImageX(x1);
  start.y = DesignY2ImageY(y1);
  finish.x = DesignX2ImageX(x2);
  finish.y = DesignY2ImageY(y2);
  CvScalar minColor = cvScalar(255.0, 255.0, 255.0);
  CvScalar maxColor = cvScalar(0.0, 0.0, 0.0);
  CvScalar resColor;

  if (nLines >= nMaxLines)
    resColor = maxColor;
  else
    resColor = cvScalar(255 - 255 * nLines / nMaxLines, 255 - 255 * nLines / nMaxLines,
    255 - 255 * nLines / nMaxLines);
  //resColor = cvScalar(255 * nLines / nMaxLines, 255 * nLines / nMaxLines, 255 * nLines / nMaxLines);

  cvRectangle(IMG, start, finish, resColor, CV_FILLED);
  _AutoRefresh();
}

void HPlotter::DrawTilePins(double x1, double y1, double x2, double y2, int nPins, int nMaxPins)
{
  if (!IsEnabled())
    return;

  CvPoint start, finish;
  start.x = DesignX2ImageX(x1);
  start.y = DesignY2ImageY(y1);
  finish.x = DesignX2ImageX(x2);
  finish.y = DesignY2ImageY(y2);
  CvScalar minColor = cvScalar(255.0, 255.0, 255.0);
  CvScalar maxColor = cvScalar(0.0, 0.0, 0.0);
  CvScalar resColor;

  if (nPins >= nMaxPins)
    resColor = maxColor;
  else
    resColor = cvScalar(255 - 255 * nPins / nMaxPins, 255 - 255 * nPins / nMaxPins,
    255 - 255 * nPins / nMaxPins);

  cvRectangle(IMG, start, finish, resColor, CV_FILLED);
  _AutoRefresh();
}

bool HPlotter::_IsEnabled()
{
  return m_hd.cfg.ValueOf("plotter.enabled", false);
}

void HPlotter::_AutoRefresh()
{
  if (!IsEnabled())
    return;

  if (m_Data->drawingsNum++ % m_Data->autoRefreshStep == 0)
  {
    Refresh(NO_WAIT);
  }
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
          for (int i = 0; (i < n) && !isbuf; i++)
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