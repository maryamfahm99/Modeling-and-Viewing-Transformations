# CS248fall2023 - HW1
# Maryam Almaskin

This Project is about ray tracing. Inputs are objects such spheres, ellipsoids, and boxes.
There is no need to install any libraries, however, to run the code, you need to OpenFramework set up and include my files in the source code.

The file that includes the main function is the file: main.cpp
vec.h and matrix.h are the essential files to create the objects in sphere.h and plane.h and ellipsoid.h

Most of the executed and heavy code is in the ray.cpp file. Here you can find the calculations for the intersections, shading, shadow, and coloring. 

Notes: 
  1. plane.h includes three classes: triangle, quad, cube
  2. If you execute the code, you are going to see the perspective projection. uncomment the line 73 in ofApp.cpp to and run it again to see the parallel projection.
  
#3. Performance Analysis: Time taken by program is : 0.974978 sec. 

  4.  For extra credit, this code implements the phong shading, boxes and triangles as objects.


I acknowledge that I used online resources such as youtube, lectures, ChatGPT, friends to learn and understand better. I implemented what I understood and took what benefited me and worked for me the best.

