
#ifndef TILES_IN_NET
#define TILES_IN_NET

#include "tile.h"
#include "itlTiming.h"
#include <stdio.h>
#include <list>

#define minx -300
#define miny -300

double min_x() { return minx; }
double min_y() { return miny; }

struct Tiles_in_net {
	int max_tiles_num; //max number of tiles in net
	int tiles_num; //exact number of tiles in net
	Tile *tiles;   //array of tiles
	int main_tile_ind; //index of tile with source

	Tiles_in_net() { max_tiles_num = tiles_num = 0; main_tile_ind = 0; }
	//Tiles_in_net(int num): max_tiles_num(num) {tiles = new Tile [max_tiles_num]; }
	~Tiles_in_net(); 
	
	int add_tile(int _x, int _y);
	void create_tiles(FILE *_f); //generates tiles for net

	void connect_centers(int i1, int i2, bool left); //connects centers of two tiles
	void connect_tiles(int i1, int i2); //first connection
	void make_connections(int ind); //final connection. It is a recursive function!
	void push_pins_into_tiles(Pin _pins[], int pins_num, int source_ind, const Circuit& circuit);
	void connect_source_with_center(StNodeEx *_source);
};

struct Coordinates { //stores coordinates for tiles - needed in create tiles only
	int X;
	int Y;
	Coordinates(int _x, int _y): X(_x), Y(_y) {;}
};

//***********functions for Tiles_in_net****************************

Tiles_in_net::~Tiles_in_net() {
	//cout<<"tin destructor started\n";
	if( tiles_num )
		delete [] tiles;

	cout<<"tin destructor finished\n";
}

int Tiles_in_net::add_tile(int _x, int _y) { //adds tile to array; returns tile's index in array
	cout<<"started adding\n";
	if(tiles_num != 0) {
		for(int i = 0; i < tiles_num; i++) { //this tile has already been created
			if( (tiles[i].x == _x) && (tiles[i].y == _y) ) {
				cout<<"the same tile\n";
				return i;
			}
		}

		tiles[tiles_num].x = _x;  //new tile
		tiles[tiles_num].y = _y;
		tiles[tiles_num].set_center_xy();
		tiles_num++;
		return (tiles_num - 1);
	}

	else {						//the first tile
		tiles[tiles_num].x = _x;
		tiles[tiles_num].y = _y;
		tiles[tiles_num].set_center_xy();
		tiles_num++;
		cout<<"add: tiles_num = "<<tiles_num<<endl;
		return (tiles_num - 1);
	}
}

void Tiles_in_net::connect_tiles(int i1, int i2) {
	if( tiles[i1].x == tiles[i2].x ) {
		if(tiles[i1].y < tiles[i2].y){
			int tmp = i1; i1 = i2; i2 = tmp;
		}

		tiles[i1].nearest_d = i2; //tiles[i2].y;
		tiles[i2].nearest_t = i1; //tiles[i1].y;
	}

	else {
		if( tiles[i1].x > tiles[i2].x) {
			int tmp = i1; i1 = i2; i2 = tmp;
		}

		tiles[i1].nearest_r = i2; //tiles[i2].x;
		tiles[i2].nearest_l = i1; //tiles[i1].x;
	}
}

void Tiles_in_net::connect_centers(int i1, int i2, bool right) {
	if( tiles[i1].x == tiles[i2].x ) {
		if( tiles[i1].center->left != NULL ) {
			if ( right )
				tiles[i1].center->left->right = tiles[i2].center;
			else
				tiles[i1].center->left->left = tiles[i2].center;
		}

		else {
			StNodeEx *pl = CreateNodeEx();
			pl->y = tiles[i1].center->y - tile_edge/3;
			pl->x = tiles[i1].center->x;

			if( right )
				pl->right = tiles[i2].center;
			else
				pl->left = tiles[i2].center;

			tiles[i1].center->left = pl;
		}
	}

	else {
		if( tiles[i1].center->right != NULL ) {
			if( right )
				tiles[i1].center->right->right = tiles[i2].center;
			else
				tiles[i1].center->right->left = tiles[i2].center;
		}

		else {
			StNodeEx *pr = CreateNodeEx();
			pr->y = tiles[i1].center->y;
			pr->x = tiles[i1].center->x + tile_edge/3;

			if( right )
				pr->right = tiles[i2].center;
			else
				pr->left = tiles[i2].center;

			tiles[i1].center->right = pr;
		}
	}
}

