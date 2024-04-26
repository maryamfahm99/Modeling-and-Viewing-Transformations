//
//  ray.cpp
//  template
//
//  Created by memo2 on 9/20/23.
//

#include <stdio.h>
#include "ray.h"

// ############### These functions find the intersection between the ray and the objects #############


float ray::sphr_intersect(vec& o, vec& d, sphere& s){
    
//    vec O, dir;
////    O = s.get_transf()*o;
////    dir = s.get_transf()*d;
//    O = o;
//    dir = d;
    
    vec center =  s.get_center(); float radius = s.get_radius();
    float a = d.dot_product(d); // b*b
    float b = (d.dot_product(o-center))*2.0;  // 2*b*(A-C)
    float c  = (o-center).dot_product(o-center) - (radius*radius);
    float discrim = (b*b) - (4*(a*c));
    if (discrim < 0.0){
        return -1.0;
    }else{
        // find the minimum t
        float t0 = (-b+sqrt(discrim))/(2.0*a);
        float t1 = (-b-sqrt(discrim))/(2.0*a);
        if( t0 < t1 ){
            return t0;
        }else{
            return t1;
        }
    }
}

float ray::triangle_intersect(ray& r, triangle& pln){
    
//    vec O, dir;
//    O = pln.get_transf()*r.get_origin();
//    dir = pln.get_transf()*r.get_dir();
//    r = ray(O,dir);
    
    // a,b,c are the points of the triangle
    vec a = pln.get_a(); vec b = pln.get_b(); vec c = pln.get_c();
    // o,d are the origin and direction of the ray
    vec o = r.get_origin(); vec d = r.get_dir();
    
    // ray intersection with the plane
    vec N = ((a-b).cross_product(c-b));
    N.normalized();
    if ((d.dot_product(N)) != 0){
        float t0 = ((b - o).dot_product(N))/(d.dot_product(N));
        // ray tracing with the triangle
        vec v1 = a - b;
        vec v2 = a - c;
        vec v3 = a - o;
        vec v4 = vec(0,0,0,1);
        
        matrix m3 = matrix(v1,v2,v3,v4);
        matrix m4 = matrix(v1,v2,d,v4);
        m3.transpose(); m4.transpose();
            
        matrix m2 = matrix(v1,v3,d,v4);
        m2.transpose();
        float gamma = m2.det()/m4.det();
        if (gamma < 0 || gamma > 1){
            return -1;
        }else{
            matrix m1 = matrix(v3,v2,d,v4);
            m1.transpose();
            float beta = m1.det()/m4.det();
            if(beta < 0 || beta > (1 - gamma)){
                return -1;
            }
        }
        return t0;
    }
    return -1;
    
}

float ray::quad_intersect(ray& r, quad& q){
    vec n = ((q.get_b()-q.get_a()).cross_product(q.get_c()-q.get_a()));
    n.normalized();
    
    vec O, dir;
    O = q.get_transf()*r.get_origin();
    dir = q.get_transf()*r.get_dir();
    
    r = ray(O,dir);
    
    if ((n.dot_product(r.get_dir())) != 0 ){ // plane intersection
        //std::cout << "pln int" <<endl;
        float t = ((q.get_a() - r.get_origin()).dot_product(n))/(n.dot_product(r.get_dir()));
        
        //project on xz plane
        vec p = r.t_position(t);
        float x = p.x();
        float z = p.z();
        p = vec(x,0,z,1);
        
        x = r.get_dir().x();
        z = r.get_dir().z();
        vec d = vec(x,0,z,1);
        
        int num = 0;
        vec p1,p2;
        
        vec qd[4];
        qd[0] = q.get_a();
        qd[1] = q.get_b();
        qd[2] = q.get_c();
        qd[3] = q.get_d();
        
        for (int i = 0; i < 4; i++){
            p1 = qd[i];
            p2 = qd[(i+1) % 4];
            
            if (( p1.z() > p.z() && p2.z() <= p.z()) || ( p2.z() > p.z() && p1.z() <= p.z()) ){
                float t0 = (p.z()-p1.z())/(p2.z()-p1.z());
                x = p1.x() + t0*(p2.x()-p1.x());

                if (x > p.x()){
                    num = num + 1;
                }
            }
        }
        if (num % 2 == 1){
            return t;
        }
    }
    return -1;
}

