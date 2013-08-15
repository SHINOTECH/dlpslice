//
//  stlreader.cpp
//  dlpslice
//
//  Created by t00ts on 05/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#include "stlreader.h"
#include <fstream>
#include <iostream>
#include "part.h"
#include "utils.h"

#define STL_HEADER_SIZE 80
#define STL_N_FACETS_SIZE 4
#define STL_NORMAL_SIZE 4
#define STL_VERTEX_SIZE 4
#define STL_FACET_NORMAL_SIZE STL_NORMAL_SIZE*3
#define STL_FACET_VERTEX_SIZE STL_VERTEX_SIZE*3
#define STL_FACET_ATTCNT_SIZE 2

STLReader::STLReader () {

}

STLReader::~STLReader() {

}

int STLReader::read (const char* filename, Part *part) {

  using namespace std;
  
  long fsize;
  ifstream fh (filename, ios::in | ios::binary | ios::ate);

  if (fh.is_open()) {
    
    // File size
    fsize = fh.tellg();

    cerr << "File opened successfully. Total size: " << fsize << " bytes" << endl;
    
    // Allocate memory containers
    char *mfacets = new char[STL_N_FACETS_SIZE];

    // Number of facets
    fh.seekg (STL_HEADER_SIZE, ios::beg);
    fh.read (mfacets, sizeof(mfacets));
    this->nfacets = int(*mfacets);
    delete[] mfacets;

    // Read facets
    for (int i = 0; i < this->nfacets; ++i) {
      if (fh.good()) {
        
        // Move get pointer to offset corresponding to i-th facet
        fh.seekg (STL_HEADER_SIZE + STL_N_FACETS_SIZE + (i * (STL_FACET_NORMAL_SIZE + STL_FACET_VERTEX_SIZE*3 + STL_FACET_ATTCNT_SIZE)) + STL_FACET_NORMAL_SIZE, ios::beg);

        // Read vertices
        std::bitset<32> vbx1, vbx2, vbx3, vby1, vby2, vby3, vbz1, vbz2, vbz3;
        
        // Vertex 1
        fh.read ((char *) &vbx1, STL_VERTEX_SIZE);
        fh.read ((char *) &vby1, STL_VERTEX_SIZE);
        fh.read ((char *) &vbz1, STL_VERTEX_SIZE);

        // Vertex 2
        fh.read ((char *) &vbx2, STL_VERTEX_SIZE);
        fh.read ((char *) &vby2, STL_VERTEX_SIZE);
        fh.read ((char *) &vbz2, STL_VERTEX_SIZE);
        
        // Vertex 3
        fh.read ((char *) &vbx3, STL_VERTEX_SIZE);
        fh.read ((char *) &vby3, STL_VERTEX_SIZE);
        fh.read ((char *) &vbz3, STL_VERTEX_SIZE);
        
        // Binary to decimal conversion
        Vertex v1, v2, v3;
        v1 = bin2Vertex (vbx1, vby1, vbz1);
        v2 = bin2Vertex (vbx2, vby2, vbz2);
        v3 = bin2Vertex (vbx3, vby3, vbz3);

        // Store facet
        Facet f (v1, v2, v3);
        part->addFacet(f);

      }
      else {
        return -2;    // [ ** Error: Corrupt file ]
      }
    }

    fh.close();    
    return 1;

  }
  else {
    return -1;    // [ ** Error: Couldn't open file ]
  }
  
}


Vertex STLReader::bin2Vertex (std::bitset<32> bx, std::bitset<32> by, std::bitset<32> bz) const {

  return Vertex (GetFloat32(bx.to_string()), GetFloat32(by.to_string()), GetFloat32(bz.to_string()));

}