void Tiles_in_net::make_connections(int ind) {
	if( ind < 0)
		return;

	int ind1 = tiles[ind].nearest_d;
	int ind2 = tiles[ind].nearest_r;
	int ind3 = tiles[ind].nearest_l;
	int ind4 = tiles[ind].nearest_t;

	if( ind1 >= 0 ) {
		tiles[ind1].nearest_t = -1;
		connect_centers(ind, ind1, 0);
		make_connections(ind1);
	}

	if( ind2 >= 0 ) {
		tiles[ind2].nearest_l = -1;
		connect_centers(ind, ind2, 1);
		make_connections(ind2);
	}

	if( ind3 >= 0 ) {
		tiles[ind3].nearest_r = -1;
		connect_centers(ind, ind3, 0);
		make_connections(ind3);
	}

	if( ind4 >= 0 ) {
		tiles[ind4].nearest_d = -1;
		connect_centers(ind, ind4, 1);
		make_connections(ind4);
	}
}

void Tiles_in_net::create_tiles(FILE *ftiles) {
	//cout<<"started creating\n";
	max_tiles_num = 0;

	std::list<Coordinates> coordinates_list;
	std::list<Coordinates>::iterator coordinates_iterator;
	
	char *_str; //string for reading 
	int m = 40;
	_str = new char [m];
	
	while( fgets(_str, m, ftiles) ) {
		if(_str[0] == 'n')
			break;
	}

	fgets(_str, m, ftiles);
	cout<<_str<<endl;

	//--first define tile'es max number
	while( fgets(_str, m, ftiles) ) {
		if( 'o' == _str[0] ) {
			cout<<_str<<endl;
			char *st = new char [m];
			int x1, y1, x2, y2;
			int i = 0;
			int j = 0;

			while(_str[i] != '(')
				i++;

			i++;

			while(_str[i] != ',')
				st[j++] = _str[i++];

			x1 = atoi(st);
		
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;
			i++;

			while(_str[i] != ')')
				st[j++] = _str[i++];

			y1 = atoi(st);

			
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;

			while(_str[i] != '(')
				i++;

			i++;

			while(_str[i] != ',')
				st[j++] = _str[i++];

			x2 = atoi(st);
		
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;
			i++;

			while(_str[i] != ')')
				st[j++] = _str[i++];

			y2 = atoi(st);

			Coordinates first_tile(x1, y1);
			Coordinates second_tile(x2, y2);

			coordinates_list.push_back(first_tile);
			coordinates_list.push_back(second_tile);
			max_tiles_num++;
			delete [] st;
		}

		else
			break;
	}

	if( max_tiles_num == 0 ) { //only 1 tile
		cout<<"only one tile in this net\n";
		max_tiles_num = 1;
		tiles = new Tile [1];
		char *st = new char [m];
			int x1, y1;
			int i = 0;
			int j = 0;

			while(_str[i] != '(')
				i++;

			i++;

			while(_str[i] != ',')
				st[j++] = _str[i++];

			x1 = atoi(st);
		
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;
			i++;

			while(_str[i] != ')')
				st[j++] = _str[i++];

			y1 = atoi(st);
			int j1 = add_tile(x1,y1);
			delete [] st;
			delete [] _str;
			return;
	}

	//more than one tile: now get tiles from list and add to array
	
	max_tiles_num = coordinates_list.size();
	cout<<"tiles num="<<max_tiles_num<<endl;
	
	//now add tiles 
	tiles = new Tile [max_tiles_num];
	int ci = 0;

	//for(coordinates_iterator = coordinates_list.begin(), ci = coordinates_iterator; ( ++(++ci) ) != coordinates_list.end(); ++coordinates_iterator) {
	for(coordinates_iterator = coordinates_list.begin(); ci < max_tiles_num; ++coordinates_iterator) {
		int x1 = (*coordinates_iterator).X;
		int y1 = (*coordinates_iterator).Y;
		coordinates_iterator++;
		int x2 = (*coordinates_iterator).X;
		int y2 = (*coordinates_iterator).Y;

		cout<<"x1= "<<x1<<"; y1="<<y1<<"; x2="<<x2<<"; y2="<<y2<<endl;
		int j1 = add_tile(x1,y1);
		int j2 = add_tile(x2,y2);
		cout<<"j1= "<<j1<<"; j2= "<<j2<<endl;
		connect_tiles(j1, j2);
		ci += 2;
	}

	coordinates_list.clear();
	delete [] _str;
	cout<<"finished creating\n";
}

