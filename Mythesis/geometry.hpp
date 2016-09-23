//
//  geometry.hpp
//  Mythesis
//
//  Created by t_wangju on 9/14/16.
//  Copyright © 2016 t_wangju. All rights reserved.
//



#ifndef geometry_hpp
#define geometry_hpp

//                             DO NOT USE ANY GLOBAL VALUE!


// name: substance set with CAPITAL,and single substance with ordinary,functions' name should as p_t_insert/p_T_research,action at end and with '_';
// new and delete should be used when construct Points, and always construct Segment/Triangle with Points' addr;
// for convenient,all Points which on a Triangle's edges are included by this Triangle,so there some Points are included by different Triangles,this should pay much attention;
// Class only have the data,and most of operators should be complete by programmer;
// all data struct can't use any addr that has a null value;

#include <iostream>
#include <cmath>
#include <set>
#include <map>
#include <vector>



//class Sample;
class Point;
class Segment;
class Vector;
class Curve;
class Triangle;

class Point{
public:
    //when true,try to delete link Segments and Triangles automatic(but not recommand);
    static bool AUTO_CONDES;
    
    int x,y;
    bool simple=true;//Is this a single Point not included by any triangle or segment?meaning all link set are empty;
    bool bound=false;//Is this Point movable,when true,can't move;
    
    std::set<Triangle*> linkT;
    std::set<Segment*> linkS;
    
    //always use new Point(x,y) here, don't construct a Point with no parametres;
    Point(const int x, const int y);
    //constriant this destruct function by check linkSet,and always "try" to delete linkS/T;
    ~Point();
    
    bool operator==(const Point &p) const;
    bool operator!=(const Point &p) const;
    
    Point* linkp(const Segment *s) const;
    Segment* links(const Point *p) const;
    double distance(const Point *p) const;
    double distance(const Segment *s) const;
    bool on(const Segment *s) const;
    bool in(std::vector<Point*> PV) const;// all the points on the polygon's edge are belong to it;
    bool in(const Triangle *t) const;
    bool in_circle(Triangle *t) const;
    int sameside(const Point *p1, const Point *p2, const Point *p) const;
    
    Triangle* search(Triangle* t, std::set<Triangle*> T);
};

class Segment{
public:
    Point *v1,*v2;
    bool bound;//Is this Segment a PSLG constraint,when true,can't delete without disassame;
    
    std::set<Triangle*> linkT;
    
    Segment(Point *u, Point *v);
    //constriant this destruct function by check linkSet,and always "try" to delete linkT;
    ~Segment();
    
    Triangle* linkt(const Triangle* t);
    double length() const;
    bool cross(const Point *o, const Point *p) const;
    bool shared(Triangle *t, std::set<Triangle*> badTS) const;
};

class Vector {
public:
    double x,y;
    
    Vector(const Point *p1, const Point *p2);
    Vector(const int x, const int y);
    
    double length() const;
    int DotProduct(const Vector &v) const;
    int CrossProduct(const Vector &v) const;
    double Angle(const Vector &v) const;
};

class Curve{
    
};

class Triangle{
public:
    Point *v1,*v2,*v3;
    Segment *s1,*s2,*s3;
    
    Triangle(Point *A, Point *B, Point *C);
    ~Triangle();
    
    Triangle* next(const Point *p);
    std::set<Triangle*> explore(const Point* p);
    double 
    bool valid;
    double Radius;
    double xo,yo;
    
    Vector v12;
    Vector v23;
    Vector v31;
};


#endif /* geometry_hpp */
