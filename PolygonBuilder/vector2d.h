//
// vector2d.h
// C++ class definition of vector2d
// VIST 271 M. Ringham  3/30/2011
//
//#ifndef vector2d_h
//#define vector2d_h

#include <iostream>
#include <cmath>
using namespace std;


class vector2d {
private:
    float x, y;                         // x,y coordinate for vector

public:
    float getx () { return x; }         // get the x coordinate
    float gety () { return y; }         // get the y coordinate
    void setx ( float x1 ) { x = x1; }  // set the x coordinate
    void sety ( float y1 ) { y = y1; }  // set the y coordinate
    float magnitude();        //find the magnitude of a vector
    vector2d unitVector();      //find unit vector
  
    //addition operator
    friend vector2d operator+( vector2d, vector2d );  // operator overload to add 2 vectors
    
    //subtraction operator
    friend vector2d operator-( vector2d, vector2d ); // operator overload to subtract 2 vectors
    
    //multiplication by scalar
    friend vector2d operator*( float, vector2d ); //(scale, vector) operator overload to scale vector
    friend vector2d operator* (vector2d, float ); //(scale, vector) operator overload to scale vector
    
    //inner (dot) product
    friend float operator*( vector2d, vector2d ); //operator overload for inner dot product
};
//#endif /* vector2d_h */

