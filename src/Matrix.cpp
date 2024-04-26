//
//  Matrix.cpp
//  template
//
//  Created by memo2 on 10/3/23.
//

#include <stdio.h>
#include "Matrix.h"

matrix matrix::translate( float tx, float ty,  float tz){
    vec u = vec(1,0,0,tx);
    vec v = vec(0,1,0,ty);
    vec w = vec(0,0,1,tz);
    vec t = vec(0,0,0,1);
    matrix m = matrix(u,v,w,t);
    return m;
}

matrix matrix::scale(float sx, float sy, float sz){
    vec u = vec(sx,0,0,0);
    vec v = vec(0,sy,0,0);
    vec w = vec(0,0,sz,0);
    vec t = vec(0,0,0,1);
    matrix m = matrix(u,v,w,t);
    return m;
}
matrix matrix::RotateZ( double theta){
    vec u = vec(cos(theta),-sin(theta),0,0);
    vec v = vec(sin(theta),cos(theta),0,0);
    vec w = vec(0,0,1,0);
    vec t = vec(0,0,0,1);
    matrix m = matrix(u,v,w,t);
    return m;
}

matrix matrix::RotateX( double theta){
    vec u = vec(1,0,0,0);
    vec v = vec(0,cos(theta),-sin(theta),0);
    vec w = vec(0,sin(theta),cos(theta),0);
    vec t = vec(0,0,0,1);
    matrix m = matrix(u,v,w,t);
    return m;
}

matrix matrix::RotateY( double theta){
    vec u = vec(cos(theta),0,sin(theta),0);
    vec v = vec(0,1,0,0);
    vec w = vec(-sin(theta),0,cos(theta),0);
    vec t = vec(0,0,0,1);
    matrix m = matrix(u,v,w,t);
    return m;
}
