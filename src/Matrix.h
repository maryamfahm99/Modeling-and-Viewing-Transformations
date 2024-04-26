//
//  Matrix.h
//  template
//
//  Created by memo2 on 9/9/23.
//
#pragma once

#ifndef Matrix_h
#define Matrix_h

#include <cmath>
#include <iostream>
#include "vec.h"

using std::sqrt;


class matrix{
   public:
    float m[4][4];
    
    // Constructors:
    matrix(){
        
        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                m[i][j]=0;
            }
        }
        m[3][3]=1;
        
    }
    matrix(vec &u, vec &v, vec &w, vec &e){
        m[0][0]= u.x(); m[0][1]= u.y(); m[0][2]= u.z(); m[0][3]= u.w();
        m[1][0]= v.x(); m[1][1]= v.y(); m[1][2]= v.z(); m[1][3]= v.w();
        m[2][0]= w.x(); m[2][1]= w.y(); m[2][2]= w.z(); m[2][3]= w.w();
        m[3][0]= e.x(); m[3][1]= e.y(); m[3][2]= e.z(); m[3][3]= e.w();
    }
    // get the vectors
    vec get_vector1(){
        vec v = vec(m[0][0],m[0][1],m[0][2],m[0][3]);
        return v;
    }
    vec get_vector2(){
        vec v = vec(m[1][0],m[1][1],m[1][2],m[1][3]);
        return v;
    }
    vec get_vector3(){
        vec v = vec(m[2][0],m[2][1],m[2][2],m[2][3]);
        return v;
    }
    vec get_vector4(){
        vec v = vec(m[3][0],m[3][1],m[3][2],m[3][3]);
        return v;
    }
    // Opereations
    matrix operator+(const matrix& t){
        matrix r = matrix();
        
        for(int i = 0; i< 4; i++){
            for(int j = 0; j<4; j++){
                if(i == 4){
                    r.m[i][j]= 0;
                }else{
                    r.m[i][j]= r.m[i][j]+t.m[i][j];
                }
            }
        }
        r.m[3][3] = 1;
        return r;
    }
    matrix operator-(const matrix& t){
        matrix r = matrix();
        
        for(int i = 0; i< 4; i++){
            for(int j = 0; j<4; j++){
                if(i == 4){
                    r.m[i][j]= 0;
                }else{
                    r.m[i][j]= r.m[i][j]-t.m[i][j];
                }
            }
        }
        r.m[3][3] = 1;
        return r;
    }
    matrix operator*(const matrix& t){
        matrix r = matrix();
    
        for(int i = 0; i< 4; i++){ // for each vector i.e. row multiplies the cols vectors
            for(int j = 0; j<4; j++){
                for (int k = 0; k<4; k++){
                    //r.m[i][j] = l[i].x()*t.m[0][j] + l[i].y()*t.m[1][j] + l[i].z()*t.m[2][j] + l[i].w()*t.m[3][j];
                    r.m[i][j] += m[i][k]*t.m[k][j];
                }
            }
        }
        return r;
    }
    vec operator*(const vec& t){
        float rx, ry, rz, rw;

        rx = (get_vector1()).dot_product(t);
        ry = (get_vector2()).dot_product(t);
        rz = (get_vector3()).dot_product(t);
        rw = (get_vector4()).dot_product(t);
        vec r = vec(rx,ry,rz,rw);
        
        return r;
    }
    matrix operator*(float t){
        
        matrix r = matrix();
        for(int i = 0; i< 4; i++){
            for(int j = 0; j<4; j++){
                r.m[i][j]= m[i][j]*t;
            }
        }
        r.m[3][3] = m[3][3];
        
        return r;
    }
    vec multiply3( vec t){
        
        vec s,u,v,w;

        s = get_vector1();
        u = get_vector2();
        v = get_vector3();
        
        vec r = vec(s.x()*t.x()+s.y()*t.y()+s.z()*t.z(), u.x()*t.x()+u.y()*t.y()+u.z()*t.z(),v.x()*t.x()+v.y()*t.y()+v.z()*t.z());
        
        return r;
    }
    // Transpose
    void transpose(){
        vec l[4];
        vec s = get_vector1();
        vec u = get_vector2();
        vec w = get_vector3();
        vec o = get_vector4();
        l[0] = s; l[1] = u;
        l[2] = w; l[3] = o;

        for(int i = 0; i< 4; i++){ // for each vector i.e. row multiplies the cols vectors
            for(int j = 0; j<4; j++){
                m[j][i] = l[i].v[j];
            }
        }
    }
   
    float det(){
        return m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1]) - m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0]) + m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);
    }
    matrix iden(){
        matrix t;
        t.m[0][0] = t.m[1][1] = t.m[2][2] = 1;
        return t;
    }
    matrix inv(){
        float de = det();
        matrix r;
//        matrix identity = r.iden();
        if(de == 0.0){
            return r;
        }
        float d = 1/de;
        r.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * d;
        r.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * d;
        r.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * d;
        r.m[0][3] = 0.0;
        r.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * d;
        r.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * d;
        r.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * d;
        r.m[1][3] = 0.0;
        r.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * d;
        r.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * d;
        r.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * d;
        r.m[2][3] = 0.0;
        r.m[3][0] = -(m[3][0] * r.m[0][0] + m[3][1] * r.m[1][0] + m[3][2]* r.m[2][0]);
        r.m[3][1] = -(m[3][1] * r.m[0][1] + m[3][1] * r.m[1][1] + m[3][2]* r.m[2][1]);
        r.m[3][2] = -(m[3][0] * r.m[0][2] - m[3][1] * r.m[1][2] + m[3][2]* r.m[2][2]);
        r.m[3][3] = 1.0;
        
        
        
        return r;
    }
    // print the matrix
    void print(){
        
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                std::cout << m[i][j] << '\t';
            }
            std::cout << std::endl;
        }
    }
    //  Matrix Transformation
    matrix translate(float tx, float ty,  float tz);
    matrix scale(float sx, float sy, float sz);
    matrix RotateZ( double theta);
    matrix RotateX( double theta);
    matrix RotateY( double theta);
};

#endif /* Matrix_h */
