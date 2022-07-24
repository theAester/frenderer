#include <cmath>

using std::sin;
using std::cos;
using std::sqrt;

#include "../common/vec3.h"
#include "../common/dir3.h"
#include "../object/object.h"
#include "camera.h"

camera::camera(vec3<double> pos, vec3<double> angle, double fov, double lense_dist) : object(pos){
	this->angle = angle;
	this->fov = fov;
	this->lense_dist = lense_dist;
}

camera::camera(){

}

dir3<double> camera::proj_dir(){
	double cosa = cos(angle.x);
	double sina = sin(angle.x);
	double cosb = cos(angle.y);
	double sinb = sin(angle.y);
	return dir3<double>(
			cosb*cosa, cosb*sina, -1*sinb
			);
}
dir3<double> camera::up_dir(){
	double cosa = cos(angle.x);
	double sina = sin(angle.x);
	double cosb = cos(angle.y);
	double sinb = sin(angle.y);
	double cosc = cos(angle.z);
	double sinc = sin(angle.z);
	return dir3<double>(
			cosc*sinb*cosa + sinc*sina,
			cosc*sinb*sina - sinc*cosa,
			cosc*cosb
			);
}
