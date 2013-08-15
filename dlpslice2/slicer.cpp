//
//  slicer.cpp
//  dlpslice
//
//  Created by t00ts on 09/08/13.
//  Copyright (c) 2013 DoubleA Technology Ltd. All rights reserved.
//

#include "EasyBMP/EasyBMP.h"
#include "slicer.h"
#include "utils.h"
#include "part.h"
#include <map>
#include <string>
#include <sys/stat.h>

#include <iostream>
using namespace std;


Slicer::Slicer () {}

Slicer::Slicer (Part *part, float layerHeight, std::string outputDir) : part(part), layerHeight(layerHeight), outputDir(outputDir) {}

void Slicer::setPart (Part *part) {
  this->part = part;
}

void Slicer::setLayerHeight (float h) {
  layerHeight = h;
}

void Slicer::setOutputDir (std::string dir) {
  outputDir = dir;
}


void Slicer::slice2 () {
  
  // Allocate structure for slice cut points
  std::map<float, std::list<PlotPoint2D> > sliceData;
  
  // Define initial cut plane
  Plane cutPlane (Point (0, 0, part->getDimensions().z - layerHeight), 
                  Point (0, 0, 1));
  
  // Slice all layers
  while (cutPlane.p.z >= 0) {

    std::map <Edge, PlotPoint> cutPoints;

    // ---------------------------------------------------------
    // Find all points of intersection for current slicing layer
    // ---------------------------------------------------------
    
    // Find all edges that cut on current plane
    std::map<Edge, Node2*>::iterator edge;
    //edge = part->getPart();
    while (edge->second->zmax >= cutPlane.p.z) {
      
      Point cutPoint;

      cerr << "Calculating cut point: z = " << cutPlane.p.z << endl;
      cerr << "(" << edge->first.first.x << ", " << edge->first.first.y << edge->first.first.z << ") -> (" << edge->first.second.x << ", " << edge->first.second.y << ", " << edge->first.second.z << ")" << endl;
      
      // Calculate point of intersection
      float t = (cutPlane.p.z - edge->first.second.z) / edge->second->dirVector.z;
      cutPoint.x = edge->first.first.x + edge->second->dirVector.x * t;
      cutPoint.y = edge->first.first.y + edge->second->dirVector.y * t;
      cutPoint.z = edge->first.first.z + edge->second->dirVector.z * t;
      
      cerr << "cutpoint = (" << cutPoint.x << ", " << cutPoint.y << ", " << cutPoint.z << ")" << endl;
      
      // Save point of intersection for current edge
      // TODO: Aqui nos podemos ahorrar guardar las edgeConnections ahora con el map
      //       ya que tenemos acceso inmediato a partir del edge
      cutPoints.insert (std::pair<Edge, PlotPoint> (edge->first, PlotPoint (cutPoint, edge->second->getEdgeConnections())));

      // Next edge
      ++edge;

    }
    
    // --------------------------------------------------------------
    // Join points together and save them for the corresponding layer
    // --------------------------------------------------------------
    
    // Iterate through points found
    std::map <Edge, PlotPoint>::iterator it;
    for (it = cutPoints.begin(); it != cutPoints.end(); ++it) {
      
      // Find 2D connections for current point
      std::list<Edge>::iterator possibleConnection;
      for (possibleConnection = it->second.edgeConnections.begin(); possibleConnection != it->second.edgeConnections.end(); ++possibleConnection) {
        if (cutPoints.count (*possibleConnection)) {
          it->second.addPointConnection (cutPoints.at(*possibleConnection).p) or cerr << "Se intenta añadir mas de dos puntos de conexion 2D..." << endl;
        }
      }
      
    }
    
    // Save points with their corresponding connections
    std::list<PlotPoint2D> layerPoints;
    for (it = cutPoints.begin(); it != cutPoints.end(); ++it) {
      PlotPoint2D p (it->second.p);
      p.connections[0] = it->second.pointConnections[0];
      p.connections[1] = it->second.pointConnections[1];    
      layerPoints.push_back (p);
    }
    
    sliceData.insert (std::pair<float, std::list<PlotPoint2D> > (cutPlane.p.z, layerPoints));

    // Decrease layer
    cutPlane.p.z -= layerHeight;
    
  }
  
  // --------------------------------------------------------------
  // Create image for each layer and save
  // --------------------------------------------------------------
  
  // Check output folder exists and we have write permissions
  if (mkdir("./slices/", 0777)) {
    perror ("Error creating slice directory");
    //  return;
  }
  
  // Get pixel <-> mm equivalents
  Dimensions dim = part->getDimensions();
  
  cerr << "Slice data: " << sliceData.size() << endl;
  
  
  // Iterate through all sliced layers
  int layer = 0;
  std::map<float, std::list<PlotPoint2D> >::iterator it;
  for (it = sliceData.begin(); it != sliceData.end(); ++it) {
    
    // Create image
    BMP image;
    RGBApixel blackPixel (0, 0, 0, 0);
    image.SetSize (IMAGE_SIZE_W, IMAGE_SIZE_H);
    image.SetBitDepth (1);
    
    // Iterate through all points for current layer
    std::list<PlotPoint2D>::iterator itl;
    for (itl = it->second.begin(); itl != it->second.end(); ++itl) {
      cerr << "Painting (" << itl->p.x << ", " << itl->p.y << ")" << endl;
      image.SetPixel(itl->p.x, itl->p.y, blackPixel);
      image.SetPixel(itl->connections[0].x, itl->connections[0].y, blackPixel);
      image.SetPixel(itl->connections[1].x, itl->connections[1].y, blackPixel);
    } // /point
    
    char filename[50];
    sprintf (filename, "./slices/layer_%d.bmp", layer);
    cout << "Writing image: " << filename << endl;
    image.WriteToFile (filename);
    ++layer;
    
  } // /layer
  
  
  // ------------------- CHECK RESULTS ---------------------
  layer = 0;
  it = std::map<float, std::list<PlotPoint2D> >::iterator ();
  for (it = sliceData.begin(); it != sliceData.end(); ++it) {
    
    char filename[50];
    sprintf (filename, "./slices/layer_%d.bmp", layer);
    BMP imageCheck;
    imageCheck.ReadFromFile(filename);
    
    cerr << "------- Pixel test | Layer " << layer << " ---------" << endl;
    std::list<PlotPoint2D>::iterator itl;
    for (itl = it->second.begin(); itl != it->second.end(); ++itl) {
      cerr << "Pixel (" << itl->p.x << ", " << itl->p.y << ") -> ";
      RGBApixel px;
      px = imageCheck.GetPixel(itl->p.x, itl->p.y);
      if (px.Red == 0 && px.Green == 0 && px.Blue == 0) cerr << "PASSED" << endl;
      else cerr << "FAILED" << endl;
    }
    ++layer;
    
  }
  
}
