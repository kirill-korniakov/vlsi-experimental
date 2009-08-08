#include "PlacementRowUtils.h"

using namespace Utils;

void Utils::GeneratePlacementRows(HDesign& design)
{
  if(design.Sites.Count() == 0)
  {
    printf("ERROR: sites not found. Unable to generate placement rows.");
    exit(1);
  }

  HSites::Enumerator site = design.Sites.GetEnumerator();
  bool found = false;
  while(site.MoveNext())
    if(design.GetDouble<HSite::Height>(site) != 0 
      && design.GetDouble<HSite::Width>(site) != 0)
    {
      found = true;
      break;
    }
  if(!found)
  {
    printf("ERROR: all sites has zero dimentions. Unable to generate placement rows.");
    exit(1);
  }

  double y = 0.0;
  double sheight = design.GetDouble<HSite::Height>(site);
  double swidth  = design.GetDouble<HSite::Width>(site);
  int hsitescount = (int)(design.Circuit.Width() / swidth);
  int rowscount = (int)(design.Circuit.Height() / sheight);
  double rowsXOffset = design.Circuit.MinX() + (design.Circuit.Width() - hsitescount * swidth) / 2.0;
  double rowsYOffset = design.Circuit.MinY() + (design.Circuit.Height() - rowscount * sheight) / 2.0;
  design.PlacementRows.Initialize(1 + rowscount);
  char name[256];

  HPlacementRow prevRow = design.PlacementRows.Null();
  for(int i = 0; i < rowscount; i++)
  {
    HPlacementRowWrapper row = design[design.PlacementRows.AllocateRow()];
    row.SetHorizSitesCount(hsitescount);
    sprintf(name, "ROW_%d", i);
    row.SetName(name);
    row.SetOrientation(Orientation_N);
    row.SetSite(site);
    row.SetSiteHeight(sheight);
    row.SetSiteWidth(swidth);
    row.SetVertSitesCount(1);
    row.SetX(rowsXOffset);
    row.SetY(rowsYOffset + i * sheight);
    
    if(!::IsNull(prevRow))
    {
      row.SetPreviousRow(prevRow);
      design.Set<HPlacementRow::NextRow, HPlacementRow>(prevRow, row);
    }
    prevRow = row;
  }
}

void Utils::CalcPlacementRectangle(HDesign& design)
{
  //initialize with dummy values
  double maxx = design.Circuit.MinX();
  double maxy = design.Circuit.MinY();
  double minx = maxx + design.Circuit.Width();
  double miny = maxy + design.Circuit.Height();

  for (HPlacementRows::EnumeratorW r = design.PlacementRows.GetEnumeratorW(); r.MoveNext(); )
  {
    if (maxx < r.X() + r.HorizSitesCount() * r.SiteWidth())
      maxx = r.X() + r.HorizSitesCount() * r.SiteWidth();
    if (minx > r.X())
      minx = r.X();
    if (maxy < r.Y() + r.VertSitesCount() * r.SiteHeight())
      maxy = r.Y() + r.VertSitesCount() * r.SiteHeight();
    if (miny > r.Y())
      miny = r.Y();
  }

  design.Circuit.SetPlacementMaxX(maxx);
  design.Circuit.SetPlacementMaxY(maxy);
  design.Circuit.SetPlacementMinX(minx);
  design.Circuit.SetPlacementMinY(miny);
}