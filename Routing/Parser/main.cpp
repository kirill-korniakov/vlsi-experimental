#include "tiles_in_net.h"
#include "data_structures.h"
#include "parser.h"
#include "RouteTrees.h"
#include "itlTiming.h"

#include "FBI/fbi.h"

void main(int argc, char* argv[])
{
  Circuit    circuit;
  Statistics statistics;

  CMDParse(argc, argv);
  
  // initialize all data structures, parse benchmark and so on
  Initialization(circuit, statistics);
  
  cout<<"started pushing pins\n";
  cout<<"numpins= "<<circuit.nets[0].numOfPins<<endl;

  Tiles_in_net *tiles_in_net = new Tiles_in_net [circuit.nNets];
  FILE *tiles = fopen("01mpl.lab.res","r");

 if( tiles == NULL) {
		cout<<"cann't open file with tiles\n";
		return;
 }

 //tiles_in_net[1].create_tiles(tiles, circuit.nets[1].numOfPins);
 tiles_in_net[1].tiles = new Tile [3];
 tiles_in_net[1].tiles_num = 3;
 tiles_in_net[1].push_pins_into_tiles(circuit.nets[0].arrPins, circuit.nets[0].numOfPins, 0, circuit);
 cout<<"tile x="<<tiles_in_net[1].tiles[0].x<<endl;
 cout<<"   "<<tiles_in_net[1].tiles[1].center->x<<endl;
 tiles_in_net[1].tiles[0].connect_center_with_pins(circuit);
 cout<<"connected\n";
 circuit.nets[0].tree = new RoutingTree;
 circuit.nets[0].tree->nodes = tiles_in_net[1].tiles[0].center;
 //cout<<"x=="<<circuit.nets[0].tree->nodes->x<<endl;
 PrintTree(circuit.nets[0].tree, stdout);

/*  for(int i = 0; i < circuit.nNets; i++) {
	  tiles_in_net[i].create_tiles(tiles, circuit.nets[i].numOfPins);

	  int source_ind = circuit.nets[i].sourceIdx;
	  
	  if( source_ind < 0 ) //no source
		  source_ind = 0;	//the first pin in this net

	  tiles_in_net[i].push_pins_into_tiles(circuit.nets[i].arrPins, circuit.nets[i].numOfPins, source_ind, circuit);
	  tiles_in_net[i].make_connections(tiles_in_net[i].main_tile_ind);

	  for(int j = 0; j < tiles_in_net[i].tiles_num; j++)
		  tiles_in_net[i].tiles[j].connect_center_with_pins(circuit);

	  //connect source with center
	  StNodeEx *source = CreateNodeEx();
	  source->x = circuit.placement[circuit.nets[i].sourceIdx].xCoord;
	  source->y = circuit.placement[circuit.nets[i].sourceIdx].yCoord;
	  source->origin = &circuit.nets[i].arrPins[source_ind];
	  source->type = get_pin_type(source->origin);
	  tiles_in_net[i].connect_source_with_center(source);
	  
	  //creating tree for net
	  circuit.nets[i].tree = new RoutingTree;
	  circuit.nets[i].tree->nodes = source;
  }*/

  ;
 
  /*Tiles_in_net *tiles_in_net = new Tiles_in_net(6);
  cout<<"tiles_num: "<<tiles_in_net[1].tiles_num<<endl;
  tiles_in_net->push_pins_into_tiles(circuit.nets[2467].arrPins, circuit.nets[2467].numOfPins, 0, circuit);
 
  cout<<"\nfinished pushing\n";
  
  tiles_in_net->tiles[1].connect_center_with_pins(circuit);
  //if(tiles_in_net->tiles[0].center->right != NULL)
  cout<<"x "<<tiles_in_net->tiles[1].center->x<<endl;
  cout<<"y "<<tiles_in_net->tiles[1].center->y<<endl;
  cout<<"left->right->y "<<tiles_in_net->tiles[1].center->left->right->y<<endl;
  cout<<"ntiles= "<<tiles_in_net->tiles_num<<endl;
  delete tiles_in_net;
  cout<<"tin deleted\n";*/
  //cout<<"curnum0= "<<tiles_in_net->tiles[0].cur_num<<"; npit0= "<<tiles_in_net->tiles[0].npit<<endl;
  
}