void Tiles_in_net::push_pins_into_tiles(Pin _pins[], int pins_num, int source_ind, const Circuit& circuit) {
	
	//-----first - define how many pins belong to each tile
	for(int i = 0, j = 0; i < pins_num; i++) {
		cout<<"pin's coordinates: x= "<<circuit.placement[_pins[i].cellIdx].xCoord<<"; y= "<<circuit.placement[_pins[i].cellIdx].yCoord<<endl;
	  int _x = ((int)(circuit.placement[_pins[i].cellIdx].xCoord + _pins[i].xOffset - min_x()))/tile_edge; //this pin belongs to
      int _y = ((int)(circuit.placement[_pins[i].cellIdx].yCoord + _pins[i].yOffset - min_y()))/tile_edge; //tile(_x,_y)
	  cout<<"x="<<_x<<"y="<<_y<<endl;
 
	  int k = 0;

	  for(k = 0; k < tiles_num; k++) {
		  cout<<"tiles["<<k<<"].x="<<tiles[k].x<<"; .y="<<tiles[k].y<<endl;
		  if( (tiles[k].x == _x) && (tiles[k].y == _y) ) {
			   if( i == source_ind ) {
					main_tile_ind = k;
					k = -1;
					cout<<"found main tile: "<<_x<<_y<<endl;
					//system("PAUSE");
					break;
			   }

			   else {
					tiles[k].inc_npit();
					k = -1;
					cout<<"found tile: "<<_x<<_y<<endl;
					break;
			   }
		  }		  
	  }
	  
	  //cout<<"pushing almost finished\n";
	  if( k == tiles_num ) { //new tile - it mustn't happen
		tiles[j].set_x(_x);
		tiles[j].set_y(_y);
		tiles[j].set_center_xy();
		tiles[j].inc_npit();
		j++;
		tiles_num++;
		cout<<"missed tile( "<<_x<<","<<_y<<")!\n";
		system("PAUSE");
	  }

	  //cout<<"pushing finished\n";
	}

	//----now add pins in appropriate tile--------
	for(int i = 0; i < pins_num; i++) { //pushing pins into tiles
		if(i != source_ind ) {
			int _x = ((int)(circuit.placement[_pins[i].cellIdx].xCoord + _pins[i].xOffset - min_x()))/tile_edge;
			int _y = ((int)(circuit.placement[_pins[i].cellIdx].yCoord + _pins[i].yOffset - min_y()))/tile_edge;
	
			for(int k = 0; k < tiles_num; k++) {
				  if( (tiles[k].x == _x) && (tiles[k].y == _y) ) { //pin belongs to this tile
					  tiles[k].add_pin(_pins[i]);
						break;
				  }
			}

		}
	}
cout<<tiles_num<<" pushing finished\n";
}

void Tiles_in_net::connect_source_with_center(StNodeEx *_source) {
	_source->right = NULL;

	StNodeEx *pl = CreateNodeEx();
	pl->x = _source->x;
	pl->y = tiles[main_tile_ind].center->y;
	pl->origin = NULL;
	pl->type = NODE_TYPE_INTERNAL;
	pl->left = tiles[main_tile_ind].center;

	_source->left = pl;
}

