//
//  triangulation.hpp
//  Mythesis
//
//  Created by t_wangju on 16/9/20.
//  Copyright © 2016年 t_wangju. All rights reserved.
//

#ifndef triangulation_hpp
#define triangulation_hpp

#include <iostream>
#include "geometry.hpp"
double Min(double d1, double d2);
double Max(double d1, double d2);

bool valid(Point *p, Triangle* t, int MINDIS);
bool valid(Point *p, Segment *s);
bool valid(Triangle *t, double MINRATE);

std::set<Triangle*> init(int xmin, int ymin, int xmax, int ymax);
std::set<Triangle*> insert(std::set<Triangle*> TS, Point *p);



std::set<Point*> Possion_Disk_Sampling(std::set<Point*> P, int xmin, int ymin, int xmax, int ymax, int RADIUS, int trytimes);
std::set<Triangle*> Bowyer_Watson_Triangulation(std::set<Point*> P, int xmin, int ymin, int xmax, int ymax);
std::set<Triangle*> Ruppert_Refine(std::set<Triangle*> TS, std::set<Segment*> PSLG);

#endif /* triangulation_hpp */
