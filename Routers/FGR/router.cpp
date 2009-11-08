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

int main(int argc, char **argv)
{
  //outfile << "FGR " << FGRversion << " (" << sizeof(void*)*8 << "-bit) Compiled on "<<"???"; /// << COMPILETIME;
#ifdef __GNUC__
  //cout << " with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#endif
  //cout << endl;

  // parameters
  string outputfile;
  string resultsFile;

  for(int i = 2; i < argc; ++i)
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
  }

  FGRParams parms(argc, argv);

  FGR fgr(parms);

  // read in the nets and create the grid
  fgr.parseInput(argv[1]);

  ofstream outfile(resultsFile.c_str());

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

  fgr.plotXPM(parms.outputFile);
  //system("PAUSE");
  return 0;
}
