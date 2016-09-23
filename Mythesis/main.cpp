
//
//  main.cpp
//  test
//
//  Created by t_wangju on 16/8/5.
//  Copyright © 2016年 t_wangju. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "geometry.hpp"
#include "triangulation.hpp"

GLubyte *temp=NULL;
int plot =0;
void init(void){
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 800);
}

void lineSegment(void){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor4f(0.0,0.0,0.5,0.5);
    
    std::set<Point*> input;
    std::set<Segment*> PSLG;
    int x,y;
    double theta;
    Point *last=NULL;
    glBegin(GL_POLYGON);
    {
        for(int i=0;i<8;i++){
            theta = 0.25*M_PI*i;
            x=400+300*cos(theta);
            y=400+300*sin(theta);
            Point *temp=new Point(x,y);
            input.insert(temp);
            glVertex2i(x, y);
            if(last==NULL){
                last=temp;
            }else{
                Segment *s=new Segment(last,temp);
                PSLG.insert(s);
                last=temp;
            }
        }
        Point *temp;
        temp=new Point(400,400);
        input.insert(temp);
        for(int i=0;i<50;i++){
            int randx=rand()%600+100;
            int randy=rand()%600+100;
            temp=new Point(randx,randy);
            input.insert(temp);
        }
        //
        //        glVertex2i(temp->x, temp->y);
        //        glVertex2i(temp->x, temp->y);
        //        glVertex2i(temp->x, temp->y);
        //        glVertex2i(temp->x, temp->y);
        
    }
    glEnd();
    glFlush();
    std::set<Point*> possioninput=Possion_Disk_Sampling(input, 100, 100, 700, 700, 30, 20);
    std::set<Triangle*> answer = Bowyer_Watson_Triangulation(possioninput, 0, 0, 800, 800);
//    answer=Ruppert_Refine(answer, PSLG);
    glColor4f(1.0,1.0,1.0,0.0);
    for(std::set<Triangle*>::iterator titer=answer.begin();titer!=answer.end();titer++){
        if((*titer)->s1->linkT.size()>2||(*titer)->s2->linkT.size()>2||(*titer)->s3->linkT.size()>2){
            std::cerr<<"error: a triangle's edge has two triangles"<<std::endl;
        }
        
        glBegin(GL_LINE_LOOP);
        glVertex2i((*titer)->v1->x, (*titer)->v1->y);
        glVertex2i((*titer)->v2->x, (*titer)->v2->y);
        glVertex2i((*titer)->v3->x, (*titer)->v3->y);
        glEnd();
    }
    
    glFlush();
}


GLenum errorCheck(void){
    GLenum code;
    code = glGetError();
    const GLubyte* string;
    if(code != GL_NO_ERROR){
        string = gluErrorString(code);
        fprintf(stderr,"openGL error :%s\n",string);
    }
    return code;
}


int main(int argc, char * argv[]) {
    // insert code here...r
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    
    glutCreateWindow("TriangulationExample");
    
    init();
    glutDisplayFunc(lineSegment);
    //    glReadPixels(0, 0, 200, 200, GL_RGB, GL_UNSIGNED_BYTE, temp);
    //    //    glutDisplayFunc(redisplay);
    
    
    glutMainLoop();
    
    return 0;
}
