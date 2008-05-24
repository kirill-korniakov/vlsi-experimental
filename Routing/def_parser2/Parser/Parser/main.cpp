#include "tiles_in_net.h"
#include "data_structures.h"
#include "parser.h"
#include "RouteTrees.h"
#include "itlTiming.h"

#include "FBI/fbi.h"

void main(int argc, char* argv[])
{//parser.exe -f <filename>.aux
  Circuit    circuit;
  Statistics statistics;

  CMDParse(argc, argv);
  
  // initialize all data structures, parse benchmark and so on
  Initialization(circuit, statistics);
  
  make_trees_in_nets(circuit);
  PrintTimingReport(circuit, "timing");
  //system("PAUSE");

  //Tiles_in_net *tiles_in_net = new Tiles_in_net [circuit.nNets];
  
}