float ray::ellips_intersect(ray& r, ellipsoid& e){
     
    
    vec O, dir;
    O = r.get_origin();
    dir = r.get_dir();
    

    
    vec center = e.get_center(); float axis1 = e.get_a();
    float axis2 = e.get_b(); float axis3 = e.get_c();
    vec scaled_dir = vec(dir.x()/axis1, dir.y()/axis2, dir.z()/axis3);
    
    float a = scaled_dir.dot_product(scaled_dir);
    float b = ((vec(scaled_dir.x()/axis1, scaled_dir.y()/axis2, scaled_dir.z()/axis3)).dot_product(O-center))*2.0;
    float c = (O.x()*O.x() - 2*center.x()*O.x() + center.x()*center.x())/(axis1*axis1) +
              (O.y()*O.y() - 2*center.y()*O.y() + center.y()*center.y())/(axis2*axis2) +
              (O.z()*O.z() - 2*center.z()*O.z() + center.z()*center.z())/(axis3*axis3) -  1;
    
    
    float discrim = (b*b) - (4*(a*c));
    if (discrim < 0.0){
        return -1.0;
    }else{
        // find the minimum t
        float t0 = (-b+sqrt(discrim))/(2.0*a);
        float t1 = (-b-sqrt(discrim))/(2.0*a);
        if( t0 < t1 ){
            return t0;
        }else{
            return t1;
        }
    }
}

hittable ray::cube_intersect(ray& r, cube& c){
    
    vec o, d;
//    o = c.get_transf()*r.get_origin();
//    d = c.get_transf()*r.get_dir();
    o =  r.get_origin();
    d = r.get_dir();
    
    float halfSize = c.get_w() * 0.5;
    //the min and max
    float x1 = (c.get_origin()).x() - halfSize; float x2 = (c.get_origin()).x() + halfSize;
    float y1 = (c.get_origin()).y() - halfSize; float y2 = (c.get_origin()).y() + halfSize;
    float z1 = (c.get_origin()).z() - halfSize; float z2 = (c.get_origin()).z() + halfSize;
    float tx1, tx2, ty1, ty2, tz1, tz2;
    // t for each face
    tx1 = (x1 - o.x()) / d.x(); tx2 = (x2 - o.x()) / d.x();
    ty1 = (y1 - o.y()) / d.y(); ty2 = (y2 - o.y()) / d.y();
    tz1 = (z1 - o.z()) / d.z(); tz2 = (z2 - o.z()) / d.z();

    float tmin = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min (tz1, tz2));
    float tmax = std::min(std::min(std::max(tx1, tx2), std::max(ty1,ty2)), std::max(tz1, tz2));
    vec normal; float t;
    hittable hit;
    // Check if the ray intersects the cube
    if (tmin <= tmax && tmax >= 0) {
        // Calculate the intersection point
        vec intersectionPoint = o + (d * tmin);
        // Determine which face of the cube was hit and calculate the normal
        if (tmin == tx1) {
            normal = vec(-1,0,0,0);
        } else if (tmin == tx2) {
            normal = vec(1,0,0,0);
        } else if (tmin == ty1) {
            normal = vec(0,-1,0,0);
        } else if (tmin == ty2) {
            normal = vec(0,1,0,0);
        } else if (tmin == tz1) {
            normal = vec(0,0,-1,0) ;
        } else {
            normal = vec(0,0,1,0);
        }
        matrix trns = c.get_transf();
        trns.transpose();
        normal = trns.multiply3(normal);
        t = tmin;
        hit = hittable(r.t_position(t),normal,t, c.get_clr(),c.get_theta());
    }
        return hit;
}

// ############### These functions find the closest intersection for the shading & shadow and computes the final color #############


