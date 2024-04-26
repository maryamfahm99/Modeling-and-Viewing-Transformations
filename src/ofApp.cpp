#include "ofApp.h"
#include <cmath>
#include "ray.h"

using std::sqrt;
//--------------------------------------------------------------
void ofApp::setup() {
    
    
    bMousePressed   = false;
    Selected  = false;
    Right = Left = Up = Down = In = Out  = false;
    ScaleUp = ScaleDown = false;
    RotateX = RotateY = RotateZ= false;
    camera_animate = light_animate = false;
    
    // time set up
    clock_t  start, end;
    start = clock();
    
    // ########### Viewpoint Setting ############ //
    
    // The resolution
    w = 640;
    h = 480;
    
    view_pnt = vec(0,0,-1,1);
    light = vec(0,5,0,1);
    float focal_length = 1;
    double pi = M_PI;

    
    upVector = vec(0,1,0,0);
    viewDir = vec(0,0,1,0);
    binormal = upVector.cross_product(viewDir);
    binormal.normalized();
    viewPlnU = (binormal * width);
    upVector.normalized();
    viewPlnV = (upVector * (-height));
    vec delta_u = viewPlnU / w;
    vec delta_v = viewPlnV / h;
    vec up_left = view_pnt + viewDir - (viewPlnU/2) - (viewPlnV/2);
    
    
    binormal.print();
    viewPlnU.print();
    viewPlnV.print();

    first_pixel = up_left + ((delta_u + delta_v)* 0.5);
    //first_pixel.print(); //  the up left pixel
    
    // ########### build objects ############ //


    // plane
    //q = quad(vec(2,-1.4,3,1), vec(2,-1.4,-1,1),vec(-2,-1.4,-1,1),vec(-2,-1.4,3,1),vec(0.5,0.5,0.5,0));
    q = quad(vec(2,-1.4,2,1), vec(2,-1.4,-2,1),vec(-2,-1.4,-2,1),vec(-2,-1.4,2,1),vec(0.5,0.5,0.5,0));
    
    // spheres
    sphere s1 = sphere(vec(0,-0.8,0.15,1),0.3, vec(1,0,0,0));
    sphere s2 = sphere(vec(0,-0.4,1,1),0.7, vec(0,1,0,0));
    sphere s3 = sphere(light, 0.02, vec(1,1,1,0));
    s[0] = s1; s[1] = s2; s[2] = s3;
    
    // ellipsoids
    ellipsoid e1 = ellipsoid(vec(-1.1,-0.55,0.9-0.3,1),0.3, 0.6, 0.3, vec(0.91,0.212,0.349,0),0);
    //ellipsoid e1 = ellipsoid(vec(0,0,1,1),0.3, 0.6, 0.3, vec(0.91,0.212,0.349,0),0);
    ellipsoid e2 = ellipsoid(vec(-0.7,-0.2,0.3,1),0.1, 0.7, 0.2, vec(0,1,0,0),0);
    e[0] = e1; e[1] = e2;
    
    // triangles
    triangle tr1 = triangle(vec(0,1,-1.5,1),vec(-0.5,0,-1.5,1),vec(0.5,0,-1.5,1), vec(0,0,1,0));  // behind  the camera
    triangle tr2 = triangle(vec(0+0.5,0.7,0.7-0.3,1),vec(-0.3+0.5,0,0.7-0.3,1),vec(0.3+0.5,0,0.7-0.3,1), vec(0,0,1,0));
    //triangle tr2 = triangle(vec(0,1.0,0,1),vec(-0.5,-0.1,0,1),vec(0.5,-0.1,0,1), vec(0,0,1,0));
    tr[0] = tr2; tr[1] = tr1;
    
    //cube
    cube c1 = cube(vec(-0.6,-1.15,0.2,1),0.3,0.3,0.3,vec(0.5,0.7,1,0), 0.0);
    c[0] = c1;
    
    colorPixels.allocate(w, h, OF_PIXELS_RGB);
    
    // ########### Iterate over each pixel to find the ray intersection with the objects ############ //
    
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            vec pixel_center = first_pixel + (delta_u * i) + (delta_v * j);
            //compute the ray:
            //view_pnt = pixel_center - vec(0,0,1,0);           // $$$$$$$ If you uncomment => parallel projectio  $$$$$$$
            vec ray_direction = pixel_center - view_pnt;
            ray r =  ray(view_pnt, ray_direction);
            vec pixel_color;
            // find the intersection, closest point
            hittable hit = r.ray_intersect(r,q,s,e,tr,c);
            if ((hit.get_n()).w()!= 1){ // if intersection
            //find intersection for shadow
                vec point = hit.get_point();
                ray r2  =  ray(point, light - point);
                bool shadow  = r2.shadowRay_intersect(r2, point,q,s,e,tr,c);
            //vec color = vec(0.8,0.2,0.2,0);
                       if (shadow == true){ // if intersection
                           pixel_color = r.shadow(hit, pixel_center);
            //pixel_color.print();
                       }else{
                           pixel_color = r.shade(hit, pixel_center, r);
                       }
             }
//           else{
//                 pixel_color = r.background(r);
//             }
            colorPixels.setColor(i, j, ofColor(pixel_color.x(),pixel_color.y(),pixel_color.z()));
        }
    }
    texColor.allocate(colorPixels);
    // End of the code stored previously in the set up:
    
    
    end =  clock();
    
    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "\n\nTime taken by program is : " << fixed << time_taken << setprecision(5);
    std::cout << " sec " << endl;
}



