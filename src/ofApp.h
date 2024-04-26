#pragma once

#include "ofMain.h"
#include "vec.h"
#include "ray.h"
#include "Matrix.h"
#include "sphere.h"
#include "ellipsoid.h"
#include "plane.h"
#include "hittable.h"
#include <chrono>

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
		ofTexture 		texColor;
    
        // Set Up
        float width = 4.0; //  [-2, 2]
        float height= 3.0; // [-1.5, 1.5]
        vec viewDir, upVector, binormal;
        vec viewPlnU;
        vec viewPlnV;
        vec view_pnt;
        vec first_pixel;
        vec light;
		int 			w, h;
        int             x, y;
    
		ofPixels colorPixels;
        bool bMousePressed;
        bool Selected;
        bool Right, Left, Up, Down, In, Out;
        bool ScaleUp, ScaleDown;
        bool RotateX, RotateY, RotateZ;
        vec Selected_obj; // defined by its color
        bool camera_animate, light_animate;
    
        // Objects:
        quad q;
        sphere s[4];
        ellipsoid e[4];
        triangle tr[4];
        cube c[4];
  
};
