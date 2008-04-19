
//this file stores information about tiles

#ifndef TILE_H
#define TILE_H

#include "data_structures.h"
#include "RouteTrees.h"

#define tile_edge 20

struct Tile {
	int npit; //number pins in tile
	int cur_num; //current number of pins - it's needed in add_pin()
	int x; //tile's coordinates
	int y;
	Pin *pins_it; //pins in this tile
	int nearest_t; //top nearest tile
	int nearest_l; //left nearest tile
	int nearest_d; //down nearest tile
	int nearest_r; //right nearest tile
	StNodeEx *center; //tile's center

	Tile();
	Tile(int _x, int _y);
	~Tile();

	void set_x(int _x) { x = _x; }
	void set_y(int _y) { y = _y; }
	void set_center_xy() {center->x = tile_edge/2 + x * tile_edge; center->y = tile_edge/2 + y * tile_edge; }
	void inc_npit() { npit++; }
	void add_pin(Pin _pin);
	int connect_center_with_pins(const Circuit& circuit);
};

Tile::Tile() {
	npit = cur_num = 0;
	pins_it = NULL;
	x = y = 0;
	nearest_t = nearest_l = nearest_d = nearest_r = -1;
	center = CreateNodeEx();
	center->left = center->right = 0;
	center->type = NODE_TYPE_INTERNAL;
	center->origin = NULL;
	center->x = center->y = 0;
}

Tile::Tile(int _x, int _y): x(_x), y(_y) {
	npit = cur_num = 0; 
	pins_it = NULL;
	nearest_t = nearest_l = nearest_d = nearest_r = -1;
	center = CreateNodeEx();
	center->left = center->right = 0;
	center->type = NODE_TYPE_INTERNAL;
	center->origin = NULL;
	center->x = tile_edge/2 + x * tile_edge;
	center->y = tile_edge/2 + y * tile_edge;
}

bool is_egual(Pin p1, Pin p2) {
	if( (p1.cellIdx == p2.cellIdx) && (p1.xOffset == p2.xOffset) && (p1.yOffset == p2.yOffset) )
		return true;
	else
		return false;
}

void Tile::add_pin(Pin _pin) {
	if(cur_num != 0) {
		for(int i = 0; i < cur_num; i++) {
			if( is_egual(_pin, pins_it[i]) )
				return;
		}

		pins_it[cur_num] = _pin;
		cur_num++;
	}

	else {
		pins_it = new Pin [npit];
		pins_it[0] = _pin;
		cur_num++;
	}

	cout<<"Tile add finished\n";
}

Tile::~Tile() {
	cout<<"tile's destructor started\n";
	delete [] pins_it;
	cout<<"tile's destructor finished\n";
}

Tile **tiles;

int get_pin_type(Pin *_pin) {
	switch (_pin->chtype) {
		case 'I': return NODE_TYPE_INPUT; break;
		case 'O': return NODE_TYPE_OUTPUT; break;
		default : return NODE_TYPE_INTERNAL;
	}
}