//--------------------------------------------------------------
void ofApp::update() {
    //double t = ofGetElapsedTimeMillis();
    matrix cam;
    double pi = M_PI;
    if (camera_animate){
        //std::cout << "animate" << endl;
        view_pnt = cam.RotateY(pi/12)*view_pnt;
        viewDir  = cam.RotateY(pi/12)*viewDir;
        upVector = cam.RotateY(pi/12)*upVector;
    }if (light_animate){
        if (light == vec(0,5,0,1)){
            light = vec(5,5,0,1);
        }
        std::cout << "animate" << endl;
        light = cam.RotateY(pi/12)*light;
    }

    
    if (!Selected && Up){
        std::cout << "up?" << endl;
        view_pnt = cam.translate(0,0.2,0)*view_pnt;
        Up = false;
    }if (!Selected && Down){
        std::cout << "down?" << endl;
        view_pnt = cam.translate(0,-0.2,0)*view_pnt;
        Down = false;
    }if (!Selected && Right){
        std::cout << "right?" << endl;
        view_pnt = cam.translate(0.2,0,0)*view_pnt;
        Right = false;
    }if (!Selected && Left){
        std::cout << "left?" << endl;
        view_pnt = cam.translate(-0.2,0,0)*view_pnt;
        Left = false;
    }if (!Selected && In){
        std::cout << "Dolly?" << endl;
        view_pnt = cam.translate(0,0,0.2)*view_pnt;
        In = false;
    }if (!Selected && Out){
        std::cout << "out?" << endl;
        view_pnt = cam.translate(0,0,-0.2)*view_pnt;
        Out = false;
    }if (!Selected && RotateX){
        std::cout << "rotate?" << endl;
        view_pnt = cam.RotateX(pi/12)*view_pnt;
        viewDir  = cam.RotateX(pi/12)*viewDir;
        upVector = cam.RotateX(pi/12)*upVector;
        RotateX = false;
    }if (!Selected && RotateY){
        std::cout << "rotate?" << endl;
        view_pnt = cam.RotateY(pi/12)*view_pnt;
        viewDir  = cam.RotateY(pi/12)*viewDir;
        upVector = cam.RotateY(pi/12)*upVector;
        RotateY = false;
    }if (!Selected && RotateZ){
        std::cout << "rotate?" << endl;
        view_pnt = cam.RotateZ(pi/12)*view_pnt;
        viewDir  = cam.RotateZ(pi/12)*viewDir;
        upVector = cam.RotateZ(pi/12)*upVector;
        RotateZ = false;
    }
    
    
    binormal = upVector.cross_product(viewDir);
    binormal.normalized();
    viewPlnU = (binormal * width);
    upVector.normalized();
    viewPlnV = (upVector * (-height));
    
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	texColor.draw(0, 0, w, h);
    
    
    
    matrix cam;
    double pi = M_PI;
    vec delta_u = viewPlnU / w;
    vec delta_v = viewPlnV / h;
    vec up_left = view_pnt + viewDir - (viewPlnU/2) - (viewPlnV/2);
    first_pixel = up_left + ((delta_u + delta_v)* 0.5);
    
    
    hittable hit;
    if(bMousePressed){
        std::cout << "mouse pressed" << endl;
        // Is any object selected?
        vec pixel_center = first_pixel + (delta_u * x) + (delta_v * y);
        vec d = pixel_center - view_pnt;
        ray select = ray(view_pnt, d);
        hit = select.ray_intersect(select,q,s,e,tr,c);
        // Found an object, select it => select mode:
        if (hit.get_t() != -1){
            Selected_obj  = hit.get_clr();
            Selected = true;
            // show it by highlight or change color               ********* in progress *********
        }else{ // Unselect
            Selected = false;
            std::cout << "Unselect" << endl;
        }
    }

    
    // Render:
    colorPixels.allocate(w, h, OF_PIXELS_RGB);
    
    // ########### Iterate over each pixel to find the ray intersection with the objects ############ //
    
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            vec pixel_center = first_pixel + (delta_u * i) + (delta_v * j);
            //compute the ray:
            //view_pnt = pixel_center - vec(0,0,1,0);           // $$$$$$$ If you uncomment => parallel projection  $$$$$$$
            vec ray_direction = pixel_center - view_pnt;
            ray r =  ray(view_pnt, ray_direction);
            vec pixel_color;
            // find the intersection, closest point
            hittable hit = r.ray_intersect(r,q,s,e,tr,c);
            if ((hit.get_n()).w()!= 1){ // if intersection
            //find intersection for shadow
                vec point = hit.get_point();
                ray r2  =  ray(point, light - point);
                bool shadow  = r2.shadowRay_intersect(r2, point,q,s,e,tr,c);
            //vec color = vec(0.8,0.2,0.2,0);
                       if (shadow == true){ // if intersection
                           pixel_color = r.shadow(hit, pixel_center);
            //pixel_color.print();
                       }else{
                           pixel_color = r.shade(hit, pixel_center, r);
                       }
             }
//           else{
//                 pixel_color = r.background(r);
//             }
            colorPixels.setColor(i, j, ofColor(pixel_color.x(),pixel_color.y(),pixel_color.z()));
        }
    }
    texColor.allocate(colorPixels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
        std::cout << "selected and key pressed" << endl;
    matrix m;
    double pi = M_PI;
        switch(key) {
            case OF_KEY_RIGHT:
                Right = true;
                std::cout << "Right" << endl;
                
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(-0.05,0,0);
                    std::cout << "red ball right" << endl;
                    //s[0].transformation.print();
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(-0.05,0,0);
                    e[0].get_transf().print();
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(-0.05,0,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(-0.05,0,0);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(-0.05,0,0);
                }
                
                
                break;
            case OF_KEY_LEFT:
                Left = true;
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(0.05,0,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(0.05,0,0);
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(0.05,0,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(0.05,0,0);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(0.05,0,0);
                }
                break;
            case OF_KEY_UP:
                Up = true;
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(0,-0.05,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(0,-0.05,0);
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(0,-0.05,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(0,-0.05,0);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(0,-0.05,0);
                }
                break;
            case OF_KEY_DOWN:
                Down = true;
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(0,0.05,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(0,0.05,0);
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(0,0.05,0);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(0,0.05,0);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(0,0.05,0);
                }
                break;
            case 'j':
                In = true;
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(0,0,-0.05);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(0,0,-0.05);
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(0,0,-0.05);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(0,0,-0.05);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(0,0,-0.05);
                }
                break;
            case 'k':
                Out = true;
                if(Selected_obj == vec(1,0,0,0)){
                    s[0].transformation = s[0].transformation*m.translate(0,0,0.05);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    e[0].transformation = e[0].transformation*m.translate(0,0,0.05);
                }if(Selected_obj == vec(0,1,0,0)){
                    s[1].transformation = s[1].transformation*m.translate(0,0,0.05);
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    tr[0].transformation = tr[0].transformation*m.translate(0,0,0.05);
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    c[0].transformation = c[0].transformation*m.translate(0,0,0.05);
                }
                break;
            case '1':
                //ScaleUp = true;
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(0.95,1,1))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(0.95,1,1))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(0.95,1,1))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(0.95,1,1))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(0.95,1,1))*trans;                }
                break;
            case '2':
                //ScaleUp = true;
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(1.05,1,1))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(1.05,1,1))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(1.05,1,1))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(1.05,1,1))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(1.05,1,1))*trans;
                }
                break;
            case '3':
                // scale y down
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(1,0.95,1))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(1,0.95,1))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(1,0.95,1))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(1,0.95,1))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(1,0.95,1))*trans;
                }
                
                break;
            case '4':
                // scale y up
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(1,1.05,1))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(1,1.05,1))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(1,1.05,1))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(1,1.05,1))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(1,1.05,1))*trans;
                }
                
                break;
            case '5':
                // scale z down
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(1,1,0.95))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(1,1,0.95))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(1,1,0.95))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(1,1,0.95))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(1,1,0.95))*trans;
                }
                
                break;
            case '6':
                // scale z down
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.scale(1,1,1.05))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.scale(1,1,1.05))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.scale(1,1,1.05))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.scale(1,1,1.05))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.scale(1,1,1.05))*trans;
                }
                break;
            case 'y':
                RotateY = true;
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.RotateY(pi/12))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.RotateY(pi/12))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.RotateY(pi/12))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.RotateY(pi/12))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.RotateY(pi/12))*trans;
                }
                break;
            case 'x':
                RotateX = true;
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.RotateX(pi/12))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.RotateX(pi/12))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.RotateX(pi/12))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.RotateX(pi/12))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.RotateX(pi/12))*trans;
                }
                break;
            case 'z':
                RotateZ = true;
                if(Selected_obj == vec(1,0,0,0)){
                    vec Center = s[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[0].transformation = ((s[0].transformation*inv_trans)*m.RotateZ(pi/12))*trans;
                    //std::cout << "red ball right" << endl;
                }
                if(Selected_obj == vec(0.91,0.212,0.349,0)){
                    vec Center = e[0].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    e[0].transformation = ((e[0].transformation*inv_trans)*m.RotateZ(pi/12))*trans;
                    
                }if(Selected_obj == vec(0,1,0,0)){
                    vec Center = s[1].get_center();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    s[1].transformation = ((s[1].transformation*inv_trans)*m.RotateZ(pi/12))*trans;
                    
                    //std::cout << "red ball right" << endl;
                }if(Selected_obj == vec(0,0,1,0)){
                    vec Center = tr[0].TriCenter();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    tr[0].transformation = ((tr[0].transformation*inv_trans)*m.RotateZ(pi/12))*trans;
                }if(Selected_obj == vec(0.5,0.7,1,0)){
                    vec Center = c[0].get_origin();
                    matrix trans =  m.translate(-Center.x(), -Center.y(), -Center.z());
                    matrix inv_trans =  m.translate(Center.x(), Center.y(), Center.z());
                    c[0].transformation = ((c[0].transformation*inv_trans)*m.RotateZ(pi/12))*trans;
                }
                break;
            case 'a':
                camera_animate = true;
                break;
            case 'l':
                light_animate = true;
                break;

            case 'r':
                view_pnt = vec(0,0,-1,1);
                upVector = vec(0,1,0,0);
                viewDir = vec(0,0,1,0);
                light = vec(0,5,0,1);
                double pi = M_PI;
                // plane
                //q = quad(vec(2,-1.4,3,1), vec(2,-1.4,-1,1),vec(-2,-1.4,-1,1),vec(-2,-1.4,3,1),vec(0.5,0.5,0.5,0));
                q = quad(vec(2,-1.4,2,1), vec(2,-1.4,-1,1),vec(-2,-1.4,-1,1),vec(-2,-1.4,2,1),vec(0.5,0.5,0.5,0));
                
                // spheres
                sphere s1 = sphere(vec(0,-0.8,0.15,1),0.3, vec(1,0,0,0));
                sphere s2 = sphere(vec(0,-0.4,1,1),0.7, vec(0,1,0,0));
                sphere s3 = sphere(light, 0.02, vec(1,1,1,0));
                s[0] = s1; s[1] = s2; s[2] = s3;
                
                // ellipsoids
                ellipsoid e1 = ellipsoid(vec(-1.1,-0.55,0.9-0.3,1),0.3, 0.6, 0.3, vec(0.91,0.212,0.349,0),0);
                ellipsoid e2 = ellipsoid(vec(-0.7,-0.2,0.3,1),0.1, 0.7, 0.2, vec(0,1,0,0),0);
                e[0] = e1; e[1] = e2;
                
                // triangles
                triangle tr1 = triangle(vec(0,1,-1.5,1),vec(-0.5,0,-1.5,1),vec(0.5,0,-1.5,1), vec(0,0,1,0));  // behind  the camera
                triangle tr2 = triangle(vec(0+0.5,0.7,0.7-0.3,1),vec(-0.3+0.5,0,0.7-0.3,1),vec(0.3+0.5,0,0.7-0.3,1), vec(0,0,1,0));
                //triangle tr2 = triangle(vec(0,1.0,0,1),vec(-0.5,-0.1,0,1),vec(0.5,-0.1,0,1), vec(0,0,1,0));
                tr[0] = tr2; tr[1] = tr1;
                
                //cube
                cube c1 = cube(vec(-0.6,-1.15,0.2,1),0.3,0.3,0.3,vec(0.5,0.7,1,0), 0.0);
                c[0] = c1;
                camera_animate = false;
                light_animate = false;
            
                // this resets the camera but not the new positions
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    //Right = Left = Up = Down = In = Out  = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    //save them 
    this ->x = x; this-> y = y;
    std::cout << x << endl;
    bMousePressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    bMousePressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
