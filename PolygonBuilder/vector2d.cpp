//
// vector2d.cpp
// C++ class implementation of vector2d
// VIST 271 McKenzie Burch  3/23/2011
// This program defines the member function and overload operators of a vector class

#include "vector2d.h"


//find the magnitude of a vector
//magnitude is the hypotenuse of the x and y components
float vector2d::magnitude()
{
    float x, y;

    x = this->getx();
    y = this->gety();

    return sqrt(x*x + y*y);
}

//find a unit vector that is parallel to a given vector
//unit vector is the vecotr divided by magnitude
vector2d vector2d::unitVector()
{
    float mag = this->magnitude();
    vector2d unit;
    
    unit.setx( this->getx() / mag );
    unit.sety( this->gety() / mag );
    
    return unit;
}

//overloaded addition operator to add 2 vecotrs together
vector2d operator+( vector2d v1, vector2d v2 )
{
    vector2d add;

    add.setx ( v1.x + v2.x );
    add.sety ( v1.y + v2.y );

    return add;
}

//overloaded subtraction operator to subtract 2 vecotrs together
vector2d operator-( vector2d v1, vector2d v2 )
{
    vector2d sub;
    
    sub.setx( v1.x - v2.x );
    sub.sety( v1.y - v2.y );
    
    return sub;
}

//overloaded multiply operator to scalar multily a vecotr
vector2d operator*( float s, vector2d v )
{
    vector2d scale;
    
    scale.setx( s * v.x );
    scale.sety( s * v.y );
    
    return scale;
}

//overloaded multiply operator to scalar multily a vecotr
vector2d operator*( vector2d v, float s )
{
    vector2d scale;
    
    scale.setx( v.x * s );
    scale.sety( v.y * s );
    
    return scale;
}

//overloaded multiply operator to multily 2 vectors using inner (dot) product
float operator*( vector2d v1, vector2d v2 )
{
    float dot;
    
    dot = (v1.x * v2.x) + (v1.y * v2.y);
    
    return dot;
}
