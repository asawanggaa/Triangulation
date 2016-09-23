//
//  triangulation.cpp
//  Mythesis
//
//  Created by t_wangju on 16/9/20.
//  Copyright © 2016年 t_wangju. All rights reserved.
//

#include "triangulation.hpp"
#include <vector>
#include <list>
#define MAX_BOUND 10000

double Min(double d1, double d2){
    return d1<d2?d1:d2;
}
double Max(double d1, double d2){
    return d1>d2?d1:d2;
}
bool valid(Point *p, Triangle* t, int MINDIS){
    if(p->distance(t->v1)<MINDIS){
        return false;
    }
    if(p->distance(t->v2)<MINDIS){
        return false;
    }
    if(p->distance(t->v3)<MINDIS){
        return false;
    }
    return true;
}
bool valid(Point *p, Segment *s){
    double xo=(s->v1->x+s->v2->x)/2;
    double yo=(s->v1->y+s->v2->y)/2;
    double r=sqrt(pow(s->v1->x-xo, 2)+pow(s->v1->y-yo, 2));
    Point temp(xo,yo);
    if(temp.distance(p)<r){
        return false;
    }else{
        return true;
    }
}
bool valid(Triangle *t, double MINRATE){
    double max=Max(t->s1->length(), Max(t->s2->length(), t->s3->length()));
    double min=Min(t->s1->length(), Max(t->s2->length(), t->s3->length()));
    if(max/min>MINRATE){
        return false;
    }else{
        return true;
    }
}
std::set<Triangle*> init(int xmin, int ymin, int xmax, int ymax){
    Point *A=new Point(xmin, ymin);
    Point *B=new Point(xmax, ymin);
    Point *C=new Point(xmax, ymax);
    Point *D=new Point(xmin, ymax);
    
    Triangle *t1=new Triangle(A, B, C);
    Triangle *t2=new Triangle(C, D, A);
    
    std::set<Triangle*> TS;
    
    TS.insert(t1);
    TS.insert(t2);
    return TS;
}
std::set<Triangle*> insert(std::set<Triangle*> TS, Point *p){
    Triangle *start=p->search(*TS.begin(), TS);
    if(!valid(p, start, 2)){
        return TS;
    }
    std::set<Triangle*> badTS=start->explore(p);
    std::set<Triangle*>::iterator btiter;
    std::set<Segment*> SSS;
    for(btiter=badTS.begin();btiter!=badTS.end();btiter++){
        if(!(*btiter)->s1->shared(*btiter, badTS)){
            SSS.insert((*btiter)->s1);
        }
        if(!(*btiter)->s2->shared(*btiter, badTS)){
            SSS.insert((*btiter)->s2);
        }
        if(!(*btiter)->s3->shared(*btiter, badTS)){
            SSS.insert((*btiter)->s3);
        }
    }
    std::set<Segment*>::iterator siter;
    std::vector<Segment>::iterator viter;
    std::vector<Segment> SSV;
    for(siter=SSS.begin();siter!=SSS.end();siter++){
        SSV.push_back(**siter);
    }
    for(btiter=badTS.begin();btiter!=badTS.end();btiter++){
        TS.erase(*btiter);
        delete *btiter;
    }
    for(viter=SSV.begin();viter!=SSV.end();viter++){
        Triangle *temp=new Triangle(p, viter->v1, viter->v2);
        TS.insert(temp);
        viter->linkT.clear();
    }
    return TS;
}

std::set<Point*> Possion_Disk_Sampling(std::set<Point*> P, int xmin, int ymin, int xmax, int ymax, int RADIUS, int trytimes){
    int m=(xmax-xmin)/RADIUS+((xmax-xmin)%RADIUS==0?1:1);
    int n=(ymax-ymin)/RADIUS+((ymax-ymin)%RADIUS==0?1:1);
    std::vector<Point*> **mat=new std::vector<Point*>*[m];
    for(int i=0;i<m;i++){
        mat[i]=new std::vector<Point*>[n];
        for(int j=0;j<n;j++){
            mat[i][j].clear();
        }
    }
    
    Point *initp=new Point(rand()%(xmax-xmin)+xmin,rand()%(ymax-ymin)+ymin);
    std::set<Point*> activelist;
    
    for(auto piter=P.begin();piter!=P.end();piter++){
        mat[((*piter)->x-xmin)/RADIUS][((*piter)->y-ymin)/RADIUS].push_back(*piter);
        activelist.insert(*piter);
    }
    mat[(initp->x-xmin)/RADIUS][(initp->y-ymin)/RADIUS].push_back(initp);
    activelist.insert(initp);
    while(!activelist.empty()){
        Point *p=*activelist.begin();
        const double pi=std::acos(-1);
        for(int k=0;k<trytimes;k++){
            bool val=true;
            double theta=rand()%360-180;
            theta*=pi/180;
            double r=rand()%RADIUS+RADIUS;
            double x=cos(theta)*r+p->x;
            double y=sin(theta)*r+p->y;
            if(x<xmin||x>=xmax||y<ymin||y>ymax){
                continue;
            }
            Point *temp=new Point(x,y);
            int ox=(x-xmin)/RADIUS;
            int oy=(y-ymin)/RADIUS;
            for(int s=-2;s<2;s++){
                for(int t=-2;t<2;t++){
                    if(ox+s<0||ox+s>=m||oy+t<0||oy+t>=n){
                        continue;
                    }
                    if(mat[ox+s][oy+t].empty()){
                        continue;
                    }
                    for(auto i=mat[ox+s][oy+t].begin();i!=mat[ox+s][oy+t].end();i++){
                        if((*i)->distance(temp)<RADIUS){
                            val=false;
                        }
                    }
                }
            }
            if(val){
                activelist.insert(temp);
                mat[(temp->x-xmin)/RADIUS][(temp->y-ymin)/RADIUS].push_back(temp);
            }else{
                delete temp;
            }
        }
        activelist.erase(activelist.begin());
    }
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            for(auto k=mat[i][j].begin();k!=mat[i][j].end();k++){
                P.insert(*k);
            }
        }
    }
    return P;
}

