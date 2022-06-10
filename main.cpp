#include <list>
#include <iostream>
#include <math.h>
#include <complex>
#include <SDL2/SDL.h>
using namespace std;
using cplx = complex<double>;
#include "common/comm.h"
#include "object/object.h"
#include "camera/camera.h"
#include "object/light.h"
#include "object/body.h"
#include "scene/scene.h"

#define XRES 30
#define YRES 20

class sphere : public body{
	public:
	double rad;
	sphere(vec3<double> pos, vec3<double> angle, double radius,double reflectivify):body(pos, angle,reflectivity){
		this->rad = radius;
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.6,0.6,0.6);
	}
	cplx surf_func(vec3<cplx> r){
		return r.x*r.x + r.y*r.y + r.z*r.z - rad*rad;
	}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return lambert_shader(sc,dist,dir,bounce,2);
	}
};


int main(int argc, char* argv[]){
	SDL_Renderer* renderer;
	SDL_Window* window;

	uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN;
	uint32_t SDL_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER;

	SDL_Init(SDL_FLAGS);
	window = SDL_CreateWindow("bruh", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, XRES,YRES, WINDOW_FLAGS);

	SDL_Event e;
	SDL_PollEvent(&e);

	camera cam(vec3<double>(-5, 0, 3), vec3<double>(0,0,0), 72.6, 1); 
	scene sc(XRES,YRES,cam,vec3<double>(0.1,0.1,0.1));

	sphere* sp = new sphere(vec3<double>(3,1,0), vec3<double>(0,0,0), 2,0);

	sc.bodies.push_back(sp);

	light* lt = new light(vec3<double>(0,0,6), vec3<double>(0.7,0.7,0.7));

	sc.lights.push_back(lt);

	uint32_t* screen_buff = new uint32_t[YRES*XRES];

	SDL_Surface* surf = SDL_GetWindowSurface(window);
	//SDL_Surface* surf;
	while(e.type != SDL_QUIT){
		SDL_PollEvent(&e);
		
		sc.render(screen_buff);

		surf = SDL_CreateRGBSurfaceFrom((void*)screen_buff, XRES,YRES,32,YRES*4,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);

		SDL_UpdateWindowSurface(window);

		SDL_Delay(200);
	}
	return 0;
}
