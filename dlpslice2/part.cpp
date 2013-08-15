//
//  part2.cpp
//  dlpslice2
//
//  Created by t00ts on 15/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#include <iostream>
#include "part.h"
#include "utils.h"

Part::Part () {
  dim = Dimensions (0, 0, 0);
  nEdges = nFacets = 0;
}

Part::~Part () {}

void Part::addFacet (Facet f) {
  
  // Decompose facet into sides
  Edge AB (f.one, f.two);
  Edge BC (f.two, f.three);
  Edge CA (f.three, f.one);
  
  // Insert sides
  Node *nAB = insert (AB);
  Node *nBC = insert (BC);  
  Node *nCA = insert (CA);
  
  // Insert connections for each side
  nAB->addConnection (nBC);
  nAB->addConnection (nCA);
  
  nBC->addConnection (nAB);
  nBC->addConnection (nCA);
  
  nCA->addConnection (nAB);
  nCA->addConnection (nBC);
  
  ++nFacets;

  return;
  
}

Dimensions Part::getDimensions () const {
  return dim;
}

int Part::get_nFacets () const {
  return nFacets - 1;
}

int Part::get_nEdges () const {
  return nEdges;
}

void Part::computeDimensions () {

  float minx, maxx, miny, maxy, minz, maxz;
  bool firstIteration = true;
  int count = 0;
  
  // Get maximums and minimums on all 3 axes
  Node *act = firstNode;
  while (act != NULL) {
    
    if (firstIteration) {

      minx = fminf (act->edge.first.x, act->edge.second.x);
      maxx = fmaxf (act->edge.first.x, act->edge.second.x);
      
      miny = fminf (act->edge.first.y, act->edge.second.y);
      maxy = fmaxf (act->edge.first.y, act->edge.second.y);
      
      minz = fminf (act->edge.first.z, act->edge.second.z);
      maxz = fmaxf (act->edge.first.z, act->edge.second.z);
      
      firstIteration = false;
      
    }
    else {
      
      minx = fminf (fminf (act->edge.first.x, act->edge.second.x), minx);
      maxx = fmaxf (fmaxf (act->edge.first.x, act->edge.second.x), maxx);
      
      miny = fminf (fminf (act->edge.first.y, act->edge.second.y), miny);
      maxy = fmaxf (fmaxf (act->edge.first.y, act->edge.second.y), maxy);
      
      minz = fminf (fminf (act->edge.first.z, act->edge.second.z), minz);
      maxz = fmaxf (fmaxf (act->edge.first.z, act->edge.second.z), maxz);

    }
    
    act = act->next;
    ++count;
  }
  
  cerr << "DIMENSION TOTAL NODES: " << count << endl;
  
  // Compute sizes
  dim = Dimensions (maxx - minx, maxy - miny, maxz - minz);

}

Node* Part::insert (Edge edge) {
  
  Node *n = new Node (edge);
  bool in = false;
  Node *act = firstNode;
  int i = 0;
  
  do {
  
    // First element to enter list
    if (act == NULL) {
      firstNode = n;
      in = true;
      ++nEdges;
    }
    else {
    
      // Edge already exists
      if (edge_compare (act->edge, n->edge)) {
        delete (n);
        in = true;
        n = act;
      }

      if (!in && act->zmax >= n->zmax) {
      
        // Insert last position
        if (act->next == NULL) {

          act->next = n;
          n->prev = act;
          in = true;
          ++nEdges;

        }
        else ; // Keep moving down
        
      }
      
      // Found insert position
      else if (!in && act->zmax < n->zmax) {
      
        if (act->prev == NULL && act == firstNode) {
          // Insert as first node
          n->next = firstNode;
          firstNode->prev = n;
          firstNode = n;
        }
        else {
          // Insert n before act
          Node *prev = act->prev;
          prev->next = n;
          n->prev = prev;
          n->next = act;
          act->prev = n;
        }
        
        in = true;
        ++nEdges;
      }
    
    }
    
    // Continue if not yet inserted or found
    if (!in) {
      act = act->next;
      ++i;
    }

  } while (!in && act != NULL);

  return n;

}

void Part::printz () {
  Node *act = firstNode;
  int i = 0;
  while (act != NULL) {
    cerr << i++ << ": " << edge_print (act->edge) << " -> zmax = " << act->zmax << endl;   
    act = act->next;
  }
}