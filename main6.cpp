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

#define XRES 72
#define YRES 45

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
	for(int j=0;j<breadth;j++){
		if(j<r)
			a+=q+1;
		else
			a+=q;
		if(a>i)
			return j;
	}
	return breadth-1;
}

int main(int argc, char* argv[]){
	camera cam(vec3<double>(-10, 0, 0), vec3<double>(0,0,0), 72.6, 1); 
	scene sc(XRES,YRES,cam,vec3<double>(0.1,0.1,0.1));

	donut* don = new donut(vec3<double>(0,0,0), vec3<double>(1.1,0,0), 1.8,2.8,true,4.4,4.4,2.4);

	sc.bodies.push_back(don);

	light* lt = new light(vec3<double>(-5,-3,4), vec3<double>(1,1,1));
	light* lt2 = new light(vec3<double>(-7,3,2), vec3<double>(0.2,0.2,0.2));

	sc.lights.push_back(lt);
	sc.lights.push_back(lt2);

	int pitch = XRES;
	unsigned char* screen_buff = new unsigned char[XRES*YRES];
	double c=0;
	while(true){
		clear();
		cgoto(0,0);
		sc.render( (void*) screen_buff,pitch, true);
		for(int i=0;i<YRES;i++){
			for(int j=0;j<XRES;j++){
				printf("%c ",chars[ sizeof(chars)-1-seg( (unsigned int)screen_buff[i*XRES + j] ) ]);
				//printf("%02d ",sizeof(chars)-seg( (unsigned int)screen_buff[i*XRES + j]));
				//printf("%03d ",(unsigned int)screen_buff[i*XRES + j]);
			}
			printf("\n");
		}
		c -= 0.05;
		don->change_angle(vec3<double>(1.1,c,1.1*c));
	}
	return 0;
}
