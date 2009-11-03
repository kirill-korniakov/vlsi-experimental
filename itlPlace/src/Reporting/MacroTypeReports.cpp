#include "Reporting.h"
#include "Utils.h"


void ReportCellByMacroType(HDesign& design)
{
  WRITELINE("");
  WRITELINE("Reporting: Number of pins for macrotype");

  design.MacroTypes.Count();
  string * namesMT = new string [design.MacroTypes.Count()];
  int * countsCell = new int [design.MacroTypes.Count()];

  for (int i = 0; i < design.MacroTypes.Count(); i++)
    countsCell[i] = 0;
  int j = 0;
  int maxLen = 0;
  for (HMacroTypes::EnumeratorW enumeratorMT = design.MacroTypes.GetEnumeratorW(); 
    enumeratorMT.MoveNext(); j++)
  {
    namesMT[j] = enumeratorMT.Name();
    if (maxLen < int(namesMT[j].length()))
      maxLen = namesMT[j].length();
  }
  string name;
  for (HCells::CellsEnumeratorW cell = design.Cells.GetEnumeratorW(); cell.MoveNext(); )
  {
    if (cell.PlacementStatus() != PlacementStatus_Movable
      && cell.PlacementStatus() != PlacementStatus_Fixed)
      continue;

    HCellWrapper cw = design[cell] ;
    name = design.MacroTypes.GetString<HMacroType::Name>(cw.Type());
    int k = 0;
    bool find = true;
    while ((k < design.MacroTypes.Count()) && (find == true))
    {  
      if (name == namesMT[k])
      {
        find = false;
        countsCell[k]++;
      }
      k++;
    }
  }

  for (int i = 0; i < design.MacroTypes.Count(); i++)
  {
    string nBlank;
    for (int j = namesMT[i].length(); j < maxLen; j++)
      nBlank +=" ";
    WRITELINE("  Number of cells entering in macro type %s %s = %d", 
      namesMT[i].c_str(), nBlank.c_str(), countsCell[i]);
  }

  delete [] namesMT;
  delete [] countsCell;
}




void ReportMacroTypeInfo(HDesign& design)
{
  WRITELINE("");
  WRITELINE("Reporting: physical information about macro type\n");

  for (HMacroTypes::EnumeratorW enumeratorMT = design.MacroTypes.GetEnumeratorW(); 
    enumeratorMT.MoveNext(); )
  {

    WRITELINE("  MacroTypes %s\t parameters:", enumeratorMT.Name().c_str());
    WRITELINE("   # size X = %f;\n # size Y = %f",
      enumeratorMT.SizeX(), enumeratorMT.SizeY());
    WRITELINE("   # type = %s", Utils::GetMacroFunctionName(enumeratorMT.Type()) ); 
    WRITELINE("   # pins count =%d", enumeratorMT.PinsCount());
    WRITELINE("  Reporting:  information about pins");
    double xDisplacement = 0, yDisplacement = 0;

    for (HMacroType::PinsEnumeratorW enuPin = 
      enumeratorMT.GetEnumeratorW(); enuPin.MoveNext();)
    {
      WRITELINE("   Pin name     =\t%s", enuPin.Name().c_str());
      WRITELINE("     # Direction =\t%d", enuPin.Direction());
      WRITELINE("     # Function  =\t%s", Utils::GetPinFunctionStr(enuPin.Function()));

      //извиняюсь, но дальше написана хрень какая-то
      //в таком виде это просто нельзя использовать
      //for (HCells::CellsEnumeratorW cell = 
      //  design.Cells.GetEnumeratorW(); cell.MoveNext(); )
      //  for (HCell::PinsEnumeratorW enuPinCell = 
      //    cell.GetPinsEnumeratorW(); enuPinCell.MoveNext();)
      //    if (enuPinCell.Name() == enuPin.Name())
      //    {                       
      //      xDisplacement = enuPinCell.OffsetX();
      //      yDisplacement = enuPinCell.OffsetY();
      //      break;
      //    }  


      //WRITELINE("     # Displacement of cell : ");
      //WRITELINE("               X =\t%f", xDisplacement);
      //WRITELINE("               Y =\t%f\n", yDisplacement);

    }
  }

}