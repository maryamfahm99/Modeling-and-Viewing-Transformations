//
//  ellipsoid.h
//  template
//
//  Created by memo2 on 9/20/23.
//

#ifndef ellipsoid_h
#define ellipsoid_h

#include "vec.h"
#include "Matrix.h"
#include "ray.h"

// This file consists of the ellipsoid class.
// I identify the ellipsoid by the center  and the three axes. 

class ellipsoid{
   public:
    vec center;
    float a, b, c;
    double theta;
    vec color;
    matrix transformation;
    
    ellipsoid(){
        a = 0.3; b  = 0.3;  c = 0.3;
        center = vec(0,0,0,1);
        color = vec(0.5,0.5,0.5,0);
        theta = 0.0;
        matrix m;
        transformation = m.iden();
    }

    ellipsoid(vec o, float u, float v, float w, vec clr, double th){
        center = o;
        a = u;
        b = v;
        c = w;
        color = clr;
        theta = th;
        matrix m;
        transformation = m.iden();
        
    }
    vec get_center(){
        return center;
    }
    float get_a(){
        return a;
    }
    float get_b(){
        return b;
    }
    float get_c(){
        return c;
    }
    double get_theta(){
        return theta;
    }
    vec get_clr(){
        return color;
    }
    bool point_on_ellipsoid(vec& point){
        float value = ((point.x()-center.x())*(point.x()-center.x()))/(a*a) +
                      ((point.y()-center.y())*(point.y()-center.y()))/(b*b) +
                      ((point.z()-center.z())*(point.z()-center.z()))/(c*c);
        
        if ((value - 1) < 1e-3 && (value-1) > -1e-3 ){
            return true;
        }
        return false;
    }
    matrix get_transf(){
        return transformation;
    }

};

#endif /* ellipsoid_h */
