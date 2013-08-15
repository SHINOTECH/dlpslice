//
//  utils.h
//  dlpslice
//
//  Created by t00ts on 05/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#ifndef dlpslice_utils_h
#define dlpslice_utils_h

#include <cmath>
#include <list>
#include <string>
#include <bitset>
#include <climits>
#include <string>
#include <sstream>


#define IMAGE_SIZE_W 1280 
#define IMAGE_SIZE_H 800
#define VAT_SIZE_MM_W 160.
#define VAT_SIZE_MM_H 100.

#include <iostream>
using namespace std;

struct Vertex {

  float x, y, z;
  Vertex () : x(0.), y(0.), z(0.) {}
  Vertex (float x, float y, float z) {
    this->x = (x > 1.e-30) ? x : 0;
    this->y = (y > 1.e-30) ? y : 0;
    this->z = (z > 1.e-30) ? z : 0;
  }
  
  Vertex (const Vertex &srcVertex) {
    x = srcVertex.x;
    y = srcVertex.y;
    z = srcVertex.z;
  }
  
  inline Vertex& operator= (const Vertex &srcVertex) {
    x = srcVertex.x;
    y = srcVertex.y;
    z = srcVertex.z;
    return *this;
  }

  inline bool operator== (const Vertex v) const {
    return x == v.x && y == v.y && z == v.z;
  }

  inline bool operator< (const Vertex v) const {
    return z < v.z;
  }

};


typedef Vertex Dimensions;
typedef Vertex Point;
typedef Vertex DirVector;
typedef std::pair<Vertex, Vertex> Edge;

inline bool edge_compare (const Edge &e1, const Edge &e2) {

  Edge swapped = make_pair (e1.second, e1.first);
  return (e1 == e2 || swapped == e2);

}

inline std::string edge_print (const Edge &e) {
  std::stringstream ss;
  ss << "(" << e.first.x << ", " << e.first.y << ", " << e.first.z << ") · (" << e.second.x << ", " << e.second.y << ", " << e.second.z << ")";
  return ss.str();
}

struct Facet {
  
  Vertex one, two, three;
  Facet (Vertex one, Vertex two, Vertex three) : one(one), two(two), three(three) {
  }

};


struct Plane {

  Point p, normal;
  Plane (Point p, Point n) : p(p), normal(n) {}

};


struct PlotPoint {
  
  Point p;
  std::list<Edge> edgeConnections;
  Point pointConnections[2];
  
  PlotPoint () { 
    nConnections = 0;
  };
  
  PlotPoint (Point p, std::list<Edge> edgeCon) : p(p), edgeConnections(edgeCon) { 
    nConnections = 0;
  }
  
  bool addPointConnection (Point p) {
    if (nConnections >= 2) return false;
    pointConnections[nConnections++] = p;
    return true;
  }

private:
  int nConnections;

};

struct PlotPoint2D {

  Point p;
  Point connections[2];

  PlotPoint2D ();
  PlotPoint2D (Point p) : p(p) {}

};


struct Node {

  Edge edge;
  DirVector dirVector;
  float zmax;
  std::list<Node*> connections;
  Node *next, *prev;
  
  Node () {
    zmax = 0;
    next = prev = NULL;
  };

  Node (Edge e) {
    
    // Define edge
    edge = e;

    // Define direction vector
    dirVector.x = e.second.x - e.first.x;
    dirVector.y = e.second.y - e.first.y;
    dirVector.z = e.second.z - e.first.z;
    
    // Define zmax
    zmax = fmaxf (e.first.z, e.second.z);
    
    // Just to be sure...
    next = prev = NULL;

  }
  
  bool addConnection (Node *c) {
    if (connections.size() < 4) {
      connections.push_back(c);
      return true;
    }
    else return false;
  }
  
  std::list<Edge> getEdgeConnections () {
    std::list<Edge> lcon;
    std::list<Node*>::iterator it;
    for (it = connections.begin(); it != connections.end(); ++it) {
      lcon.push_back ((*it)->edge);
    }
    return lcon;
  }

};


struct Node2 {
  
  DirVector dirVector;
  float zmax;
  std::list<Edge> connections;
  
  bool addConnection (Edge e) {
    if (connections.size() < 4) {
      connections.push_back(e);
      return true;
    }
    else return false;
  }
  
  std::list<Edge> getEdgeConnections () {
    return connections;
  }

};

/*struct NodeCompare {
    const bool operator () (const Edge n1, const Edge e2) const {
    return (fmaxf(n1.first.z, n1.second.z) >
            fmaxf(e2.first.z, e2.second.z));
  }
};*/


inline int Binary2Hex (std::string bin) {
  
  std::bitset<32> set (bin);
  int hex = (int)set.to_ulong();
  return hex;
  
}

inline float GetFloat32 (std::string bin) {
  
  int HexNumber = Binary2Hex (bin);  
  
  bool negative  = !!(HexNumber & 0x80000000);  
  int  exponent  =   (HexNumber & 0x7f800000) >> 23;      
  int  sign = negative ? -1 : 1;  

  // Subtract 127 from the exponent  
  exponent -= 127;
  
  // Convert the mantissa into decimal using the  
  // last 23 bits  
  int power = -1;  
  float total = 0.0;  
  for (int i = 0; i < 23; i++) {
    int c = bin[ i + 9 ] - '0';
    total += (float)c * (float)pow (2.0, power);
    power--;  
  }  
  total += 1.0;  
  
  float value = sign * (float) pow( 2.0, exponent ) * total;  
  
  return value;  
}  


inline float fmapf (float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif
