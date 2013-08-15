//
//  slicer.h
//  dlpslice
//
//  Created by t00ts on 09/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#ifndef dlpslice_slicer_h
#define dlpslice_slicer_h

#include <string>
#include <map>
#include "part.h"
#include "utils.h"

class Slicer {

public:
  Slicer ();
  Slicer (Part *part, float layerHeight, std::string outputDir);
  ~Slicer ();
  
  void setPart (Part *part);
  void setLayerHeight (float h);
  void setOutputDir (std::string dir);
  
  void slice2 ();

private:
  Part *part;
  float layerHeight;
  std::string outputDir;

};

#endif
