//
//  geometry.cpp
//  Mythesis
//  This is a basic lib for geometry operate
//  Created by t_wangju on 9/14/16.
//  Copyright © 2016 t_wangju. All rights reserved.
//

////////////////////////////////////
//   DO NOT USE ANY GLOBAL VALUE!
////////////////////////////////////

// name: substance set with CAPITAL,and single substance with ordinary,functions' name should as p_t_insert/p_T_research,action at end and with '_';
// new and delete should be used when construct Points, and always construct Segment/Triangle with Points' addr;
// for convenient,all Points which on a Triangle's edges are included by this Triangle,so there some Points are included by different Triangles,this should pay much attention;
// Class only have the data,and most of operators should be complete by programmer;
// all data struct can't use any addr that has a null value;


#include "geometry.hpp"

#define DOUBLEZERO 1e-8

double lambda(int x1, int y1, int x2, int y2, int x3, int y3){
    return (x1*y2+x2*y3+x3*y1)-(y1*x2+y2*x3+y3*x1);
}
double max(double m1, double m2){
    return m1>m2?m1:m2;
}
int ads(int input){
    if(input>0){
        return 1;
    }
    if(input<0){
        return -1;
    }
    return 0;
}
Point::Point(const int x, const int y){
    this->x=x;
    this->y=y;
    simple=true;
    linkT.clear();
    linkS.clear();
}
Point::~Point(){
    if(simple==true){
        return;
    }
    if(linkT.empty()&&linkS.empty()){
        return;
    }else{
        std::cerr<<"error: destructing a point linked to other element"<<std::endl;
        exit(0);
    }
}
bool Point::operator==(const Point &p) const{
    if(x==p.x&&y==p.y){
        return true;
    }
    return false;
}
bool Point::operator!=(const Point &p) const{
    return !((*this)==p);
}
Point* Point::linkp(const Segment *s) const{
    if(s->v1==this){
        return s->v2;
    }
    if(s->v2==this){
        return s->v1;
    }
    return NULL;
}
Segment* Point::links(const Point *p) const{
    std::set<Segment*>::iterator siter;
    Segment *s1=NULL,*s2=NULL;
    for(siter=linkS.begin();siter!=linkS.end();siter++){
        if(linkp(*siter)==p){
            s1=*siter;
        }
    }
    for(siter=p->linkS.begin();siter!=p->linkS.end();siter++){
        if(p->linkp(*siter)==this){
            s2=*siter;
        }
    }
    if(s1==s2){
        return s1;
    }
    return NULL;
}
double Point::distance(const Point *p) const{
    Vector v(this,p);
    return v.length();
}
double Point::distance(const Segment *s) const{
    Vector pv1(this,s->v1);
    Vector pv2(this,s->v2);
    Vector v1v2(s->v1,s->v2);
    double DotProduct1=pv1.DotProduct(v1v2);
    double DotProduct2=pv2.DotProduct(v1v2);
    if(DotProduct1*DotProduct2<=0){
        Vector v(s->v2->y-s->v1->y,s->v1->x-s->v2->x);
        double Area=abs(x*s->v1->y+s->v1->x*s->v2->y+s->v2->x*y-
                        y*s->v1->x+s->v1->y*s->v2->x+s->v2->y*x);
        double Hypotenuse=v1v2.length();
        return Area/Hypotenuse;
    }else{
        return distance(s->v1)<distance(s->v2)?distance(s->v1):distance(s->v2);
    }
}
bool Point::on(const Segment *s) const{
    if(distance(s)<DOUBLEZERO&&distance(s)>-DOUBLEZERO){
        return true;
    }else{
        return false;
    }
}
bool Point::in(std::vector<Point *> PV) const{
    double WindingNum=0;
    PV.push_back(*PV.begin());
    std::vector<Point*>::iterator piter;
    for(piter=PV.begin();piter!=PV.end();piter++){
        Vector v1(this,*piter);
        Vector v2(this,*piter);
        WindingNum+=v1.Angle(v2);
    }
    if(WindingNum<DOUBLEZERO&&WindingNum>-DOUBLEZERO){
        return true;
    }else{
        return false;
    }
}
bool Point::in(const Triangle *t) const{
    std::vector<Point*> temp;
    temp.push_back(t->v1);
    temp.push_back(t->v2);
    temp.push_back(t->v3);
    return in(temp);
}
bool Point::in_circle(Triangle *t) const{
    if(t->valid==false){
        double module=2*lambda(t->v1->x, t->v1->y, t->v2->x, t->v2->y, t->v3->x, t->v3->y);
        double xproduct=lambda(t->v1->x*t->v1->x+t->v1->y*t->v1->y, t->v1->y, t->v2->x*t->v2->x+t->v2->y*t->v2->y, t->v2->y, t->v3->x*t->v3->x+t->v3->y*t->v3->y, t->v3->y);
        double yproduct=lambda(t->v1->x, t->v1->x*t->v1->x+t->v1->y*t->v1->y, t->v2->x, t->v2->x*t->v2->x+t->v2->y*t->v2->y, t->v3->x, t->v3->x*t->v3->x+t->v3->y*t->v3->y);
        t->xo=xproduct/module;
        t->yo=yproduct/module;
        Point to(t->xo,t->yo);
        t->Radius=max(max(to.distance(t->v1), to.distance(t->v2)), to.distance(t->v3));
        t->valid=true;
    }
    Point O(t->xo,t->yo);
    double dis=O.distance(this);
    if(dis<t->Radius){
        return true;
    }else{
        return false;
    }
}
int Point::sameside(const Point *p1, const Point *p2, const Point *p) const{
    Vector ov1(this,p1);
    Vector ov2(this,p2);
    Vector pv1(p,p1);
    Vector pv2(p,p2);
    int theta1=pv1.CrossProduct(pv2);
    int theta2=ov1.CrossProduct(ov2);
    theta1=ads(theta1);
    theta2=ads(theta2);
    return theta1*theta2;
}
// BUG HERE;
Triangle* Point::search(Triangle* t, std::set<Triangle *> T) {
    Triangle* next=t->next(this);
    if(next==t){
        return t;
    }
    return this->search(next, T);
}

