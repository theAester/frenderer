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

	ball* don = new ball(vec3<double>(0,0,0), vec3<double>(1.1,0,0), 3);

	sc.bodies.push_back(don);

	light* lt = new light(vec3<double>(-5,-3,4), vec3<double>(1,1,1));
	light* lt2 = new light(vec3<double>(-7,3,2), vec3<double>(0.2,0.2,0.2));

	sc.lights.push_back(lt);
	sc.lights.push_back(lt2);

	int pitch = XRES;
	unsigned char* screen_buff = new unsigned char[XRES*YRES];
	double c=0;
	//while(true){
		clear();
		cgoto(0,0);
		sc.render( (void*) screen_buff,pitch, true);
		for(int i=0;i<YRES;i++){
			for(int j=0;j<XRES;j++){
				printf("%c ",chars[ sizeof(chars)-1-seg( (unsigned int)screen_buff[i*XRES + j] ) ]);
			}
			printf("\n");
		}
		c -= 0.05;
		don->change_angle(vec3<double>(1.1,c,1.1*c));
	//}
	return 0;
}