std::set<Triangle*> Bowyer_Watson_Triangulation(std::set<Point*> PS, int xmin, int ymin, int xmax, int ymax){
    std::set<Triangle*> TS=init(xmin, ymin, xmax, ymax);
    std::set<Point*>::iterator piter;
    for(piter=PS.begin();piter!=PS.end();piter++){
        TS=insert(TS, *piter);
    }
    
    Point A(xmin, ymin);
    Point B(xmax, ymin);
    Point C(xmax, ymax);
    Point D(xmin, ymax);
    std::set<Triangle*>::iterator titer=TS.begin();
    while(titer!=TS.end()){
        if(*((*titer)->v1)==A||*((*titer)->v1)==B||*((*titer)->v1)==C||*((*titer)->v1)==D||
           *((*titer)->v2)==A||*((*titer)->v2)==B||*((*titer)->v2)==C||*((*titer)->v2)==D||
           *((*titer)->v3)==A||*((*titer)->v3)==B||*((*titer)->v3)==C||*((*titer)->v3)==D
           ){
            Triangle *temp=*titer;
            titer++;
            TS.erase(temp);
        }else{
            titer++;
        }
    }
    return TS;
}

std::set<Triangle*> Ruppert_Refine(std::set<Triangle*> TS, std::set<Segment*> PSLG){
    bool status=false;
    std::set<Segment*>::iterator siter;
    std::set<Segment*> encroachedSegments;
    std::set<Triangle*>::iterator titer;
    std::set<Triangle*> skinnyTriangles;
    for(siter=PSLG.begin();siter!=PSLG.end();siter++){
        for(auto i=(*siter)->v1->linkT.begin();i!=(*siter)->v1->linkT.end();i++){
            for(auto j=(*siter)->v2->linkT.begin();j!=(*siter)->v2->linkT.end();j++){
                if((*i)==(*j)){
                    status=true;
                    break;
                }
            }
            if(status==true){
                break;
            }
        }
        if(!status){
            encroachedSegments.insert(*siter);
        }
    }
    for(titer=TS.begin();titer!=TS.end();titer++){
        if(!valid((*titer)->v1, (*titer)->s1)){
            encroachedSegments.insert((*titer)->s1);
        }
        if(!valid((*titer)->v2, (*titer)->s2)){
            encroachedSegments.insert((*titer)->s2);
        }
        if(!valid((*titer)->v3, (*titer)->s3)){
            encroachedSegments.insert((*titer)->s3);
        }
        if(!valid(*titer, 2)){
            skinnyTriangles.insert(*titer);
        }
    }
    std::set<Triangle*> TSet=TS;
    while((!encroachedSegments.empty())||(!skinnyTriangles.empty())){
        if(!encroachedSegments.empty()){
            for(siter=encroachedSegments.begin();siter!=encroachedSegments.end();siter++){
                Point *temp=new Point(((*siter)->v1->x+(*siter)->v2->x)/2 ,((*siter)->v1->y+(*siter)->v2->y)/2);
                TSet=insert(TSet, temp);
            }
        }else if(!skinnyTriangles.empty()){
            for(titer=skinnyTriangles.begin();titer!=skinnyTriangles.end();titer++){
                Point *temp=new Point((*titer)->xo,(*titer)->yo);
                if(!valid(temp, (*titer)->s1)){
                    encroachedSegments.insert((*titer)->s1);
                }else if(!valid(temp, (*titer)->s2)){
                    encroachedSegments.insert((*titer)->s2);
                }else if(!valid(temp, (*titer)->s3)){
                    encroachedSegments.insert((*titer)->s3);
                }else{
                    TSet=insert(TSet, temp);
                }
            }
        }
        for(siter=PSLG.begin();siter!=PSLG.end();siter++){
            for(auto i=(*siter)->v1->linkT.begin();i!=(*siter)->v1->linkT.begin();i++){
                for(auto j=(*siter)->v2->linkT.begin();j!=(*siter)->v2->linkT.end();j++){
                    if((*i)==(*j)){
                        status=true;
                        break;
                    }
                }
                if(status==true){
                    break;
                }
            }
            if(!status){
                encroachedSegments.insert(*siter);
            }
        }
        for(titer=TS.begin();titer!=TS.end();titer++){
            if(!valid((*titer)->v1, (*titer)->s1)){
                encroachedSegments.insert((*titer)->s1);
            }
            if(!valid((*titer)->v2, (*titer)->s2)){
                encroachedSegments.insert((*titer)->s2);
            }
            if(!valid((*titer)->v3, (*titer)->s3)){
                encroachedSegments.insert((*titer)->s3);
            }
            if(!valid(*titer, 2)){
                skinnyTriangles.insert(*titer);
            }
        }
    }
    return TSet;
}
