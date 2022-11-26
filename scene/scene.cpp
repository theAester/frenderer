#include <complex>
#include <cmath>
#include <list>
#include <limits>
#define inf numeric_limits<double>::max()
using namespace std;
using cplx = complex<double>;
#include "../common/vec3.h"
#include "../common/dir3.h"
#include "../object/object.h"
#include "../object/camera.h"
#include "../object/body.h"
#include "../object/light.h"
#include "scene.h"

#ifndef M_PI
#define M_PI 3.14159265368979
#endif

scene::scene(int resolution_x, int resolution_y, camera cam, vec3<double> ambient){
	resolution.x = resolution_x;
	resolution.y = resolution_y;
	this->cam = cam;
	this->ambient = ambient;
}

void scene::render(void* screen_in,int pitch, bool grey){
	// calculate screen projection points
	double aspect_ratio = ((double)resolution.x)/((double)resolution.y);
	dir3<double> cam_proj = cam.proj_dir();
	dir3<double> cam_up = cam.up_dir();

	dir3<double> khat = dir3<double>(
			cam_proj.y * cam_up.z - cam_proj.z * cam_up.y,
			cam_proj.z * cam_up.x - cam_proj.x * cam_up.z,
			cam_proj.x * cam_up.y - cam_proj.y * cam_up.x
			);//cross product

	double dist = cam.lense_dist;
	double l = dist*tan(cam.fov*M_PI/360);
	double ll = l / aspect_ratio;
	
	vec3<double> screen_corner = cam_up*ll - khat*l + cam_proj*dist;

	double stepx = 2*l/resolution.x;
	double stepy = 2*ll/resolution.y;
	
	uint32_t* pp;
	unsigned char* kk;

	for(int j=0;j<resolution.y;j++){
		if(!grey) pp = (uint32_t*)((uint8_t*) screen_in + j*pitch);
		else kk = (unsigned char*)screen_in + j*pitch;
		for (int i=0;i<resolution.x;i++){
			dir3<double> prdir ( screen_corner + khat*i*stepx - cam_up*j*stepy);
			vec3<double> acq_color = raytrace_master(prdir);
			acq_color = clip(acq_color);
			if(!grey) *pp= 0xFF000000 | ((uint8_t) (acq_color.x*255))<<16 | ((uint8_t) (acq_color.y*255))<<8 | (uint8_t) (acq_color.z*255);
			else *kk = (unsigned char) (acq_color.x*255);
			if(!grey) {if(!(i==resolution.x-1 && j==resolution.y-1)) pp ++;}
			else if(!(i==resolution.x-1 && j==resolution.y-1)) kk ++;
		}
	}
	return;
}

vec3<double> scene::clip(vec3<double> v){
	if(v.x<0) v.x = 0;
	else if(v.x>1) v.x = 1;
	if(v.y<0) v.y = 0;
	else if(v.y>1) v.y = 1;
	if(v.z<0) v.z = 0;
	else if(v.z>1) v.z = 1;
	return v;
}

vec3<double> scene::raytrace_master(dir3<double> dir){
	return raytrace_single(cam.pos, dir, 0);
}

vec3<double> scene::raytrace_single(vec3<double> pos, dir3<double> dir, int bounce =0){
	list<body*>::iterator itt = bodies.begin();
	list<body*>::iterator itts = itt;
	list<body*>::iterator itte = bodies.end();
	double mindist = inf;
	while(itt != itte){
		double dist = (*itt)->intersect(pos,dir);
		if(dist != inf){ // collision detected
			if(dist<mindist){
				mindist = dist;
				itts = itt;
			}
		}
		itt ++;
	}
	if(mindist == inf)
		return vec3<double>(0,0,0); // into oblivion ...

	return (*itts)->shader(this, mindist, dir, bounce + 1);
}
