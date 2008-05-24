
//this file stores information about tiles

#ifndef TILE_H
#define TILE_H

#include "data_structures.h"
#include "RouteTrees.h"

#define tile_edge 4000

void buble_sort(Pin *_pins, int num, bool _xcoord, const Circuit &circuit) {
	for(int i = 0; i < num; i++)
		for(int j = num - 1; j > i; j--) {
			if( _xcoord ) {
				double xj = circuit.placement[_pins[j].cellIdx].xCoord + _pins[j].xOffset;
				double xj1 = circuit.placement[_pins[j - 1].cellIdx].xCoord + _pins[j - 1].xOffset;
				
				if( xj < xj1 ) {
					Pin temp_pin = _pins[j];
					_pins[j] = _pins[j - 1];
					_pins[j - 1] = temp_pin;
				}
			}

			else {
				double yj = circuit.placement[_pins[j].cellIdx].yCoord + _pins[j].yOffset;
				double yj1 = circuit.placement[_pins[j - 1].cellIdx].yCoord + _pins[j - 1].yOffset;
				
				if( yj < yj1 ) {
					Pin temp_pin = _pins[j];
					_pins[j] = _pins[j - 1];
					_pins[j - 1] = temp_pin;
				}
			}
		}
}

struct Tile {
	int npit;    //number pins in tile
	int cur_num; //current number of pins - it's needed in add_pin()
	Pin *pins_it; //array of pins in this tile
	StNodeEx** p_nodes; // array of pointers to nodes in A-tree
	
	int x; //tile's coordinates
	int y;
	StNodeEx *center; //tile's center

	int nearest_t; //top nearest tile
	int nearest_l; //left nearest tile
	int nearest_d; //down nearest tile
	int nearest_r; //right nearest tile

//------------------------methods----------------------------------------------
	Tile();
	Tile(int _x, int _y);
	~Tile();

	void set_x(int _x) { x = _x; }
	void set_y(int _y) { y = _y; }
	void set_center_xy() {center->x = tile_edge/2 + x * tile_edge; center->y = tile_edge/2 + y * tile_edge; }
	void inc_npit() { npit++; }
	void add_pin(Pin _pin);
	int connect_center_with_pins(const Circuit& circuit);
	void find_node_for_pin(Pin &_pin);
};

Tile::Tile() {
	npit = cur_num = 0;
	pins_it = NULL;
	p_nodes = NULL;
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
	p_nodes = NULL;
	nearest_t = nearest_l = nearest_d = nearest_r = -1;
	center = CreateNodeEx();
	center->left = center->right = 0;
	center->type = NODE_TYPE_INTERNAL;
	center->origin = NULL;
	center->x = tile_edge/2 + x * tile_edge;
	center->y = tile_edge/2 + y * tile_edge;
}

bool is_equal(const Pin &p1, const Pin &p2) {
	if( (p1.cellIdx == p2.cellIdx) && (p1.xOffset == p2.xOffset) && (p1.yOffset == p2.yOffset) )
		return true;
	else
		return false;
}

bool is_equal(const Pin *p1, const Pin &p2) {
	if( (p1->cellIdx == p2.cellIdx) && (p1->xOffset == p2.xOffset) && (p1->yOffset == p2.yOffset) )
		return true;
	else
		return false;
}

void Tile::add_pin(Pin _pin) {
	//cout<<"tile add started\n";
	if(cur_num != 0) {
		for(int i = 0; i < cur_num; i++) {
			if( is_equal(_pin, pins_it[i]) )
				return;
		}

		pins_it[cur_num] = _pin;
		cur_num++;
	}

	else {
		pins_it = new Pin [npit];
		pins_it[0] = _pin;
		cur_num++;
		cout<<"pin_it created; cur pins num = "<<cur_num<<" "<<npit<<"; x,y: "<<x<<y<<endl;
		//system("PAUSE");
	}

	cout<<"Tile add finished: cur pins num= "<<cur_num<<endl;
}

Tile::~Tile() {
	//cout<<"tile's destructor started\n";
	if( npit ) {
		delete [] pins_it; 
		delete [] p_nodes;
	}
	//cout<<"tile's destructor finished\n";
}

int get_pin_type(Pin *_pin) {
	switch (_pin->chtype) {
		case 'I': return NODE_TYPE_INPUT; break;
		case 'O': return NODE_TYPE_OUTPUT; break;
		default : return NODE_TYPE_INTERNAL;
	}
}

