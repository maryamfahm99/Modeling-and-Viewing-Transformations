//
//  ray.h
//  template
//
//  Created by memo2 on 9/13/23.
//
#pragma once

#ifndef ray_h
#define ray_h

#include "vec.h"
#include "Matrix.h"
#include "sphere.h"
#include "ellipsoid.h"
#include "plane.h"
#include "hittable.h"

#include <iostream>
#include <string>
#include <cmath>
using namespace std;


using std::sqrt;

class ray{
  private:
    vec origin; // this is a point
    vec direction; // this is a vector
  public:
    
    bool rotatex,rotatey,rotatez;
    ray(){
        origin = vec(0,0,-1,1);
        direction = vec(0,0,0,0);
    }
    
    ray(const vec& o, const vec& d){
        origin = o;
        direction = d;
    }
    vec get_origin() const{
        return origin;
    }
    vec get_dir() const{
        return direction;
    }
    vec t_position(float t){
        return origin + (direction*t);
    }
    // These functions find the intersection between the ray and the objects
    float sphr_intersect(vec& o, vec& d, sphere& s);
    float ellips_intersect(ray& r, ellipsoid& e);
    float triangle_intersect(ray& r, triangle& pln);
    float quad_intersect(ray& r, quad& q);
    hittable cube_intersect(ray& r, cube& c);
    
    // These functions find the closest intersection for the shading & shadow and computes the final color
    vec shade(hittable& hit,vec& pixel, ray&r);
    hittable ray_intersect(ray& r, quad& q, sphere s[4], ellipsoid e[4], triangle tr[4], cube c[4]);
    bool shadowRay_intersect(ray& r,vec& point, quad& q, sphere s[4], ellipsoid e[4], triangle tr[4], cube c[4]);
    vec shadow(hittable& hit,vec& pixel);
    vec background(ray&r);
    
    
};





#endif /* ray_h */
