
#include "clipper/clipper.hpp"
#include "poly2tri/poly2tri.h"
#include "clip2tri/clip2tri.h"

#include <iostream>
#include <cstdio>

using namespace std;
using namespace c2t;


// TODO actually fill out some test data...  this code compiles, but doesn't work!
int main() {
   vector<vector<Point> > inputPolygons;

   PolyTree solution;

   clip2tri clip2tri;
   clip2tri.mergePolysToPolyTree(inputPolygons, solution);


   // This will downscale the Clipper output and use poly2tri to triangulate
   vector<Point> outputTriangles;  // Every 3 points is a triangle
   Path boundingPolygon;
   clip2tri.triangulateComplex(outputTriangles, boundingPolygon, solution);

   // outputTriangles is what you want!
}
