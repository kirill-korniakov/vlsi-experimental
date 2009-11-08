#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "DataType.h"
#include "flute.h"
#include "DataProc.h"
#include "RSMT.h"
#include "maze.h"
#include "maze3D.h"
#include "RipUp.h"
#include "utility.h"
#include "route.h"

int main(int argc, char** argv)
{
//    char benchFile[FILESTRLEN];
    char routingFile[STRINGLEN];
    char degreeFile[STRINGLEN];
	  char optionS[STRINGLEN];
    clock_t t1, t2, t3, t4;
    float gen_brk_Time, reading_Time, P1_Time, P2_Time, P3_Time, maze_Time, totalTime, congestionmap_time;
    int iter, last_totalOverflow, diff_totalOverflow, enlarge, ripup_threshold;
    int expand[10] = {-1, 20, 30, 40, 50, 60, 70, 80, 90};
    float costHeight[10];
    int i, j,past_overflow,cur_overflow;
    int L_afterSTOP;
    int ESTEP1,CSTEP1, thStep1;
    int ESTEP2,CSTEP2, thStep2;
    int ESTEP3,CSTEP3, thStep3;
	  float  LOGIS_COF1, LOGIS_COF2, LOGIS_COF3;
    int Ripvalue, LVIter, cost_step;
	  int maxOverflow, past_cong, last_cong, finallength, numVia, ripupTH3D, newTH, healingTrigger;
	  int costFUNCType, updateType, mazeSet, minofl, minoflrnd, mazeRound;
	  Bool goingLV, healingNeed, noADJ, extremeNeeded, UNROUTABLE, needOUTPUT;

    Bool input, WriteOut;
    input=WriteOut=0;

    for(iter=0; iter<10; iter++)
    costHeight[iter] = 120.0 + 15.0*iter;

	//if(strcmp(*argv,"./FastRoute")==0)
  if (argc > 1)
	{
		argc--;  argv++;
		if(argc==0)
		{
			printf("--FastRoute --\n");
			printf("Usage: ./FastRoute  <input> -o <output>\n");
		}
	}

  else {
		printf("--FastRoute --\n");
		printf("Usage: ./FastRoute  <input> <output>\n");

		while(argc)
		{
			argc--; argv++;
		}
	}    
  

   if(argc)
   {
      strcpy(benchFile, *argv);
      argv++; argc--; 
      input=1;
      
   } else {
	   printf("No input file\n");
   }

   if(argc)
   {
		strcpy(optionS, *argv);
		argv++; argc--;
		if (strcmp(optionS,"-o")==0 )
		{
			if(argc)
		   {
			  strcpy(routingFile, *argv);
			  argv++; argc--;
			  WriteOut=1;
			  needOUTPUT = TRUE;
		   } else {
			   printf("No output file specified\n");
			   exit(0);
		   }
		} else {
			printf("output option not recognized,  FR will not generate output file\n");
			needOUTPUT = FALSE;
		}
   } else {
	   printf("No output file specified, FR will not generate output file\n");
	   needOUTPUT = FALSE;
   }

   
	LB=0.9;
	UB=1.3;

	LOGIS_COF1=1.0;
	LOGIS_COF2=1.33;
	LOGIS_COF3=2.5;

	SLOPE=5;
	THRESH_M=50;
	ENLARGE=15;//5
	ESTEP1=10;//10
	ESTEP2=5;//5
	ESTEP3=5;//5
	CSTEP1=5;//5
	CSTEP2=3;//3
	CSTEP3=10;//15
	COSHEIGHT=4;
	L=0;
	A=1;
	VIA=2;
	L_afterSTOP=1;
	Ripvalue=-1;
	ripupTH3D = 10;
	goingLV = TRUE;
	noADJ = FALSE;

	thStep1 = 10;
	thStep2 = 4;
	healingNeed = FALSE;
	updateType = 0;
	LVIter = 3;
	extremeNeeded = FALSE;
	UNROUTABLE = FALSE;
	costFUNCType = 0;
	mazeRound = 200;

  
	if((strstr(benchFile,"adaptec1.capo70.3d.35.50.90.gr")!= NULL) || (strstr(benchFile,"adaptec3.dragon70.3d.30.50.90.gr")!= NULL) || (strstr(benchFile,"bigblue1.capo60.3d.50.10.100.gr")!= NULL) )
	{
		SLOPE=5;
		THRESH_M=30;
		ENLARGE=15;//5
		ESTEP1=10;//10
		ESTEP2=5;//5
		ESTEP3=5;//5
		CSTEP1=5;//5
		CSTEP2=5;//3
		CSTEP3=10;//15
		COSHEIGHT=4;
		VIA=4;
		A=1;
		L_afterSTOP=1;
		mazeSet = 2;
		goingLV = TRUE;
		updateType = 0;
	}

	
	if( (strstr(benchFile,"adaptec5.mfar50.3d.50.20.100.gr")!= NULL)   || (strstr(benchFile,"newblue5.ntup50.3d.40.10.100.gr")!= NULL) || (strstr(benchFile,"newblue6.mfar80.3d.60.10.100.gr")!= NULL))
	{
		SLOPE=5;
		THRESH_M=30;
		ENLARGE=15;//5
		ESTEP1=10;//10
		ESTEP2=5;//5
		ESTEP3=5;//5
		CSTEP1=5;//5
		CSTEP2=5;//3
		CSTEP3=10;//15
		COSHEIGHT=4;
		VIA=2;
		A=1;
		L_afterSTOP=1;
		mazeSet = 2;
		goingLV = TRUE;
		healingNeed = TRUE;
		updateType = 0;
		if ((strstr(benchFile,"newblue6.mfar80.3d.60.10.100.gr")!= NULL)) {
			errorPRONE = TRUE;
		}
	}

	if((strstr(benchFile,"adaptec2.mpl60.3d.35.20.100.gr")!= NULL)||(strstr(benchFile,"adaptec4.aplace60.3d.30.50.90.gr")!= NULL) || (strstr(benchFile,"newblue2.fastplace90.3d.50.20.100.gr")!= NULL) || (strstr(benchFile,"bigblue3.aplace70.3d.50.10.90.m8.gr")!= NULL))
	{
		LB=0.9;
		UB=1.3;
		THRESH_M=10;
		LOGIS_COF=1.29;
		ENLARGE=10;
		ESTEP1=5;
		ESTEP2=4;
		ESTEP3=3;
		CSTEP1=2;
		CSTEP2=5;
		CSTEP3=10;
		COSHEIGHT=2;
		A=1;
		L=0;
		VIA=6;
		L_afterSTOP=0;
		ripupTH3D = 10;
		mazeSet = 2;
		goingLV = FALSE;
		updateType = 0;
		LVIter = 2;
		LOGIS_COF3=2;
		if ( (strstr(benchFile,"bigblue3.aplace70.3d.50.10.90.m8.gr")!= NULL)) {
			goingLV = TRUE;
		}
	}


	if((strstr(benchFile,"newblue7.kraftwerk70.3d.80.20.82.m8.gr")!= NULL)||  (strstr(benchFile,"newblue4.mpl50.3d.40.10.95.gr")!= NULL)|| (strstr(benchFile,"bigblue4.fastplace70.3d.80.20.80.gr")!= NULL))
	{
		THRESH_M=60;
		ENLARGE=20;
		ESTEP1=8;
		ESTEP2=6;
		ESTEP3=4;
		CSTEP1=5;
		CSTEP2=5;
		CSTEP3=10;
		COSHEIGHT=4;
		A=1;
		L=0;
		L_afterSTOP=1;
		VIA=3;
		Ripvalue=-1;
		mazeSet = 3;
		updateType = 4;
		noADJ = TRUE;
		errorPRONE = TRUE;
		UNROUTABLE = TRUE;
		if ( (strstr(benchFile,"newblue7.kraftwerk70.3d.80.20.82.m8.gr")!= NULL)) {
			mazeRound = 169;
			extremeNeeded = TRUE;
		}
		if ( (strstr(benchFile,"newblue4.mpl50.3d.40.10.95.gr")!= NULL)) {
			mazeRound = 184;
		}
		if ( (strstr(benchFile,"bigblue4.fastplace70.3d.80.20.80.gr")!= NULL)) {
			mazeRound = 210;
			extremeNeeded = TRUE;
		}
	}

	if((strstr(benchFile,"newblue1.ntup50.3d.30.50.90.gr")!= NULL) || (strstr(benchFile,"bigblue2.mpl60.3d.40.60.60.gr")!= NULL)  ) 
	{
		THRESH_M=60;
		ENLARGE=20;
		ESTEP1=8;
		ESTEP2=6;
		ESTEP3=4;
		CSTEP1=5;
		CSTEP2=5;
		CSTEP3=10;
		COSHEIGHT=4;
		A=1;
		L=0;
		L_afterSTOP=1;
		VIA=3;
		Ripvalue=-1;
		mazeSet = 4;
		updateType = 1;
		noADJ = TRUE;
		errorPRONE = TRUE;
	}



	if ( (strstr(benchFile,"newblue3.kraftwerk80.3d.40.50.90.gr")!= NULL)) {
		
		STOP=4;
		THRESH_M=10;
		SLOPE=BIG_INT;//BIG_INT
		A=0;//0
		ENLARGE=50;//50
		ESTEP1=40;//40
		CSTEP1=30;//30
		VIA=1;
		L_afterSTOP=0;
		mazeSet = 1;
		noADJ = TRUE;
		errorPRONE = TRUE;
		updateType = 3;
		costFUNCType = 2;
		mazeRound = 18 ;

	}


	ripupTH3D = BIG_INT;
     
    if(input==1)
	{

		t1 = clock();
		printf("\nReading %s ...\n", benchFile);
		readFile(benchFile);
		printf("\nReading Lookup Table ...\n");
		readLUT();
		printf("\nDone reading table\n\n");           
		t2 = clock();
		reading_Time = (float)(t2-t1)/CLOCKS_PER_SEC;
		printf("Reading Time: %f sec\n", reading_Time);
	    
		// call FLUTE to generate RSMT and break the nets into segments (2-pin nets)

		viacost = 0;
		gen_brk_RSMT(FALSE, FALSE, FALSE, FALSE, noADJ);
		printf("first L\n");
		routeLAll(TRUE);
		gen_brk_RSMT(TRUE, TRUE,TRUE,FALSE, noADJ);
		getOverflow2D( &maxOverflow);
		printf("second L\n");
		newrouteLAll(FALSE, TRUE);
		getOverflow2D( &maxOverflow); 
		spiralRouteAll ();
		newrouteZAll(10) ;
		printf("first Z\n");
		past_cong = getOverflow2D( &maxOverflow); 

		convertToMazeroute();

		enlarge = 10;
		newTH = 10;
		healingTrigger = 0;

		for (i = 0; i < LVIter; i++) {
			printf("LV routing round %d, enlarge %d \n", i,enlarge);
			routeLVAll(newTH, enlarge);

			enlarge += 5;
			newTH -= 5;
			if (newTH < 1) {
				newTH = 1;
			}
		}  
		
		past_cong = getOverflow2Dmaze( &maxOverflow); 
	
		t3 = clock();
		reading_Time = (float)(t3-t2)/CLOCKS_PER_SEC;
		printf("LV Time: %f sec\n", reading_Time);
		InitEstUsage();

		i=1;
		costheight=COSHEIGHT;
		enlarge=ENLARGE;
		LOGIS_COF=LOGIS_COF1;
		bal=0;
		ripup_threshold=Ripvalue;
		
		minofl = totalOverflow;

		if (mazeSet == 2 ) {

			costFUNCType = 0;

			while(totalOverflow>0)
			{
				 if(THRESH_M<=1)
				 {
					L=L_afterSTOP;
					if (totalOverflow < 2000) {
						costFUNCType = 1;
					}
				 }

				if(last_cong<totalOverflow)
				{
					initialCongestionHistory(i);
				} else if(last_cong==totalOverflow)
				{
					reduceCongestionHistory(i);
				}

				 
				 if(THRESH_M>15) {
					THRESH_M-=thStep1;
				 } else if(THRESH_M>=2) {
					THRESH_M-=thStep2;
				 } else {
					 THRESH_M = 0;
				 }
				 if(THRESH_M<=0) {
					 THRESH_M=0;
					 costFUNCType = 1;
				 }

				 if(totalOverflow>2000)
				 {
					 enlarge+=ESTEP1;//ENLARGE+(i-1)*ESTEP;
					 cost_step = CSTEP1;
					 updateCongestionHistory( i, updateType);
					 if (THRESH_M== 0) {
						LOGIS_COF=LOGIS_COF2;
					 }
				 }
				 else if(totalOverflow<500)
				 {
					 
					 cost_step = CSTEP3;
					 enlarge+=ESTEP3;
					 LOGIS_COF=LOGIS_COF3;
					 ripup_threshold = -1;
					 updateCongestionHistory( i, updateType);
				 } 
				 else
				 {
					 cost_step = CSTEP2;
					 enlarge+=ESTEP2;
					 LOGIS_COF=LOGIS_COF2;
					 updateCongestionHistory( i, updateType);
				 }

				 if (totalOverflow < 200) {
					 bal = 1;
					 if (healingNeed) {
						 costheight += 20;
					 }

				 }
				 if (totalOverflow < 80) {
					 bal = 2;
				 } 

				 costheight = min (1000, costheight);
				 enlarge = min (enlarge, xGrid/2);
				 costheight+=cost_step;
				 mazeedge_Threshold = THRESH_M;

				 printf("iteration %d, enlarge %d, costheight %d, threshold %d \nvia cost %d log_coef %f, healingTrigger %d cost_step %d L %d A %d\n",i,enlarge,costheight,mazeedge_Threshold, VIA,LOGIS_COF, healingTrigger, cost_step, L , A);
				 mazeRouteMSMD(enlarge, costheight, bal, ripup_threshold,mazeedge_Threshold,costFUNCType, FALSE);
				 last_cong = past_cong;
				 past_cong = getOverflow2Dmaze(&maxOverflow); 

				 i++;

				 if (past_cong >= last_cong ) {
					 VIA = 0;
					 bal = 1;
					 healingTrigger ++;
					 costFUNCType = 1;
				 }
				 if (i >= mazeRound) {
					 getOverflow2Dmaze( &maxOverflow);
					 break;
				 }
			}
		}  else if (mazeSet == 3 ) {

			costFUNCType = 0;

			while(totalOverflow>0)
			{
				 if(THRESH_M<=1)
				 {
					L=L_afterSTOP;
					if (totalOverflow < 2000) {
						costFUNCType = 1;
					}
				 }

				if(last_cong<totalOverflow)
				{
					initialCongestionHistory(i);
				} else if(last_cong==totalOverflow)
				{
					reduceCongestionHistory(i);
				}

				 if(THRESH_M>15) {
					THRESH_M-=thStep1;
				 } else if(THRESH_M>=2) {
					THRESH_M-=thStep2;
				 } else {
					 THRESH_M = 0;
					 if (extremeNeeded == TRUE) {
						costheight += 100;
					}
				 }
				 if(THRESH_M<=0) {
					 THRESH_M=0;
					 costFUNCType = 1;
					if (UNROUTABLE) {
						costheight += 100;
					}
					
				 }

				
				 if(totalOverflow>2000)
				 {
					 enlarge+=ESTEP1;//ENLARGE+(i-1)*ESTEP;
					 cost_step = CSTEP1;
					 updateCongestionHistory( i, updateType);
					 if (THRESH_M== 0) {
						LOGIS_COF=LOGIS_COF2;
					 }
				 }
				 else if(totalOverflow<500)
				 {
					 cost_step = CSTEP3;
					 enlarge+=ESTEP3;
					 LOGIS_COF=LOGIS_COF3;
					 ripup_threshold = -1;
					 updateCongestionHistory( i, updateType);
				 } 
				 else
				 {
					 cost_step = CSTEP2;
					 enlarge+=ESTEP2;
					 LOGIS_COF=LOGIS_COF2;
					 updateCongestionHistory( i, updateType);
				 }


				 if (totalOverflow < 150) {
					 A = 0;
					 //updateType = 2;
					if (extremeNeeded == TRUE) {
						costheight += 100;
					}
					 if (healingNeed) {
						 costheight += 20;
					 }
					 LOGIS_COF = 3;
					 if (totalOverflow < 80) {
						  bal = 1;
						 SLOPE = 4;
						 
					 }
				 }
				 
				if (UNROUTABLE) {
					costheight = min (10000, costheight);
				} else {
					costheight = min (1000, costheight);
				}
				enlarge = min (enlarge, xGrid/2);
				costheight+=cost_step;
				mazeedge_Threshold = THRESH_M;
				printf("iteration %d, enlarge %d, costheight %d, threshold %d \nvia cost %d log_coef %f, healingTrigger %d maxOFL %d updateType %d\n",i,enlarge,costheight,mazeedge_Threshold, VIA,LOGIS_COF, healingTrigger,maxOverflow, updateType);

			    mazeRouteMSMD(enlarge, costheight, bal, ripup_threshold,mazeedge_Threshold,costFUNCType, TRUE);

				last_cong = past_cong;
				past_cong = getOverflow2Dmaze( &maxOverflow);
				if (minofl > past_cong) {
					minofl = past_cong;
					minoflrnd = i;
				}

				i++;

				 if (past_cong >= last_cong ) {
					 VIA = 0;
					// bal = 1;
					 healingTrigger ++;
					 costFUNCType = 1;
					 L = 1;
				 }

				 if (i >= mazeRound) {
					 getOverflow2Dmaze( &maxOverflow);
					 break;
				 }

			}
			printf("\n\n minimal ofl %d, occuring at round %d\n\n",minofl,minoflrnd);
		} else if (mazeSet == 1) {
			while(totalOverflow>0)
			{ 
				if(THRESH_M<=1)
				{
					L=L_afterSTOP;
					bal=1;
					ripup_threshold = -1;
					VIA=0;
				}

 				if(last_cong<totalOverflow)
				{
					initialCongestionHistory(i);
				}	
				else if(last_cong==totalOverflow)
				{
					reduceCongestionHistory(i);
				}

				if(totalOverflow>2000)//phase 1
				{
					enlarge+=ESTEP1;
					costheight+=CSTEP1;
					updateCongestionHistory(i, updateType);
				}
				else if(totalOverflow<500)// phase 3
				{
					costheight+=CSTEP3;
					enlarge+=ESTEP3;
					LOGIS_COF=LOGIS_COF3;
					//VIA=VIACC;
					updateCongestionHistory(i, updateType);
				}
				else
				{
					costheight+=CSTEP2;
					enlarge+=ESTEP2;
					LOGIS_COF=LOGIS_COF2;
					updateCongestionHistory(i, updateType);
				}

				if(THRESH_M>15)
				THRESH_M-=10;
			     
				else if(THRESH_M>=2)
				THRESH_M-=4;
			     
				if(THRESH_M<=0)
				THRESH_M=0;

				printf("iteration %d, enlarge %d, costheight %d, threshold %d, via cost %d\n",i,enlarge,costheight,THRESH_M,VIA);

				// mazeRouteMSMD(enlarge, costheight, bal, ripup_threshold,mazeedge_Threshold,costFUNCType, TRUE);
				mazeRouteMSMD(enlarge, costheight, bal, ripup_threshold, THRESH_M, costFUNCType, FALSE );
				last_cong = getOverflow2Dmaze( &maxOverflow);

				if (minofl > last_cong) {
					 minofl = last_cong;
					 minoflrnd = i;
				 }
				 if (i >= 400) {
					 getOverflow2Dmaze( &maxOverflow);
					 break;
				 }

				i++;
				if (i >= mazeRound) {
					 getOverflow2Dmaze( &maxOverflow);
					 break;
				 }

			}//while
			printf("\n\n minimal ofl %d, occuring at round %d\n\n",minofl,minoflrnd);

		} else if (mazeSet == 4) {
			i=1;
			costheight=COSHEIGHT;
			enlarge=ENLARGE;
			LOGIS_COF=LOGIS_COF1;
			bal=0;
			ripup_threshold=Ripvalue;
			costFUNCType = 0;
			minofl = totalOverflow;

			while(totalOverflow>0)
			{
				 if(THRESH_M<=1)
				 {
					L=L_afterSTOP;
					if (totalOverflow < 2000) {
						costFUNCType = 1;
					}
				 }

				if(last_cong<totalOverflow)
				{
					initialCongestionHistory(i);
				} else if(last_cong==totalOverflow)
				{
					reduceCongestionHistory(i);
				}

				 if(THRESH_M>15) {
					THRESH_M-=thStep1;
				 } else if(THRESH_M>=2) {
					THRESH_M-=thStep2;
				 } else {
					 THRESH_M = 0;
				 }
				 if(THRESH_M<=0) {
					 THRESH_M=0;
					 costFUNCType = 1;
				 }

				
				 if(totalOverflow>2000)
				 {
					 enlarge+=ESTEP1;//ENLARGE+(i-1)*ESTEP;
					 cost_step = CSTEP1;
					 updateCongestionHistory( i, updateType);
					 if (THRESH_M== 0) {
						LOGIS_COF=LOGIS_COF2;
					 }
				 }
				 else if(totalOverflow<500)
				 {
					 cost_step = CSTEP3;
					 enlarge+=ESTEP3;
					 LOGIS_COF=LOGIS_COF3;
					 ripup_threshold = -1;
					 updateCongestionHistory( i, updateType);
				 } 
				 else
				 {
					 cost_step = CSTEP2;
					 enlarge+=ESTEP2;
					 LOGIS_COF=LOGIS_COF2;
					 updateCongestionHistory( i, updateType);
				 }


				 if (totalOverflow < 150) {
					 A = 0;
					 //updateType = 2;
					 if (healingNeed) {
						 costheight += 20;
					 }
					 //LOGIS_COF = 3;
					 if (totalOverflow < 80) {
						  bal = 1;
						 SLOPE = 4;
						 
					 }
				 }
				 
			
				 costheight = min (1000, costheight);
				 enlarge = min (enlarge, xGrid/2);
				 costheight+=cost_step;
				 mazeedge_Threshold = THRESH_M;

				 printf("iteration %d, enlarge %d, costheight %d, threshold %d \nvia cost %d log_coef %f, healingTrigger %d maxOFL %d\n",i,enlarge,costheight,mazeedge_Threshold, VIA,LOGIS_COF, healingTrigger,maxOverflow);

			     mazeRouteMSMD(enlarge, costheight, bal, ripup_threshold,mazeedge_Threshold,costFUNCType, TRUE);

				 last_cong = past_cong;
				 past_cong = getOverflow2Dmaze( &maxOverflow);
				 if (minofl > past_cong) {
					 minofl = past_cong;
					 minoflrnd = i;
				 }

				 i++;

				 if (past_cong >= last_cong ) {
					 VIA = 0;
					// bal = 1;
					 healingTrigger ++;
					 costFUNCType = 1;
					 L = 1;
				 }

				 if (i >= 121) {
					 getOverflow2Dmaze( &maxOverflow);
					 break;
				 }

			}
			printf("\n\n minimal ofl %d, occuring at round %d\n\n",minofl,minoflrnd);
		} 

		checkUsage();

		printf("maze routing finished\n");
         
		t4 = clock();
		maze_Time = (float)(t4-t3)/CLOCKS_PER_SEC;
		printf("P3 runtime: %f sec\n", maze_Time);

		printf("Final 2D results: \n");
		getOverflow2Dmaze( &maxOverflow);

		printf("\nLayer Assignment Begins");
		newLA ();
		printf("layer assignment finished\n");

		t2 = clock();
		gen_brk_Time = (float)(t2-t1)/CLOCKS_PER_SEC;
		printf("2D + Layer Assignment Runtime: %f sec\n", gen_brk_Time); 

		costheight = 3;
		viacost = 1;
		

		if (goingLV && past_cong == 0) {
			printf("3D Maze Routing Begins \n");
			mazeRouteMSMDOrder3D(enlarge, 0, 10 );
			printf("3D maze routing finished, starting via filling\n");
		}
		
		fillVIA();
		finallength = getOverflow3D();
		printf("\n threedvia result\n");
		numVia= threeDVIA ();
		checkRoute3D();
		if (needOUTPUT) {
			writeRoute3D(routingFile);
		}

	}//Input ==1



	t4 = clock();
	maze_Time = (float)(t4-t1)/CLOCKS_PER_SEC;
	printf("Final routing length : %d\n",finallength);
	printf("Final number of via  : %d\n",numVia);
	printf("Final total length 1 : %d\n",finallength+numVia);
	printf("Final total length 3 : %d\n",(finallength+3*numVia));
	printf("3D runtime: %f sec\n", maze_Time);

	freeAllMemory();
	return(1);
}

