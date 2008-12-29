#ifndef _FASTACCESS_H_
#define _FASTACCESS_H_

#include "data_structures.h"


inline Row& GetRow(Circuit &c, int index) {return c.rows[index];}

//net group
inline Net& GetNet(Circuit &c, int netindex) {return c.nets[netindex];}
inline Pin& GetNetPin(Net& net, int pinindex) {return net.arrPins[pinindex];}
inline Pin& GetNetPin(Circuit &c, int netindex, int pinindex) {return GetNetPin(c.nets[netindex], pinindex);}
inline Pin* GetNetPins(Net& net) {return net.arrPins;}
inline int GetNodeIndex(Net& net, int pinindex) {return net.arrPins[pinindex].cellIdx;}
inline int GetNodeIndex(Circuit &c, int netindex, int pinindex) {return GetNodeIndex(c.nets[netindex], pinindex);}
inline Node& GetNode(Circuit &c, Net& net, int pinindex)
{
    int cellind = GetNodeIndex(net,pinindex);
    return (cellind < c.nNodes) ? c.nodes[cellind] : c.terminals[cellind - c.nNodes];
}
inline Node& GetNode(Circuit &c, int netindex, int pinindex) {return GetNode(c,GetNet(c,netindex),pinindex);}
inline Place& GetPlace(Circuit &c, Net& net, int pinindex) {return c.placement[GetNodeIndex(net,pinindex)];}
inline Place& GetPlace(Circuit &c, int netindex, int pinindex) {return c.placement[GetNodeIndex(c,netindex,pinindex)];}

//pin group
inline Node& GetNode(Circuit &c, Pin& pin) {return (pin.cellIdx < c.nNodes) ? c.nodes[pin.cellIdx] : c.terminals[pin.cellIdx - c.nNodes];}
inline Place& GetPlace(Circuit &c, Pin& pin) {return c.placement[pin.cellIdx];}

//node group
inline Node& GetNode(Circuit &c, int nodeindex) {return (nodeindex < c.nNodes) ? c.nodes[nodeindex] : c.terminals[nodeindex - c.nNodes];}
inline Node& GetNodeFast(Circuit &c, int nodeindex) {return c.nodes[nodeindex];}
inline Node& GetTerminal(Circuit &c, int termindex) {return c.terminals[termindex-c.nNodes];}
inline Node& GetTerminalFast(Circuit &c, int termindex) {return c.terminals[termindex];}
inline vector<int> GetNetsIndexes(Circuit &c, int nodeindex) {return c.tableOfConnections[nodeindex];}
inline int GetNetIndex(Circuit &c, int nodeindex, int pinindex) {return c.tableOfConnections[nodeindex][pinindex];}
inline Net& GetNet(Circuit &c, int nodeindex, int pinindex) {return c.nets[GetNetIndex(c,nodeindex,pinindex)];}

//inverse
//inline int GetNetIndex(Circuit &c, Net& net) {return &net - c.nets;}
//inline int GetNodeIndex(Circuit &c, Node& node) {return &node - c.nodes;}
//inline int GetNodeIndex(Circuit &c, Place& place) {return &place - c.placement;}

#endif// _FASTACCESS_H_