#include "data_structures.h"
#include "parser.h"
#include "RouteTrees.h"
#include "itlTiming.h"


void main(int argc, char* argv[])
{
  Circuit    circuit;
  Statistics statistics;

  CMDParse(argc, argv);
  
  // initialize all data structures, parse benchmark and so on
  Initialization(circuit, statistics);

  //srand(time(0));
  //for(int i = 0; i < circuit.nNodes; i++)
  //{
  //    circuit.placement[i].xCoord = rand();
  //    circuit.placement[i].yCoord = rand();
  //}

  double slack = 0;
  FILE* netout = fopen("RoutedNets.txt","w");
  for(int j = 0; j < circuit.nNets; j++)
  {
      fprintf(netout,"net%d  %s  %d\n",j,circuit.nets[j].name,circuit.nets[j].numOfPins);
      fflush(netout);
      if(circuit.nets[j].numOfPins > 500)
      {
          cout << "Net " << j << "    " << circuit.nets[j].name << " SKIPPED with " << circuit.nets[j].numOfPins << " pins." << endl;
          continue;
      }
      MakeRouteTree(circuit,circuit.nets[j]);
      AnalizeTiming(circuit,circuit.nets[j]);
      double max = 0;
      for(int l = 0; l < circuit.nets[j].numOfPins; l++)
          if(circuit.nets[j].arrPins[l].routeInfo->type != NODE_TYPE_SOURCE)
              if(circuit.nets[j].arrPins[l].routeInfo->ArrivalTime > max)
                  max = circuit.nets[j].arrPins[l].routeInfo->ArrivalTime;
              //slack += circuit.nets[j].arrPins[l].routeInfo->ArrivalTime;
      slack +=max;
      fprintf(netout, "Net slack = %10.2f\n", max);
      PrintTree(circuit.nets[j].tree,netout);
  }
  fclose(netout);
  printf("Total negative slack = -%10.4f\n",slack);

  ExportDEF("itl_export.def", circuit);
}