int Tile::connect_center_with_pins(const Circuit &circuit) {
	cout<<npit<<" started connecting\n";
	if( npit == 0) 
		return 1;

	if(pins_it == NULL)
		cout<<"NULL\n"<<npit<<endl;

	cout<<pins_it[0].chtype<<endl;
	p_nodes = new StNodeEx* [npit];
	int nodes_counter = 0;

	Pin *upper_pins = new Pin [npit]; //pins above tile's center
	int nupper_pins = 0;                //their number
	Pin *down_pins = new Pin [npit]; //pins below tile's center
	int ndown_pins = 0;                 //their number
	set_center_xy();

	for(int i = 0; i < npit; i++) {
		int j = pins_it[i].cellIdx;

		if(circuit.placement[j].yCoord + pins_it[i].yOffset> center->y) //dividing pins:
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
		buble_sort(upper_pins, nupper_pins, false, circuit);

		for(int i = 0; i < nupper_pins; ) {
			double _y = circuit.placement[upper_pins[i].cellIdx].yCoord + upper_pins[i].yOffset;
			int j = i;
			StNodeEx *pl = CreateNodeEx();
			pl->type = NODE_TYPE_INTERNAL;
			pl->x = p->x;
			pl->y = _y;
			pl->origin = NULL;
			p->left = pl;
			p = pl;

			while( ( j < nupper_pins) && ( (circuit.placement[upper_pins[j].cellIdx].yCoord + upper_pins[j].yOffset) == _y) ) {
				if(circuit.placement[upper_pins[j].cellIdx].xCoord == p->x) { //pin is on this line
					Pin _pin = upper_pins[j];
                    //p->origin = &_pin;
					p->origin = new Pin;
					p->origin->cellIdx = _pin.cellIdx;
					p->origin->chtype = _pin.chtype;
					p->origin->xOffset = _pin.xOffset;
					p->origin->yOffset = _pin.yOffset;
					p->type = get_pin_type( (p->origin) ); //----------------------------type
					p_nodes[nodes_counter++] = p;
				}
				
				else {
					StNodeEx *pr = CreateNodeEx();
					Pin _pin = upper_pins[j];
                    //pr->origin = &_pin;
					pr->origin = new Pin;
					pr->origin->cellIdx = _pin.cellIdx;
					pr->origin->chtype = _pin.chtype;
					pr->origin->xOffset = _pin.xOffset;
					pr->origin->yOffset = _pin.yOffset;
					pr->type = get_pin_type( (pr->origin) );//------------------------------type
					pr->y = pl->y;
					pr->x = circuit.placement[upper_pins[j].cellIdx].xCoord + upper_pins[j].xOffset;
					pr->right = pr->left = NULL;
					p_nodes[nodes_counter++] = pr;
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

			if( (circuit.placement[j].xCoord + down_pins[i].xOffset) < center->x)
				left_down_pins[ldp++] = down_pins[i];

			if( (circuit.placement[j].xCoord + down_pins[i].xOffset) > center->x)
				right_down_pins[rdp++] = down_pins[i];

			if( (circuit.placement[j].xCoord + down_pins[i].xOffset) == center->x)
				central_down_pins[cdp++] = down_pins[i];
		}

		//delete [] down_pins; down_pins = NULL;
		
		//---connecting center with central down pins...
		if( cdp ) {
			buble_sort(central_down_pins, cdp, false, circuit);
			StNodeEx *p = CreateNodeEx();
			p->origin = NULL;
			p->y = center->y;
			p->x = center->x + tile_edge/4;
			p->right = p->left = NULL;
			p->type = NODE_TYPE_INTERNAL;

			StNodeEx *pv = p;
			
			for(int i = 0; i < cdp; i++){
				int j = central_down_pins[i].cellIdx;

				if( (circuit.placement[j].yCoord + central_down_pins[i].yOffset) == center->y) {
					Pin _pin = central_down_pins[i];
                    //center->origin = &_pin;
					center->origin = new Pin;
					center->origin->cellIdx = _pin.cellIdx;
					center->origin->chtype = _pin.chtype;
					center->origin->xOffset = _pin.xOffset;
					center->origin->yOffset = _pin.yOffset;
					center->type = get_pin_type( (center->origin) ); //-------------------------------type
					p_nodes[nodes_counter++] = center;
				}

				else {
					StNodeEx *pr = CreateNodeEx();
					pr->origin = NULL;
					pr->y = circuit.placement[j].yCoord + central_down_pins[i].yOffset;
					pr->x = pv->x;
					pr->right = pr->left = NULL;
					pr->type = NODE_TYPE_INTERNAL;
					pv->left = pr;
					pv = pr;

					StNodeEx *prr = CreateNodeEx();
					Pin _pin = central_down_pins[i];
                    //prr->origin = &_pin;
					prr->origin = new Pin;
					prr->origin->cellIdx = _pin.cellIdx;
					prr->origin->chtype = _pin.chtype;
					prr->origin->xOffset = _pin.xOffset;
					prr->origin->yOffset = _pin.yOffset;
					prr->x = circuit.placement[j].xCoord + central_down_pins[i].xOffset;
					prr->y = pr->y;
					prr->right = pr->left = NULL;
					prr->type = get_pin_type( (prr->origin) );//----------------------------type
					p_nodes[nodes_counter++] = prr;
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
			cout<<"ldp\n";
			StNodeEx *p = CreateNodeEx();
			p->right = p->left = NULL;
			p->type = NODE_TYPE_INTERNAL;
			p->x = center->x;
			p->y = center->y - tile_edge/4;
			p->origin = NULL;

			p->left = center->left;
			center->left = p;

			//TODO: sort letf_down_pins by x
			buble_sort(left_down_pins, ldp, true, circuit);

			for(int i = 0; i < ldp; ) {
				int j = i;
				double _x = circuit.placement[left_down_pins[i].cellIdx].xCoord + left_down_pins[i].xOffset;
				
				StNodeEx *pr = CreateNodeEx();
				pr->origin = NULL;
				pr->left = pr->right = NULL;
				pr->type = NODE_TYPE_INTERNAL;
				pr->x = _x;
				pr->y = p->y;
				p->right = pr;
				p = pr;

				while( (j < ldp) && ( (circuit.placement[left_down_pins[j].cellIdx].xCoord + left_down_pins[j].xOffset) == _x) ) {
					if( (circuit.placement[left_down_pins[j].cellIdx].yCoord + left_down_pins[j].yOffset) == p->y) {
						Pin _pin = left_down_pins[j];
                        //p->origin = &_pin;
						p->origin = new Pin;
						p->origin->cellIdx = _pin.cellIdx;
						p->origin->chtype = _pin.chtype;
						p->origin->xOffset = _pin.xOffset;
						p->origin->yOffset = _pin.yOffset;
						p->type = get_pin_type( (p->origin) ); //-------------------------------type
						p_nodes[nodes_counter++] = p;
					}

					else {
						StNodeEx *pl = CreateNodeEx();
						Pin _pin = left_down_pins[j];
                        //pl->origin = &_pin;
						pl->origin = new Pin;
						pl->origin->cellIdx = _pin.cellIdx;
						pl->origin->chtype = _pin.chtype;
						pl->origin->xOffset = _pin.xOffset;
						pl->origin->yOffset = _pin.yOffset;
						pl->type = get_pin_type( (pl->origin) ); //-------------------------------type
						pl->x = pr->x;
						pl->y = circuit.placement[left_down_pins[j].cellIdx].yCoord + left_down_pins[j].yOffset;
						pl->left = pl->right = NULL;
						p_nodes[nodes_counter++] = pl;
						pr->left = pl;
						pr = pl;
					}

					j++;
				}

				i = j;
			}
		}

		//---connecting center with right down pins...
		//cout<<"started rdp!\n";
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
			buble_sort(right_down_pins, rdp, true, circuit);

			for( int i = 0; i < rdp; ) {
				double _x = circuit.placement[right_down_pins[i].cellIdx].xCoord + right_down_pins[i].xOffset;
				int j = i;

				StNodeEx *pr = CreateNodeEx();
				pr->origin = NULL;
				pr->type = NODE_TYPE_INTERNAL;
				pr->left = pr->right = NULL;
				pr->x = _x;
				pr->y = p->y;
				p->right = pr;
				p = pr;

				while( (j < rdp) && ( (circuit.placement[right_down_pins[i].cellIdx].xCoord + right_down_pins[i].xOffset ) == _x) ) {
					if( (circuit.placement[right_down_pins[i].cellIdx].yCoord + right_down_pins[i].yOffset) == p->y) {
						Pin _pin = right_down_pins[j];
                        //p->origin = &_pin;
						p->origin = new Pin;
						p->origin->cellIdx = _pin.cellIdx;
						p->origin->chtype = _pin.chtype;
						p->origin->xOffset = _pin.xOffset;
						p->origin->yOffset = _pin.yOffset;
						p->type = get_pin_type( (p->origin) ); //-----------------------------type
						p_nodes[nodes_counter++] = p;
					}

					else {
						StNodeEx *pl = CreateNodeEx();
						pl->left = pl->right = NULL;
						cout<<"before *\n";
						Pin _pin = right_down_pins[j];
                        //pl->origin = &_pin;
						pl->origin = new Pin;
						pl->origin->cellIdx = _pin.cellIdx;
						pl->origin->chtype = _pin.chtype;
						pl->origin->xOffset = _pin.xOffset;
						pl->origin->yOffset = _pin.yOffset;
						pl->type = get_pin_type( (pl->origin) ); //-----------------------------type
						pl->y = circuit.placement[right_down_pins[j].cellIdx].yCoord + right_down_pins[j].yOffset;
						pl->x = pr->x;
						p_nodes[nodes_counter++] = pl;
						pr->left = pl;
						pr = pl;
					}

					j++;
				}

				i = j;
			}
		}

		delete [] left_down_pins;
		delete [] right_down_pins;
		delete [] central_down_pins;
	}

	delete [] upper_pins;
	return 0;
}

void Tile::find_node_for_pin(Pin &_pin) {
	//cout<<"find node started\n";
	//cout<<;
	int i = 0;
	
	while( i < npit ) {
		//cout<<"ind: "<<p_nodes[i]->origin->cellIdx<<endl;
		if( is_equal(p_nodes[i]->origin, _pin) )
			break;

		i++;
	}

	_pin.routeInfo = p_nodes[i];
	//cout<<"find node: "<<_pin.routeInfo->x<<endl;
}

#endif