int Tile::connect_center_with_pins(const Circuit &circuit) {
	if( npit == 0) 
		return 1;

	Pin *upper_pins = new Pin [npit]; //pins above tile's center
	int nupper_pins = 0;                //their number
	Pin *down_pins = new Pin [npit]; //pins below tile's center
	int ndown_pins = 0;                 //their number
	set_center_xy();

	for(int i = 0; i < npit; i++) {
		int j = pins_it[i].cellIdx;

		if(circuit.placement[j].yCoord > center->y) //dividing pins:
			upper_pins[nupper_pins++] = (pins_it[i]);             //pins above center and

		else
			down_pins[ndown_pins++] = (pins_it[i]);               //pins below center
	}
	
    //------connecting ceter with upper pins...
	if( nupper_pins ) {
		StNodeEx *p = CreateNodeEx();
		p->origin = NULL;
		p->x = center->x + tile_edge/8; //coordinates
		p->y = center->y;
		p->type = NODE_TYPE_INTERNAL;
		p->right = NULL;
		
		//if( center->right )
			p->right = center->right;
		
		center->right = p;
		//TODO: sort upper_pins by y coord

		for(int i = 0; i < nupper_pins; ) {
			double _y = circuit.placement[upper_pins[i].cellIdx].yCoord;
			int j = i;
			StNodeEx *pl = CreateNodeEx();
			pl->type = NODE_TYPE_INTERNAL;
			pl->x = p->x;
			pl->y = _y;
			pl->origin = NULL;
			p->left = pl;
			p = pl;

			while( ( j < nupper_pins) && (circuit.placement[upper_pins[j].cellIdx].yCoord == _y) ) {
				if(circuit.placement[upper_pins[j].cellIdx].xCoord == p->x) { //pin is on this line
					p->origin = &upper_pins[j];
					p->type = get_pin_type( (p->origin) ); //----------------------------type				
				}
				
				else {
					StNodeEx *pr = CreateNodeEx();
					pr->origin = &upper_pins[j];
					pr->type = get_pin_type( (pr->origin) );//------------------------------type
					pr->y = pl->y;
					pr->x = circuit.placement[upper_pins[j].cellIdx].xCoord;
					pr->right = pr->left = NULL;
					pl->right = pr;
					pl = pr;
				}

				j++;
			}

			i = j;
		}
	}

    //-----------connecting center with down pins...
	if(ndown_pins) {
		Pin *left_down_pins = new Pin [ndown_pins];
		int ldp = 0;
		Pin *right_down_pins = new Pin [ndown_pins];
		int rdp = 0;
		Pin *central_down_pins = new Pin [ndown_pins];
		int cdp = 0;

		for(int i = 0; i < ndown_pins; i++) {
			int j = down_pins[i].cellIdx;

			if(circuit.placement[j].xCoord < center->x)
				left_down_pins[ldp++] = down_pins[i];

			if(circuit.placement[j].xCoord > center->x)
				right_down_pins[rdp++] = down_pins[i];

			if(circuit.placement[j].xCoord == center->x)
				central_down_pins[cdp++] = down_pins[i];
		}

		delete [] down_pins; down_pins = NULL;
		
		//---connecting center with central down pins...
		if( cdp ) {
			StNodeEx *p = CreateNodeEx();
			p->origin = NULL;
			p->y = center->y;
			p->x = center->x + tile_edge/4;
			p->right = p->left = NULL;
			p->type = NODE_TYPE_INTERNAL;

			StNodeEx *pv = p;
			
			for(int i = 0; i < cdp; i++){
				int j = central_down_pins[i].cellIdx;

				if( circuit.placement[j].yCoord == center->y) {
					center->origin = &central_down_pins[i];
					center->type = get_pin_type( (center->origin) ); //-------------------------------type
				}

				else {
					StNodeEx *pr = CreateNodeEx();
					pr->origin = NULL;
					pr->y = circuit.placement[j].yCoord;
					pr->x = pv->x;
					pr->right = pr->left = NULL;
					pr->type = NODE_TYPE_INTERNAL;
					pv->left = pr;
					pv = pr;

					StNodeEx *prr = CreateNodeEx();
					prr->origin = &central_down_pins[i];
					prr->x = circuit.placement[j].xCoord;
					prr->y = pr->y;
					prr->right = pr->left = NULL;
					prr->type = get_pin_type( (prr->origin) );//----------------------------type
					pr->right = prr;
				}
			}
		
			if( p->left ) {
				p->right = center->right;
				center->right = p;
			}

			else {       //there are no pins
				delete p;
				p = NULL;
			}
		}

		//---connecting center with left down pins...
		if( ldp ) {
			StNodeEx *p = CreateNodeEx();
			p->right = p->left = NULL;
			p->type = NODE_TYPE_INTERNAL;
			p->x = center->x;
			p->y = center->y - tile_edge/4;
			p->origin = NULL;

			p->left = center->left;
			center->left = p;

			//TODO: sort letf_down_pins by x
			for(int i = 0; i < ldp; ) {
				int j = i;
				double _x = circuit.placement[left_down_pins[i].cellIdx].xCoord;
				
				StNodeEx *pr = CreateNodeEx();
				pr->origin = NULL;
				pr->left = pr->right = NULL;
				pr->type = NODE_TYPE_INTERNAL;
				pr->x = _x;
				pr->y = p->y;
				p->right = pr;
				p = pr;

				while( (j < ldp) && (circuit.placement[left_down_pins[j].cellIdx].xCoord == _x) ) {
					if( circuit.placement[left_down_pins[j].cellIdx].yCoord == p->y) {
						p->origin = &left_down_pins[j];
						p->type = get_pin_type( (p->origin) ); //-------------------------------type
					}

					else {
						StNodeEx *pl = CreateNodeEx();
						pl->origin = &left_down_pins[j];
						pl->type = get_pin_type( (pl->origin) ); //-------------------------------type
						pl->x = pr->x;
						pl->y = circuit.placement[left_down_pins[j].cellIdx].yCoord;
						pl->left = pl->right = NULL;
						pr->left = pl;
						pr = pl;
					}

					j++;
				}

				i = j;
			}
		}

		//---connecting center with right down pins...
		cout<<"started rdp!\n";
		if( rdp ) {
			StNodeEx *p = CreateNodeEx();
			p->origin = NULL;
			p->left = p->right = NULL;
			p->type = NODE_TYPE_INTERNAL;
			p->x = center->x;
			p->y = center->y - tile_edge/8;

			p->left = center->left;
			center->left = p;
			
			//TODO: sort right_down_pins by x
			for( int i = 0; i < rdp; ) {
				double _x = circuit.placement[right_down_pins[i].cellIdx].xCoord;
				int j = i;

				StNodeEx *pr = CreateNodeEx();
				pr->origin = NULL;
				pr->type = NODE_TYPE_INTERNAL;
				pr->left = pr->right = NULL;
				pr->x = _x;
				pr->y = p->y;
				p->right = pr;
				p = pr;

				while( (j < rdp) && (circuit.placement[right_down_pins[i].cellIdx].xCoord == _x) ) {
					if( circuit.placement[right_down_pins[i].cellIdx].yCoord == p->y) {
						p->origin = &right_down_pins[j];
						p->type = get_pin_type( (p->origin) ); //-----------------------------type
					}

					else {
						StNodeEx *pl = CreateNodeEx();
						pl->left = pl->right = NULL;
						cout<<"before *\n";
						pl->origin = &right_down_pins[j];
						pl->type = get_pin_type( (pl->origin) ); //-----------------------------type
						pl->y = circuit.placement[right_down_pins[j].cellIdx].yCoord;
						pl->x = pr->x;
						pr->left = pl;
						pr = pl;
						cout<<"finished else\n";
					}

					j++;
				}

				i = j;
			}
		}

		/*delete [] left_down_pins;
		delete [] right_down_pins;
		delete [] central_down_pins;*/
	}

	/*delete [] upper_pins;
	*/
	return 0;
}

#endif