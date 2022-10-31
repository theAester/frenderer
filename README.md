# F(unction) Renderer
tiny little c++ renderer.

if you can provide a collision function for it, frenderer can render it.

# compiling

``` bash
make [filename]
```

for example use
``` bash
make main2
```
to compile main.cpp to build/main2

# Examples
main.cpp : render to an actual image. The only example of this type. Uses SDL2 for 2d graphics. The rest of the examples render to terminal(add libSDL2 to linker)

main2.cpp: reders two nested donuts spinning about the z and the y axis.

main3.cpp: a huge fucking donut

main4.cpp: two huge fucking donuts

main5.cpp: ball going through spinning donut.

# How to use
 1. derive your mathematical collision function. A shape is described by the collision function f: R3 -> R when f(<b>r</b>)=0 IIF <b>r</b> is on the shape
 2. derive a new class from the body class and override ``` cplx surf_func (vec3<cplx> r) ``` to be your collision function. cplx is complx number datatype.
 3. you also need to override the ``` diffuse_color``` and ```shader```
   ``` cpp
    class donut : public body{
      public:
      double sr,br;
      donut(vec3<double> position, vec3<double> angle, double small_radius, double big_radius) : body(position, angle, 0, 4){
                                          //     ^                                                                     ^  ^
                                          //  euler angles. extrinsic rotation performed                    reflectivity  degree of collision function(IMPORTANT)
        sr=small_radius;
        br=big_radius;
      }
      cplx surf_func(vec3<double> r){ // z^2 + (R-sqrt(x^2+y^2))^2 = r^2
          cplx x2 = r.x*r.x;
          cplx y2 = r.y*r.y;
          double R2 = RR*RR;
          cplx t1 = (x2 + y2 + r.z*r.z + R2 - rr*rr);
          return t1*t1 - 4*R2*(x2+y2); // note that its best to play around with the collision function to turn it into a polynomial. or atleast something similar
      }
      vec3<double> diffuse_color(vec3<double> intersection_point){
  //    ^
  // color in (r,g,b) format where 0 < r,g,b < 1. if any of the color componenets fall outside the specified ranged they will be clipped to the closest extreme.
        return vec3<double>(0.5,0.5,0.5); // the color-map is a dull grey everywhere
        //you can do this or define your own colormap
      }
      vec3<double> shader(scene* sc, double distance, dir3<double> projection_direction, int bounce){
      // ^                                                                                   ^
      //final color returned to caller function                                       for reflectivity
      return lambert_shader(scene, distance, projection_direction, bounce); // the built in lambert shader. doesnt support reflections
      // you can do this and/or implement your own shader
    };
  ```
 4. in the main proc. instantiate a scene, a camera and atleast one light source and add the camera and lights to your scene
  ``` cpp
    int main(int argc, char* argv[]){
      camera cam(vec3<double>(-6,0,0),vec3<double>(0,0,0),72.6, 1);
      //           ^                          ^             ^   ^
      //        position           angle(euler-extrinsic)  fov  projection screen distance(too large distances cause glitchs, too small ones cause floating point errors, choose accordingly)
      scene sc(72, 45, cam, vec3<double>(0.1,0.1,0.1));
      //         ^      ^                     ^
      //   resolution|camera to project to|ambient light
      light* lt = new light(vec3<double>(-5,-3,4), vec3<double>(1,1,1));
      //                                 ^                      ^
      //                              position                color
      light* lt2 = new light(vec3<double>(-7,3,2), vec3<double>(0.2,0.2,0.2));
      
      sc.lights.push_back(lt);
      sc.lights.push_back(lt2);
      
      donut don(vec3<double>(0,-5,0), vec3<double>(1.1,0,0), 1.8,2.8);
      
      sc.bodies.push_back(don);
      }
  ```
  5. define a screen buffer based on your needs. if you need to output to terminal a 2d array of unsigned chars will do
  6. call ```scene::render``` and pass your screen buffer, pitch(number of bytes per each row) to it. set the third argument to true to render monochrome;
  9. create a main loop and do with the filled screen buffer as you wish.

refer to example 2 and 5 for better understanding.

# functionality
the renderer uses raytracing to specify the color of each pixel. it will try to NUMERICALLY solve the collision equation for each ray which is very time consuming.
the durand-kerner algorithm is used for this matter and this is why you have to specify a degree when initializing a body.
the algorithm basically takes in the degree and through iteration finds exactly as many roots and then separates the real ones and returns the minimum.

the algorithm also tries to calculate the normal vector at each collision point numerically.

since numeric calculations can be time consuming two actions are possible:
 1. You can override the ```body::intersect``` function which basically intersects a line with your shape. it returns inf if there is no collision and the distance from the origin to the collision point if there is.
     so if your collision equation, f(<b>r</b>) = 0, is mathematically soluable, its best to override the intersect function to calculate the distance using a formula
     you can also override the ```body::get_normal``` function to directly calculate the expression for grad(f)
     
 2. you can enable the  <b>hitbox</b> feature for your classes by using the optional arguments in the body initializer<b>(see example 3)</b>.
    a hitbox is basically an ellisoid surrounding the shape but you have to specify the length of its radii so it encapsulated your entire shape.
    then the intersection function will use a formula to check for collisions with the hitbox before starting the numeric solver which can save a lot of computational power.
    
# Notes:
Code reviews and critisism is appreciated. You can email through hiradcode@yahoo.com

The current release relies on cpu and main memory to do all the calculations and hence is a little slow. A greate TODO is to leave some of the calculations 
to the GPU which would be a huge speed up.
