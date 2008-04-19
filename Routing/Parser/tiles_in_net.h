#ifndef TILES_IN_NET
#define TILES_IN_NET

#include "tile.h"
#include <stdio.h>

#define minx -32.5
#define miny -32.5

double min_x() { return minx; }
double min_y() { return miny; }

struct Tiles_in_net {
	int max_tiles_num; //max number of tiles in net (== number of pins in net)
	int tiles_num; //exact number of tiles in net
	Tile *tiles;   //array of tiles
	int main_tile_ind; //index of tile with source

	Tiles_in_net() { max_tiles_num = tiles_num = 0; main_tile_ind = -1; }
	Tiles_in_net(int num): max_tiles_num(num) {tiles = new Tile [max_tiles_num]; }
	~Tiles_in_net() {cout<<"tin destructor started\n"; delete [] tiles; cout<<"tin destructor finished\n";}
	
	int add_tile(int _x, int _y);
	void create_tiles(FILE *_f, int _tiles_num); //generates tiles for net

	void connect_centers(int i1, int i2, bool left); //connects centers of two tiles
	void connect_tiles(int i1, int i2); //first connection
	void make_connections(int ind); //final connection. It is a recursive function!
	void push_pins_into_tiles(Pin _pins[], int pins_num, int source_ind, const Circuit& circuit);
	void connect_source_with_center(StNodeEx *_source);
};

int Tiles_in_net::add_tile(int _x, int _y) { //adds tile to array; returns tile's index in array
	if(tiles_num != 0) {
		for(int i = 0; i < tiles_num; i++) { //this tile has already been created
			if( (tiles[i].x == _x) && (tiles[i].y == _y) )
				return i;
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
			pr->y = tiles[i1].y;
			pr->x = tiles[i2].x + tile_edge/3;

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

void Tiles_in_net::create_tiles(FILE *ftiles, int _tiles_num) {
	max_tiles_num = _tiles_num;
	tiles = new Tile [tiles_num];

	//long int pos = ftell(ftiles);
	char *str; //string for reading tiles'es coordinates
	int m = 40;
	str = new char [m];

	while( fgets(str, m, ftiles) ) {
		  if( 'o' == str[0] ) {
			char *st = new char [m];
			int x1, y1, x2, y2;
			int i = 0;
			int j = 0;

			while(str[i] != '(')
				i++;

			i++;

			while(str[i] != ',')
				st[j++] = str[i++];

			x1 = atoi(st);
		
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;
			i++;

			while(str[i] != ')')
				st[j++] = str[i++];

			y1 = atoi(st);

			
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;

			while(str[i] != '(')
				i++;

			i++;

			while(str[i] != ',')
				st[j++] = str[i++];

			x2 = atoi(st);
		
			for( int k = 0; k <= j; k++)
				st[k] = '\0';

			j = 0;
			i++;

			while(str[i] != ')')
				st[j++] = str[i++];

			y2 = atoi(st);
			cout<<"x1= "<<x1<<"; y1="<<y1<<"; x2="<<x2<<"; y2="<<y2;
			int j1 = add_tile(x1,y1);
			int j2 = add_tile(x2,y2);
			connect_tiles(j1, j2);

			delete [] st;	
		  }

		  else 
			  break;
	}

  delete [] str;
  //fseek(ftiles, pos, SEEK_SET);
}

void Tiles_in_net::push_pins_into_tiles(Pin _pins[], int pins_num, int source_ind, const Circuit& circuit) {
	
	//-----first - define how many pins belong to each tile
	for(int i = 0, j = 0; i < pins_num; i++) {
	  int _x = ((int)(circuit.placement[_pins[i].cellIdx].xCoord - min_x()))/tile_edge; //this pin belongsto
      int _y = ((int)(circuit.placement[_pins[i].cellIdx].yCoord - min_y()))/tile_edge; //tile(_x,_y)
	  cout<<endl<<"x="<<_x<<"y="<<_y<<endl;
 
	  int k = 0;

	  for(k = 0; k < j; k++) {
		  if( (tiles[k].x == _x) && (tiles[k].y == _y) ) {
			   if( i == source_ind ) {
					main_tile_ind = k;
					break;
			   }

			   else {
					tiles[k].inc_npit();
					break;
			   }
		  }		  
	  }
	  
	  cout<<"pushing almost finished\n";
	  if( k == j ) { //new tile - it mustn't happen
		tiles[j].set_x(_x);
		tiles[j].set_y(_y);
		tiles[j].inc_npit();
		j++;
		cout<<"missed tile( "<<_x<<","<<_y<<")\n";
		cout<<"j="<<j<<endl;
	  }
	}

	//----now add pins in appropriate tile--------
	for(int i = 0; i < pins_num; i++) { //pushing pins into tiles
		int _x = ((int)(circuit.placement[_pins[i].cellIdx].xCoord - min_x()))/tile_edge;
		int _y = ((int)(circuit.placement[_pins[i].cellIdx].yCoord - min_y()))/tile_edge;
	cout<<"-x= "<<_x<<endl;	
	  for(int k = 0; k < tiles_num; k++) {
		  if( (tiles[k].x == _x) && (tiles[k].y == _y) ) { //pin belongs to this tile
			  cout<<"add function called\n";
			  tiles[k].add_pin(_pins[i]);
			  break;
		  }
	  }

	}
cout<<"pushing finished\n";
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

#endif
