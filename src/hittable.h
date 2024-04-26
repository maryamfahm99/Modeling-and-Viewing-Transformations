//
//  hittable.h
//  template
//
//  Created by memo2 on 9/22/23.
//
#pragma once
#ifndef hittable_h
#define hittable_h

#include "vec.h"
#include "Matrix.h"
#include "ray.h"


// This class hold the normal, point, and t, color for each pixel. It is then handed over to the shade, shadow function

class hittable{
   public:
    vec point;
    vec n;
    float t;
    vec color;
    double theta;
    matrix transformation;
    
    hittable(){
        point = vec(0,0,0,0);
        n = vec(0,0,0,1);
        t = -1;
        color = vec(0.4,0.5,0.5,0);
        theta = 0.0;
        matrix m;
        transformation =  m.iden();
    }
    hittable(vec pnt, vec N, float time, vec clr, double th){
        point = pnt;
        n = N;
        t = time;
        color = clr;
        theta = th;
        matrix m;
        transformation =  m.iden();
    }
    vec get_n(){
        return n;
    }
    vec get_point(){
        return point;
    }
    float get_t(){
        return t;
    }
    vec get_clr(){
        return color;
    }
    float distance(vec& origin){
        return (point-origin).length();
    }
    double get_theta(){
        return theta;
    }
    matrix get_transf(){
        return transformation;
    }
};

#endif /* hittable_h */