Segment::Segment(Point *u, Point *v){
    v1=u;
    v2=v;
    v1->linkS.insert(this);
    v2->linkS.insert(this);
}
Segment::~Segment(){
    v1->linkS.erase(this);
    v2->linkS.erase(this);
}
Triangle* Segment::linkt(const Triangle *t){
    std::set<Triangle*>::iterator titer;
    for(titer=linkT.begin();titer!=linkT.end();titer++){
        if((*titer)!=t){
            return *titer;
        }
    }
    return NULL;
}
double Segment::length() const{
    Vector v(v1,v2);
    return v.length();
}
bool Segment::cross(const Point *o, const Point *p) const{
    if(p->sameside(v1, v2, o)<=0&&p->sameside(o, v1, v2)>=0&&p->sameside(o, v2, v1)>=0){
        return true;
    }
    return false;
}
bool Segment::shared(Triangle *t, std::set<Triangle *> badTS) const{
    std::set<Triangle*>::iterator titer;
    for(titer=linkT.begin();titer!=linkT.end();titer++){
        if(*titer==t){
            continue;
        }
        if(badTS.find(*titer)!=badTS.end()){
            return true;
        }
    }
    return false;
}

Vector::Vector(const Point *p1, const Point *p2){
    x=p2->x-p1->x;
    y=p2->y-p1->y;
}
Vector::Vector(const int x, const int y){
    this->x=x;
    this->y=y;
}
double Vector::length() const{
    double len=sqrt(x*x+y*y);
    return len;
}
int Vector::DotProduct(const Vector &v) const{
    return x*v.x+y*v.y;
}
int Vector::CrossProduct(const Vector &v) const{
    return x*v.y-y*v.x;
}
// when Clockwise from v1 to v2, return a negative angle value;
double Vector::Angle(const Vector &v) const{
    if(length()==0||v.length()==0)
        return 0;
    double dotproduct   = DotProduct(v);
    double module       = length()*v.length();
    double cosine       = dotproduct/module;
    cosine=cosine<=  1? cosine :  1;
    cosine=cosine>= -1? cosine : -1;
    double theta        = acos(cosine)*180/M_PI;
    if(CrossProduct(v)<0){
        theta=-theta;
    }
    return theta;
}

Triangle::Triangle(Point *A, Point *B, Point *C):v1(A),v2(B),v3(C),v12(A,B),v23(B,C),v31(C,A){
    s1=v2->links(v3);
    s2=v3->links(v1);
    s3=v1->links(v2);
    if(s1==NULL){
        s1=new Segment(v2,v3);
    }
    if(s2==NULL){
        s2=new Segment(v3,v1);
    }
    if(s3==NULL){
        s3=new Segment(v1,v2);
    }
    v1->linkT.insert(this);
    v2->linkT.insert(this);
    v3->linkT.insert(this);
    s1->linkT.insert(this);
    s2->linkT.insert(this);
    s3->linkT.insert(this);
    if(s1->linkT.size()>2||s2->linkT.size()>2||s3->linkT.size()>2){
        std::cerr<<"error: a segment link to more than two Triangles"<<std::endl;
    }
    valid=false;
}
Triangle::~Triangle(){
    v1->linkT.erase(this);
    v2->linkT.erase(this);
    v3->linkT.erase(this);
    s1->linkT.erase(this);
    s2->linkT.erase(this);
    s3->linkT.erase(this);
    if(s1->linkT.empty()){
        delete s1;
    }
    if(s2->linkT.empty()){
        delete s2;
    }
    if(s3->linkT.empty()){
        delete s3;
    }
}
Triangle* Triangle::next(const Point *p){
    if(p->sameside(v3, v1, v2)>=0&&p->sameside(v2, v3, v1)<0){
        return s1->linkt(this);
    }
    if(p->sameside(v1, v2, v3)>=0&&p->sameside(v3, v1, v2)<0){
        return s2->linkt(this);
    }
    if(p->sameside(v2, v3, v1)>=0&&p->sameside(v1, v2, v3)<0){
        return s3->linkt(this);
    }
    return const_cast<Triangle*>(this);
}
std::set<Triangle*> Triangle::explore(const Point* p){
    int count=0;
    std::set<Triangle*> badTS;
    std::vector<Triangle*> badTV;
    badTV.push_back(this);
    while(!badTV.empty()){
        Triangle *t=*badTV.begin();
        if(t!=NULL){
            if(p->in_circle(t)){
                badTS.insert(t);
                if(badTS.size()!=count){
                    count++;
                    badTV.push_back(t->s1->linkt(t));
                    badTV.push_back(t->s2->linkt(t));
                    badTV.push_back(t->s3->linkt(t));
                }
            }
        }
        badTV.erase(badTV.begin());
    }
    return badTS;
}
