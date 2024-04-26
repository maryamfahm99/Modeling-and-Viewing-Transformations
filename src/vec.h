//
//  vec.h
//  template
//
//  Created by memo2 on 9/6/23.
//
#pragma once

#ifndef vec_h
#define vec_h

#include <cmath>
#include <iostream>

using std::sqrt;

class vec{
   public:
    float v[4];
    
    // Constructors:
    vec(){
        v[0]=0;
        v[1]=0;
        v[2]=0;
        v[3]=0;
    }
    vec(float x, float y, float z){
        v[0]=x;
        v[1]=y;
        v[2]=z;
        v[3]=0;
    }
    // create a point using a vector object:
    vec(float x, float y, float z, float w){
        v[0]=x;
        v[1]=y;
        v[2]=z;
        v[3]=w; // should be 1
    }
    vec(const vec& t){  // Copy constructor
        v[0]=t.v[0];
        v[1]=t.v[1];
        v[2]=t.v[2];
        v[3]=t.v[3];
    }
    
    float x() {
          return v[0];
        }
    float y() {
          return v[1];
        }
    float z() {
          return v[2];
        }
    float w() {
          return v[3];
        }
    // Operators
    vec operator+(const vec& other){ // if u, v are vectors then u+v = [x1,y1,z1,0]+[x2,y2,z2,0]=[x1+x2,y1+y2,z1+z2,0] thus w will not affect the result.
        vec result = vec(v[0]+other.v[0], v[1]+other.v[1], v[2]+other.v[2], v[3]+other.v[3]);
        return result;
    }
    vec operator-(const vec& other){
        vec result = vec(v[0]-other.v[0], v[1]-other.v[1], v[2]-other.v[2], v[3]-other.v[3]);
        return result;
    }
    vec operator*(const float& scalar){
        vec result = vec(v[0]*scalar, v[1]*scalar,v[2]*scalar, v[3]*scalar);
        return result;
    }
    vec operator/(const float& scalar){
        vec result = vec(v[0]/scalar, v[1]/scalar,v[2]/scalar, v[3]/scalar);
        return result;
    }
    bool operator==(const vec& u){
        if (v[0] == u.v[0] && v[1] == u.v[1] && v[2] == u.v[2] && v[3] == u.v[3]){
            return true;
        }
        return false;
    }
    // Dot product
    float dot_product(const vec& other){ //  this should not be accessed by points instances! may not be safe!
        float dot_product = (v[0]*other.v[0] + v[1]*other.v[1] + v[2]*other.v[2] + v[3]*other.v[3]);
        return dot_product;
    }
    // Cross product
    vec cross_product(const vec& u){
        vec cross_product = vec(v[1]*u.v[2]-v[2]*u.v[1], v[0]*u.v[2]-v[2]*u.v[0], v[0]*u.v[1]-v[1]*u.v[0],0);
        return cross_product;
    }
    // Length
    float length(){
        vec u = vec(v[0], v[1],v[2],0);
        return sqrt(dot_product(u));
    }
    // Normalize the vector
    void normalized(){
        float l = length();
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
    }
    void transpose(){
        float a = v[0];
        v[0]=v[2];
        v[2]=a;
    }
    // Print the vectors
    void print(){
        
        std::cout << v[0] << '\t';
        std::cout << v[1] << '\t';
        std::cout << v[2] << '\t';
        std::cout << v[3] << '\t';
        std::cout << std::endl;
    }
    vec wise(vec& other){
        vec element_wise = vec(v[0]*other.v[0], v[1]*other.v[1],v[2]*other.v[2], v[3]*other.v[3]);
        return element_wise;
    }
    
};

//struct Pixel{
//    int x;
//    int y;
//    vec color;
//};



#endif /* vector_h */
