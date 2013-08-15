//
//  part2.h
//  dlpslice2
//
//  Created by t00ts on 15/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#ifndef dlpslice2_part2_h
#define dlpslice2_part2_h

#include "utils.h"

class Part {

public:
  
  Part ();
  ~Part ();
  
  Dimensions getDimensions () const;
  
  void addFacet (Facet f);
  void computeDimensions ();

  int get_nFacets () const;
  int get_nEdges () const;

  void printz ();

private:

  Node *firstNode;
  Dimensions dim;
  int nEdges, nFacets;

  Node* insert (Edge edge);

};

#endif