hittable ray::ray_intersect(ray& r,quad& q, sphere s[4], ellipsoid e[4], triangle tr[4], cube c[4]){
    
    vec direction = r.get_dir();
    vec origin = r.get_origin();
    float distance = std::numeric_limits<float>::infinity();
    hittable hit;
    
    //QuadPlane:
    float qd_inter = quad_intersect(r, q);
    if ( qd_inter > 0.0){
        vec point3 = r.t_position(qd_inter);
        vec n = ((q.get_b()-q.get_a()).cross_product(q.get_c()-q.get_a()));
        n.normalized();
        if( (point3-origin).length() < distance){
            distance = (point3-origin).length();
            hit = hittable(point3,n*(-1),qd_inter,q.get_clr(),0.0);
            //(q.get_clr()).print();
        }
        //n.print();
    }
    
    float t;
    // spheres
    sphere s_temp;
    for (int i=0; i<0; i++){
        s_temp = s[i];
        
        vec O, dir;
        O = s_temp.get_transf()*r.get_origin();
        dir = s_temp.get_transf()*r.get_dir();
        ray r2 =  ray(O,dir);
        t = sphr_intersect(O, dir, s_temp);
        if (t > 0.0){
            //std::cout << t<< endl;
            vec point1 = r2.t_position(t);
            vec n = point1 - s_temp.get_center();
            matrix trns = s_temp.get_transf();
            trns.transpose();
            n = trns.multiply3(n);
            if( (point1-origin).length() < distance){
                //std::cout << t<< endl;
                distance = (point1-origin).length();
                hit = hittable(point1,n,t,s_temp.get_clr(),0.0);
                //s_temp.get_clr().print();
            }
        }
    }
   
    // ellipsoids
    ellipsoid e_temp;
    for (int i=0; i<1; i++){
        e_temp = e[i];
        vec O, dir;
        //e_temp.get_transf().print();
        O = e_temp.get_transf()*r.get_origin();
        dir = e_temp.get_transf()*r.get_dir();
        ray r2 =  ray(O,dir);
        t = ellips_intersect(r2,e_temp);
        if (t > 0.0){
            vec point2 = r2.t_position(t);

            vec axis = vec(1/pow(e_temp.get_a(),2),1/pow(e_temp.get_b(),2),1/pow(e_temp.get_c(),2),0);
            vec n = ((point2-e_temp.get_center())*2).wise(axis);
            matrix trns = e_temp.get_transf();
            trns.transpose();
            //std::cout << "normals in ellips" << endl;
            //n.print();
            vec n2 = trns.multiply3(n);
//            if (n == n2){
//                int i = 0;
//                //trns.print();
//            }else{
//                //n.print();
//                //n2.print();
//            }
            //n.print();
            if( (point2-O).length() < distance){
                distance = (point2-O).length();
                hit = hittable(point2,n2,t,e_temp.get_clr(),0);
                hit.transformation = e_temp.get_transf();
            }
        }
    }
    // triangles
    triangle tr_temp;
    for (int i=0; i<1; i++){
        tr_temp = tr[i];
        vec O, dir;
        O = tr_temp.get_transf()*r.get_origin();
        dir = tr_temp.get_transf()*r.get_dir();
        ray r2 = ray(O,dir);
        t = triangle_intersect(r2, tr_temp);
        if ( t > 0.0){
            //std::cout << t<< endl;
            vec point3 = r2.t_position(t);
            // a,b,c are the points of the triangle
            vec a = tr_temp.get_a(); vec b = tr_temp.get_b(); vec c = tr_temp.get_c();
            vec n = ((a-b).cross_product(c-b)); //*** This N is constant and thus shade is same? I dont think so but there is smthg off
            matrix trns = tr_temp.get_transf();
            trns.transpose();
            n = trns.multiply3(n);
            if( (point3-origin).length() < distance){
                distance = (point3-origin).length();
                hit = hittable(point3,n,t,tr_temp.get_clr(),0.0);
                hit.transformation = tr_temp.get_transf();
            }
        }
    }
    // cube
    hittable c_temp;
    for (int i=0; i<1; i++){
        vec o, d;
        o = c[i].get_transf()*r.get_origin();
        d = c[i].get_transf()*r.get_dir();
        ray r2 = ray(o,d);
        c_temp = cube_intersect(r2, c[i]);
        if ( c_temp.get_t() > 0.0){
            hit = cube_intersect(r2, c[i]);
            hit.transformation = c[i].get_transf();
        }
    }
    return hit;
}

