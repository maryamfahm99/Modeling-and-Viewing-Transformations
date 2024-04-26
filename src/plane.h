//
//  plane.h
//  template
//
//  Created by memo2 on 9/20/23.
//

#ifndef plane_h
#define plane_h

#include "vec.h"
#include "Matrix.h"
#include "ray.h"

// This file consists of three classes that are eseentially made of planes: triangle, quad, cube.
// A triangle is defined by its three points
// A quad by its four vertices
// A cube or a box by its origin and three metrics: width, height and depth.

class triangle{
public:
    vec a;
    vec b;
    vec c;
    vec color;
    matrix transformation;
    
    triangle(){
        a = vec(0,0,0,1);
        b = vec(0,0,0,1);
        c = vec(0,0,0,1);
        color = vec(0.5,0.5,0.5,0);
        matrix m;
        transformation = m.iden();
    }

    triangle(vec x, vec y, vec z, vec col){
        a = x;
        b = y;
        c = z;
        color = col;
        matrix m;
        transformation = m.iden();
    }
    vec get_a(){
        return a;
    }
    vec get_b(){
        return b;
    }
    vec get_c(){
        return c;
    }
    vec get_clr(){
        return color;
    }
    vec TriCenter() {
        float x_avg = (a.x() + b.x() + c.x()) /3.0;
        float y_avg = (a.y() + b.y() + c.y()) /3.0;
        float z_avg = (a.z() + b.z() + c.z()) /3.0;
        return vec(x_avg,y_avg,z_avg,1);
    }
    matrix get_transf(){
        return transformation;
    }
    
};

class quad{
public:
    vec a;
    vec b;
    vec c;
    vec d;
    vec color;
    matrix transformation;
    
    quad(){
        a = vec(1,1,0,1);
        b = vec(1,-1,0,1);
        c = vec(-1,-1,0,1);
        d = vec(-1,1,0,1);
        color = vec(0.5,0.5,0.5,0);
        matrix m;
        transformation = m.iden();
    }
    quad(vec x, vec y, vec z, vec w, vec col){
        a = x;
        b = y;
        c = z;
        d = w;
        color = col;
        matrix m;
        transformation = m.iden();
    }
    vec get_a(){
        return a;
    }
    vec get_b(){
        return b;
    }
    vec get_c(){
        return c;
    }
    vec get_d(){
        return d;
    }
    vec get_clr(){
        return color;
    }
    bool point_on_plane(vec& point){
        vec N =((a-b).cross_product(c-b));
        N.normalized();
        vec n = ((b-point).cross_product(c-point));
        n.normalized();
        if ( N.dot_product(n) < 1e-3 || N.dot_product(n) > -1e-3 ){
            return true;
        }
        return false;
    }
    matrix get_transf(){
        return transformation;
    }
};

class cube{
public:
    vec origin;
    float width;
    float height;
    float depth;
    vec color;
    double theta;
    matrix transformation;
    
    cube(){
        origin = vec(0,0,0,1);
        width = 1;
        height = 1;
        depth = 1;
        color = vec(0.5,0.5,0.5,0);
        theta = 0.0;
        matrix m;
        transformation = m.iden();
        
    }
    cube(vec o, float w, float h, float d, vec c, double th){
        origin = o;
        width = w;
        height = h;
        depth = d;
        color = c;
        theta = th;
        matrix m;
        transformation = m.iden();
    }
    
    vec get_origin(){
        return origin;
    }
    float get_w(){
        return width;
    }
    float get_h(){
        return height;
    }
    float get_d(){
        return depth;
    }
    vec get_clr(){
        return color;
    }
    double get_theta(){
        return theta;
    }
    matrix get_transf(){
        return transformation;
    }
};

#endif /* plane_h */
