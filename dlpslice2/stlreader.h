//
//  stlreader.h
//  dlpslice
//
//  Created by t00ts on 05/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#ifndef dlpslice_stlreader_h
#define dlpslice_stlreader_h

#include <bitset>
#include "utils.h"

class Part;

class STLReader {

public:
  STLReader ();
  ~STLReader ();
  int read (const char* filename, Part *part);
  
private:
  unsigned int nfacets;

  Vertex bin2Vertex (std::bitset<32> bx, std::bitset<32> by, std::bitset<32> bz) const;

};

#endif
