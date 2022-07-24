#include <list>
#include <SDL2/SDL.h>
#include <limits>
#include <complex>
#define inf numeric_limits<double>::max()
using namespace std;
using cplx = complex<double>;
#include "common/vec3.h"
#include "common/dir3.h"
#include "object/object.h"
#include "object/camera.h"
#include "object/light.h"
#include "object/body.h"
#include "scene/scene.h"

#define XRES 100
#define YRES 70

class sphere : public body{
	public:
	double rad;
	sphere(vec3<double> pos, vec3<double> angle, double radius):body(pos, angle,0,2){
		this->rad = radius;
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.6,0.6,0.6);
	}
	cplx surf_func(vec3<cplx> r){
		return r.x*r.x + r.y*r.y + r.z*r.z - rad*rad;
	}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return lambert_shader(sc,dist,dir);
	}
};
class spskew : public body{
	public:
	double a,b,c;
	spskew(vec3<double> pos, vec3<double> angle,double a, double b, double c) : body(pos,angle,0,2) {
		this->a = a;
		this->b = b;
		this->c = c;
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.6,0.6,0.6);
	}
	cplx surf_func(vec3<cplx> r){
		return (r.x*r.x)/a + (r.y*r.y)/b + (r.z*r.z)/c - 1.0;
	}
	double intersect(vec3<double> o, dir3<double> d){
		vec3<double> ooo = TG(o-pos);
		vec3<double> ddd = TG(d);
		vec3<double> oo((ooo.x)/a,(ooo.y)/b,(ooo.z)/c);
		vec3<double> dd(ddd.x/a,ddd.y/b,ddd.z/c);
		double aa = dd*dd;
		double bb = oo*dd*2;
		double cc = oo*oo - 1;
		double k = bb*bb - 4*aa*cc; 
		if(k > 0){
			double x1 = (sqrt(k) - bb)/(2*aa);
			double x2 = -1*(sqrt(k) + bb)/(2*aa);
			if(x2>0) return x2;
			if(x1>0) return x1;
		}
		return inf;
	}
	dir3<double> get_normal(vec3<double> inter){
		return dir3<double>(2*(inter.x - pos.x)/(a*a),2*(inter.y - pos.y)/(b*b),2*(inter.z - pos.z)/(c*c));
	}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return lambert_shader(sc,dist,dir);	
	}
};
class spflat : public sphere{
	public:
	spflat(vec3<double> pos, vec3<double> angle, double radius):sphere(pos,angle,radius) {}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return vec3<double>(0.7,0,0);
	}
};

class donut : public body{
	public:
	double rr,RR;
	donut(vec3<double> pos, vec3<double> angle, double sradius, double bradius) : body(pos, angle, 0, 4,
												true, 3.5,3.5,1.4){
		rr = sradius;
		RR = bradius;
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.6,0.6,0.3);
	}
	cplx surf_func(vec3<cplx> r){
		cplx x2 = r.x*r.x;
		cplx y2 = r.y*r.y;
		double R2 = RR*RR;
		cplx t1 = (x2 + y2 + r.z*r.z + R2 - rr*rr);
		return t1*t1 - 4*R2*(x2+y2);
	}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return lambert_shader(sc,dist,dir);	
	}
};


int main(int argc, char* argv[]){
	SDL_Renderer* renderer;
	SDL_Window* window;

	uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN;
	uint32_t SDL_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER;

	SDL_Init(SDL_FLAGS);
	window = SDL_CreateWindow("bruh", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, XRES,YRES, WINDOW_FLAGS);
	renderer = SDL_CreateRenderer(window,-1,0);
	SDL_Event e;
	SDL_PollEvent(&e);

	camera cam(vec3<double>(-5, 0, 3), vec3<double>(-0.09,0.23,0), 72.6, 1); 
	scene sc(XRES,YRES,cam,vec3<double>(0.1,0.1,0.1));

	//sphere* sp = new sphere(vec3<double>(3,1.2,1.3), vec3<double>(0,0,0), 1);
	//sphere* sp2 = new sphere(vec3<double>(2.5,-1.2,0.4), vec3<double>(0,0,0), 1;
	//spskew* bound = new spskew(vec3<double>(3,0,0), vec3<double>(1.2,0,0),3.5,3.5,1.4);
	donut* don = new donut(vec3<double>(3,0,0), vec3<double>(1.2,0,0), 0.7,2.5);

	sc.bodies.push_back(don);
	//sc.bodies.push_back(bound);
	//sc.bodies.push_back(sp);
	//sc.bodies.push_back(sp2);

	light* lt = new light(vec3<double>(0,0,6), vec3<double>(0.7,0.7,0.7));
	//light* lt2 = new light(vec3<double>(7,0,6), vec3<double>(0.24,0.24,0.24));

	sc.lights.push_back(lt);
	//sc.lights.push_back(lt2);
	int pitch;

	void* screen_buff;
	SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, XRES,YRES);
	double c=0;
	while(e.type != SDL_QUIT){
		SDL_PollEvent(&e);

		SDL_LockTexture(tex,NULL,&screen_buff,&pitch);
		sc.render(screen_buff,pitch);
		SDL_UnlockTexture(tex);

		SDL_RenderCopy(renderer,tex,NULL,NULL);

		SDL_RenderPresent(renderer);
		//sp->pos.y -= 0.1;
		//sp->pos.x += 0.3;
		c -= 0.08;
		don->change_angle(vec3<double>(1.2,c,c));
		SDL_Delay(50);
	}
	return 0;
}