void make_trees_in_nets(Circuit &circuit) {
   cout<<"started pushing pins\n";
   Tiles_in_net *tiles_in_net = new Tiles_in_net [circuit.nNets];
   FILE *tiles = fopen("01-mpl.lab.res","r");

 if( tiles == NULL) {
		cout<<"cann't open file with tiles\n";
		return;
 }

/* for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord += 0.5*circuit.nodes[i].width  - circuit.shiftX;
      circuit.placement[i].yCoord += 0.5*circuit.nodes[i].height - circuit.shiftY;
    }
 for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + circuit.nNodes].xCoord += 0.5*circuit.terminals[i].width  - circuit.shiftX;
      circuit.placement[i + circuit.nNodes].yCoord += 0.5*circuit.terminals[i].height - circuit.shiftY;
    }*/

 for(int i = 0; i < circuit.nNets; i++) {//circuit.nNets-------------------------------------------
	  //cout<<str<<endl;
	  cout<<"                                                step "<<i<<endl;

	  tiles_in_net[i].create_tiles(tiles);

	  int source_ind = circuit.nets[i].sourceIdx;
	  cout<<"soutre: "<<source_ind<<endl;
	  //system("PAUSE");

	  /*for(int l = 0; l < circuit.nets[i].numOfPins; l++) {
		  if(circuit.nets[i].arrPins[l].chtype == 'O') {
				source_ind = l;
				circuit.nets[i].sourceIdx = l;
				break;
		  }
	  }*/
	  
	  if( source_ind < 0 ) //no source
		  source_ind = 0;	//the first pin in this net

	 // cout<<"tiles num = "<<tiles_in_net[i].tiles_num<<"; main tile= "<<tiles_in_net[i].main_tile_ind<<endl;
	  tiles_in_net[i].push_pins_into_tiles(circuit.nets[i].arrPins, circuit.nets[i].numOfPins, source_ind, circuit);
	  
	  cout<<tiles_in_net[i].tiles_num<<endl;
	  //system("PAUSE");
	  if(tiles_in_net[i].tiles_num > 1)
		tiles_in_net[i].make_connections(tiles_in_net[i].main_tile_ind);

	  for(int j = 0; j < tiles_in_net[i].tiles_num; j++)
		  tiles_in_net[i].tiles[j].connect_center_with_pins(circuit);
	  //cout<<"aftre connecting0: "<<tiles_in_net[i].tiles[4].p_nodes[0]->origin->cellIdx<<endl;

	  //connect source with center
	  StNodeEx *source = CreateNodeEx();
	  source->x = circuit.placement[circuit.nets[i].arrPins[source_ind].cellIdx].xCoord + circuit.nets[i].arrPins[source_ind].xOffset;
	  source->y = circuit.placement[circuit.nets[i].arrPins[source_ind].cellIdx].yCoord + circuit.nets[i].arrPins[source_ind].yOffset;
	  cout<<"source: x="<<source->x<<"; y="<<source->y<<"; index= "<<source_ind<<endl;
	  
	  Pin source_pin = circuit.nets[i].arrPins[source_ind];
	  source->origin = new Pin;
	  source->origin->cellIdx = source_pin.cellIdx;
	  source->origin->chtype = source_pin.chtype;
	  source->origin->xOffset = source_pin.xOffset;
	  source->origin->yOffset = source_pin.yOffset;
	  source->type = get_pin_type(source->origin);
	  tiles_in_net[i].connect_source_with_center(source);
	  
	  //-----"connecting" pins with nodes-------------------------
	  for(int j = 0; j < circuit.nets[i].numOfPins; j++) {
		  if(j == source_ind) {
			  circuit.nets[i].arrPins[j].routeInfo = source;
			  continue;
		  }

		  int _index = circuit.nets[i].arrPins[j].cellIdx;
		  int _x = ((int)(circuit.placement[_index].xCoord + circuit.nets[i].arrPins[j].xOffset - min_x()))/tile_edge;
          int _y = ((int)(circuit.placement[_index].yCoord + circuit.nets[i].arrPins[j].yOffset - min_y()))/tile_edge;
		  cout<<"_x = "<<_x<<"; y = "<<_y<<endl;
		  
		  int ind_t = tiles_in_net[i].add_tile(_x, _y);
		  tiles_in_net[i].tiles[ind_t].find_node_for_pin(circuit.nets[i].arrPins[j]);
	  }
	  
	  //creating tree for net
	  RoutingTree *_Tree = new RoutingTree;
	  _Tree->nodes = source;
	  circuit.nets[i].tree = _Tree;
	 
	  //PrintTree(circuit.nets[i].tree, stdout);
	  //ReleaseNodesTree(circuit.nets[i].tree->nodes);
	  PrintTree(_Tree, stdout);
	  CalculateDalays(circuit, circuit.nets[i]);
	  //ReleaseNodesTree(_Tree->nodes);
  }

 system("PAUSE");
 //CalculateDalays(circuit, circuit.nets[0]);

for(int i = 0; i < circuit.nNets; i++) {
	 ReleaseNodesTree(circuit.nets[i].tree->nodes);
}

 delete [] tiles_in_net;
 //system("PAUSE");
}

#endif
