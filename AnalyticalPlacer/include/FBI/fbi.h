#include "def.h"
#include "../data_structures.h"
#include "../RouteTrees.h"

extern double tnslack;
extern int bufcount;
extern int lowSlack;
extern int highSlack;

void InsertBuffers(char *lib, double wcap, double wre, RoutingTree *tree, int netIndex, Circuit &circuit);
void FinalizeFBI();
void InitFBI(Circuit *circuit);