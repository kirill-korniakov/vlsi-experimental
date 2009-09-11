#include <string>
#include <vector>
#include <map>
#include <limits>
#include <fstream>

using std::vector;
using std::pair;
using std::endl;
using std::string;
using std::map;
using std::numeric_limits;
using std::ofstream;

#include "FGR.h"
#include "stdTypes.h"
using namespace fgr;

int fgr::FGRRouting(HDPGrid& grid)
{
  // parameters
  string outputfile;
  string resultsFile;

  /*for(int i = 2; i < argc; ++i)
  {
    if(argv[i] == string("-o"))
    {
      if(i+1 < argc)
      {
        outputfile = argv[++i];
        resultsFile = outputfile + ".results";
      }
      else
      {
        //outfile << "option -o requires an argument" << endl;
        FGRParams::usage(argv[0]);
        return 0;
      }
    }
  }*/

  //FGRParams parms(argc, argv);
  FGRParams parms;
  parms.labyrinth   = true;
  parms.layerAssign = false;
  
  string tmpstr = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.FGROutputFile", "bench.fgr");
  int pos = 0;
  
  while (tmpstr[pos] != '.')
    pos++;

  tmpstr[pos + 1] = 'f';
  tmpstr[pos + 2] = 'g';
  tmpstr[pos + 3] = 'r';

  parms.outputFile = tmpstr;
  parms.resultsFile = tmpstr + ".results";

  FGR fgr(parms);

  // read in the nets and create the grid
  //fgr.parseInput(argv[1]);
  fgr.parseInputAsLab(grid);

  ofstream outfile(resultsFile.c_str(), std::ios::app);

  if(!outfile.good())
  {
    outfile << "Could not open `" << resultsFile << "' for writing." << endl;
  }

  outfile << "CPU time: " << cpuTime() << " seconds" << endl;

  fgr.printParams();

  fgr.initialRouting();

  double rrrStartCpu = cpuTime();

  fgr.doRRR();

  outfile << endl << "after all rip-up iterations" << endl;
  fgr.printStatistics();

  double rrrEndCpu = cpuTime();

  outfile << "RRR CPU time: " << rrrEndCpu-rrrStartCpu << " seconds " << endl;

  outfile << endl << "assigning layers" << endl;
  fgr.layerAssignment();

  outfile << "after layer assignment" << endl;
  fgr.printStatistics();

  outfile << endl << "greedy improvement phase" << endl;
  double greedyStartCpu = cpuTime();
  fgr.greedyImprovement();

  outfile << endl << "after greedy improvement phase" << endl;
  fgr.printStatistics();

  double greedyEndCpu = cpuTime();

  outfile << "greedy CPU time: " << greedyEndCpu-greedyStartCpu << " seconds " << endl;

  outfile << endl;
  fgr.printStatistics(true,true);

  if(!parms.outputFile.empty())
  {
    fgr.writeRoutes(parms.outputFile);
  }

  //system("PAUSE");
  outfile.close();
  return 0;
}
