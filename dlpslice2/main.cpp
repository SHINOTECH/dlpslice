//
//  main.cpp
//  dlpslice
//
//  Created by t00ts on 05/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#include <iostream>
#include "stlreader.h"
#include "slicer.h"
#include "part.h"
using namespace std;

int main (int argc, const char * argv[])
{
  
  if (argc != 3) {
    std::cout << "Usage: ./dlpslice filename.stl zLayerHeight" << std::endl;
    return 0;
  }
  
  // Read STL file
  STLReader *stl = new STLReader ();
  Part *part = new Part ();

  stl->read (argv[1], part);

  cout << "Total facets: " << part->get_nFacets() << endl;
  cout << "Total edges: " << part->get_nEdges() << endl;

  part->computeDimensions();
  Dimensions dim = part->getDimensions();
  
  cout << "Part dimensions:" << endl;
  cout << "X: " << dim.x << endl;
  cout << "Y: " << dim.y << endl;
  cout << "Z: " << dim.z << endl;

  cout << endl << "Part nodes:" << endl;
  
  
  //part->printz();
  return 1;
  /*
  Slicer *slicer = new Slicer ();
  slicer->setLayerHeight (atof(argv[2]));
  slicer->setOutputDir("/Users/t00ts/Desktop/dlpout/");
  slicer->setPart (part);
  slicer->slice2 ();

  return 0;*/

}


/*
 
 ----------------------------- EDGE TEST --------------------------------
 
 
 Edge e1 = std::pair<Vertex, Vertex> (Vertex(0., 0., 0.1), Vertex(0.1, 0., 0.1));
 Edge e2 = std::pair<Vertex, Vertex> (Vertex(0.1, 0., 0.1), Vertex(0., 0., 0.1));
 Edge swapped = std::pair<Vertex, Vertex> (e1.second, e1.first);
 std::map<Edge, int> mapp;
 mapp.insert(std::pair<Edge, int> (e1, 1));
 mapp.insert(std::pair<Edge, int> (e2, 2));
 mapp.insert(std::pair<Edge, int> (swapped, 3));
 
 cout << "Test 1" << endl;
 if (mapp.find(e1) != mapp.end())
 cout << "PASSED WITH i = " << mapp.find(e1)->second << endl;
 else
 cout << "EDGE e1 NOT FOUND" << endl;
 
 cout << "Test 2" << endl;
 if (mapp.find(e2) != mapp.end())
 cout << "PASSED WITH i = " << mapp.find(e2)->second << endl;
 else
 cout << "EDGE e2 NOT FOUND" << endl;
 
 cout << "Test 3" << endl;
 if (mapp.find(swapped) != mapp.end())
 cout << "PASSED WITH i = " << mapp.find(swapped)->second << endl;
 else
 cout << "EDGE swapped NOT FOUND" << endl;
 
 return 1;
 */