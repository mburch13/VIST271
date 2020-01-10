#ifdef __APPLE__
#  include <OpenGL/gl.h>  // definitions for GL graphics routines
#  include <OpenGL/glu.h> // definitions for GL input device handling
#  include <GLUT/glut.h>  // deginitions for the GLUT utility toolkit
#else
#  include <GL/gl.h>      // definitions for GL graphics routines
#  include <GL/glu.h>     // definitions for GL input device handling
#  include <GL/glut.h>    // deginitions for the GLUT utility toolkit
#endif

//#ifndef polygon_h
//#define polygon_h

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "vector2d.h"

using namespace std;

#define WIDTH       900          // window dimensions
#define HEIGHT      800
#define RADIUS      10
#define PI 3.1415926536     // mathematical pi, not apple pie

//int x=0, y=0;

struct Point {
	float x, y;
	struct Point *next;
};

class Polygon {
	private:
		Point *head, *tail, *selected;
		bool closed;
		
	public:
		Polygon();                              // constructor for Polygon
		
		void append(int x, int y);              // append new point
		void updatePoint(int x, int y);         // update the selected point
		bool hitPoint(int x, int y);            // find selected point 
		
		bool isSelected();                      // return true if selected point
		void unSelect() { selected=NULL; }      // unselect the point
		
		bool isClosed() { return closed; }      // return state of polygon
		void close() { closed = true; }         // close the polygon
        void circle(int cx, int cy, int r);
		void draw();                            // draw the polygon
    
        bool deletePoint(int x, int y);
        bool hitLine(float x, float y);
};
//#endif