bool ray::shadowRay_intersect(ray& r,vec& point,quad& q, sphere  s[4], ellipsoid e[4], triangle tr[4], cube c[4]){
    vec direction = r.get_dir();
    vec origin = r.get_origin();
    float t;
    
    // spheres
    for (int i=0; i<0; i++){
//        vec O, dir;
//        O = s[i].get_transf()*r.get_origin();
//        dir = s[i].get_transf()*r.get_dir();
//        ray r2 =  ray(O,dir);
        t = sphr_intersect(origin, direction, s[i]);
        if (t > 0.0){
            vec point1 = r.t_position(t);
            if (s[i].point_on_sphere(point1) == true && s[i].point_on_sphere(point) ==  true){
                return false;
            }
            return true;
        }
    }
    // ellipsoids
    for (int i=0; i<1; i++){
        vec O, dir;
        O = e[i].get_transf()*r.get_origin();
        dir = e[i].get_transf()*r.get_dir();
        ray r2 =  ray(O,dir);
        t = ellips_intersect(r2,e[i]);
        if (t > 0.0){
            vec point2 = r2.t_position(t);
            if (e[i].point_on_ellipsoid(point2) == true && e[i].point_on_ellipsoid(point) == true){
                return false;
            }
            //j = -1;
            return true;
        }
    }
    // triangles
    for (int i=0; i<1; i++){
        vec O, dir;
        O = tr[i].get_transf()*r.get_origin();
        dir = tr[i].get_transf()*r.get_dir();
        ray r2 =  ray(O,dir);
        t = triangle_intersect(r2, tr[i]);
        if ( t > 0.0){
            return true;
        }
    }
    //cube
    hittable hit_cube;
    for (int i=0; i<1; i++){
        vec O, dir;
        O = c[i].get_transf()*r.get_origin();
        dir = c[i].get_transf()*r.get_dir();
        ray r2 =  ray(O,dir);
        hit_cube = cube_intersect(r2, c[i]);
        if ( hit_cube.get_t() > 0.0){
            return true;
        }
    }
    //QuadPlane    // In Our senario, this block is not needed since the ray from the object to the light does not intersect the plane.
    /*
    float qd_inter = quad_intersect(r, q);
    if ( qd_inter > 0.0){
        vec point1 = r.t_position(qd_inter);
        if (q.point_on_plane(point1) == true && q.point_on_plane(point) ==  true){
        }
        //return true;
    }
     */
    
    return false;
}

vec ray::shade(hittable& hit, vec& pixel, ray&r){
    vec color = hit.get_clr();
    //color.print();
    vec n = hit.get_n();
    vec point = hit.get_point();
    float t = hit.get_t();
    
    
    vec d = r.get_dir();
    d = hit.get_transf()* d;
    vec light = vec(0,5,0,1);              // position of the light
    
    n.normalized();
    vec lightDir = light - point;
    lightDir.normalized();
    vec L = vec(0,0,0,0);                  // L is going to be the final color
    vec h = d*(-1)+ lightDir;
    h.normalized();
    vec I = vec(1,1,1,0);

    float lamb = n.dot_product(lightDir);
    if (lamb > 0){                        //lambertian
        L = L + color*(lamb*0.6);
    }
    float phong = n.dot_product(h);
    if ( phong > 0){                      //phong
        L = L + I*(pow(phong,32.0)*0.2);
    }
    L = L + color*0.2;                    // ambient

    L = (L)*255;

    float x = L.x();
    float y = L.y();
    float z = L.z();

    if (x > 255){     // if value is more than 255, set it to 255
        x = 255;
    }
    if (y > 255){
        y = 255;
    }
    if (z > 255){
        z = 255;
    }
    L = vec(x,y,z);

    return L;
    
}
vec ray::shadow(hittable& hit,vec& pixel){
    vec color = hit.get_clr();
    vec I = vec(1,1,1,0);
    vec L = color*0.2;    // Ambient
    L = (L)*255;
    
    float x = L.x();
    float y = L.y();
    float z = L.z();

    if (x > 255){
        x = 255;
    }
    if (y > 255){
        y = 255;
    }
    if (z > 255){
        z = 255;
    }
    L = vec(x,y,z);
    return L;
}

vec ray::background(ray &r){
    vec dir = r.get_dir();
    dir.normalized();
    float a = 0.5*(dir.y() + 1)*255;
    return (vec(1.0, 1.0, 1.0, 0)*(a)) + (vec(0.5,0.7,1.0,0)*(255-a));
}




