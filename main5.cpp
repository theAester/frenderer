#include <list>
#include <limits>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#define clear() printf("\033[H\033[J")
#define cgoto(x,y) printf("\033[%d;%dH", (y), (x))
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

#define XRES 50
#define YRES 32

class ball : public body{
	public:
	double rad;
	ball(vec3<double> pos, vec3<double> angle, double radius) : body(pos, angle, 0, 2){
		this->rad = radius;
	}
	double intersect(vec3<double> o, dir3<double> d){ // override with solved equation to achieve higher speed
		o = o-pos;
		double aa = d*d;
		double bb = o*d*2;
		double cc = o*o - rad*rad;
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
		return dir3<double>(inter-pos);
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.8,0.8,0.8);
	}
	cplx surf_func(vec3<cplx> r){
		return cplx(0,0);
	}
	vec3<double> shader(scene* sc, double dist, dir3<double> dir, int bounce){
		return lambert_shader(sc,dist,dir);	
	}
};

class donut : public body{
	public:
	double rr,RR;
	donut(vec3<double> pos, vec3<double> angle, double sradius, double bradius,bool hitbox = false, double a = 0, double b = 0, double c =0) : body(pos, angle, 0, 4, hitbox,a,b,c){
		rr = sradius;
		RR = bradius;
	}
	vec3<double> diffuse_color(vec3<double> inter){
		return vec3<double>(0.8,0.8,0.8);
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

char chars[] = {'$','@','B','&','%','W','#','k','d','q','m','O','0','L','l','i','/','\\','|','(',')','1','{','}','[',']','?','<','>',';',':','^','+','~','-','\'','`','.',' '};  // more detail less contrast
 
//char chars[] = {' ','.',':','-','=','+','*','#','%','@'}; // less detail more contrast

int seg(unsigned int i){
	const int len = 255;
	const int breadth = sizeof(chars);
	const int q = len/breadth;
	const int r = len%breadth;
	int a = 0;
	for(int j=0;j<=breadth;j++){
		if(j<r)
			a+=q+1;
		else
			a+=q;
		if(a>i)
			return j;
	}
	return 0;
}

int main(int argc, char* argv[]){

	camera cam(vec3<double>(-4, 5, 1), vec3<double>(-0.7,0.15,0), 72.6, 1); 
	scene sc(XRES,YRES,cam,vec3<double>(0.1,0.1,0.1));

	donut* don = new donut(vec3<double>(0,0,0), vec3<double>(M_PI/2,0,0), 0.6,2,true,3.5,3.5,1.4);
	ball* bal = new ball(vec3<double>(0,0,0), vec3<double>(0,0,0), 0.7);

	sc.bodies.push_back(don);
	sc.bodies.push_back(bal);

	light* lt = new light(vec3<double>(-4,2,6), vec3<double>(1,1,1));

	sc.lights.push_back(lt);

	int pitch = XRES;
	unsigned char* screen_buff = new unsigned char[XRES*YRES];
	double c=0;
	while(true){
		clear();
		cgoto(0,0);
		sc.render( (void*) screen_buff,pitch, true);
		for(int i=0;i<YRES;i++){
			for(int j=0;j<XRES;j++){
				printf("%c ",chars[ sizeof(chars)-1-seg( (unsigned int)screen_buff[i*XRES + j] + 2 ) ]);
			}
			printf("\n");
		}
		c -= 2*M_PI/50;
		don->change_angle(vec3<double>(M_PI/2,c,0));
		bal->pos = vec3<double>(0,3.4*sin(-c),0);
	}
	return 0;
}
