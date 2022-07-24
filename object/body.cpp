#include <complex>
#include <list>
#include <limits>
#define inf numeric_limits<double>::max()
using namespace std;
using cplx = complex<double>;
#include "../common/vec3.h"
#include "../common/dir3.h"
#include "object.h"
#include "body.h"
#include "light.h"
#include "camera.h"
#include "../scene/scene.h"

body::body(vec3<double> pos, vec3<double> angle,double reflectivity, int pdegree,bool hitbox,double a, double b,double c): object(pos){
	this->hitbox = hitbox;
	this->a = a;
	this->b = b;
	this->c = c;
	this->reflectivity = reflectivity;
	this->change_angle(angle);
	this->pdegree = pdegree;
}
bool body::intersect_hitbox(vec3<double> o, dir3<double> d){
	vec3<double> ooo = TG(o-pos);
	vec3<double> ddd = TG(d);
	vec3<double> oo((ooo.x)/a,(ooo.y)/b,(ooo.z)/c);
	vec3<double> dd(ddd.x/a,ddd.y/b,ddd.z/c);
	double aa = dd*dd;
	double bb = oo*dd*2;
	double cc = oo*oo - 1;
	double k = bb*bb - 4*aa*cc; 
	if(k > 0) return true;
	return false;
}
double body::intersect(vec3<double> o, dir3<double> d){ // perfom durand-kerner algorithm to find roots and look for real roots
	if(hitbox){
		if(!intersect_hitbox(o,d))
			return inf;
	}
	bool first = false;
	cplx q[pdegree];
	q[0] = cplx(1.3,1.5);
	for(int i=1;i<pdegree;i++){
		q[i] = std::pow(q[0],i+1);
	}

	cplx qs[pdegree];
	bool done[pdegree];
	for(int i=0;i<pdegree;i++){
		done[i] = false;
	}
	double min = inf;
	for(int i=0; i<36;i++){
		for(int j=0;j<pdegree;j++){
			if(done[j]) continue;
			cplx temp = g(o,d,q[j]);
			for(int k=0;k<pdegree;k++){
				if(k == j) continue;
				temp  = temp / (q[j]-q[k]);
			}
			qs[j] = q[j] - temp;
		}
		for(int j=0;j<pdegree;j++){
			q[j] = qs[j];
			if(std::abs(q[j].imag()) < 0.000001){
				done[j] = true;
				if(q[j].real() < min && real(q[j]) > 0){
					min = q[j].real();
				}
			}
		}
	}

	for(int i=0;i<pdegree;i++){
		if(std::abs(q[i].imag()) < 0.000001){
			if(q[i].real() < min && real(q[i]) > 0){
				min = q[i].real();
			}
		}
	}
	return min;
}

cplx body::g(vec3<double> o, dir3<double> d, cplx c){
	// let r = o + s*d
	// g(s) = f(R_zR_yR_x[r.xhat;r.yhat;r.zhat] + pos)
	// where pos is the position vector and R_q indicates the rotation matrix about q axis
	vec3<cplx> O(o.x - pos.x , o.y - pos.y, o.z - pos.z); // typecasting into complex number
	vec3<cplx> D(d.x,d.y,d.z); // typecasting into complex number
	vec3<cplx> r = O + D*c;
	return surf_func(TG(r));
}

vec3<cplx> body::TG(vec3<cplx> r){
	vec3<cplx> inpt(
		R[0][0]*r.x + R[0][1]*r.y + R[0][2]*r.z,
		R[1][0]*r.x + R[1][1]*r.y + R[1][2]*r.z,
		R[2][0]*r.x + R[2][1]*r.y + R[2][2]*r.z
		);
	return inpt;

}

vec3<double> body::TG(vec3<double> r){
	vec3<double> inpt(
		R[0][0]*r.x + R[0][1]*r.y + R[0][2]*r.z,
		R[1][0]*r.x + R[1][1]*r.y + R[1][2]*r.z,
		R[2][0]*r.x + R[2][1]*r.y + R[2][2]*r.z
		);
	return inpt;

}

void body::change_angle(vec3<double> d){
	this->angle =d;
	cosa = cos(d.x);
	cosb = cos(d.y);
	cosc = cos(d.z);
	sina = sin(d.x);
	sinb = sin(d.y);
	sinc = sin(d.z);
	R[0][0] = cosb*cosc; 
	R[0][1] = cosc*sinb*sina-cosa*sinc;
	R[0][2] = cosc*sinb*cosa+sinc*sina;
	R[1][0] = cosb*sinc;
	R[1][1] = sinc*sinb*sina+cosc*cosa;
       	R[1][2] = sinc*sinb*cosa-sina*cosc;
	R[2][0] = -1*sinb; 
	R[2][1] = sina*cosb;
	R[2][2] = cosa*cosb;
}

dir3<double> body::get_normal(vec3<double> point){
	double h = 0.0001;
	vec3<cplx> p(point.x-pos.x,point.y-pos.y,point.z-pos.z);
	vec3<cplx> dx(h,0,0);
	vec3<cplx> dy(0,h,0);
	vec3<cplx> dz(0,0,h);
	double Dx = real(surf_func(TG(p+dx)) - surf_func(TG(p-dx)))/(2*h);
	double Dy = real(surf_func(TG(p+dy)) - surf_func(TG(p-dy)))/(2*h);
	double Dz = real(surf_func(TG(p+dz)) - surf_func(TG(p-dz)))/(2*h);
	return dir3<double>(Dx,Dy,Dz);
}

vec3<double> body::lambert_shader(scene* sc, double distance, dir3<double> ori){
	vec3<double> color = sc->ambient;
	vec3<double> intersection = sc->cam.pos + ori*distance;
	dir3<double> dir_cam = sc->cam.pos - intersection;
	dir3<double> normal = get_normal(intersection);
	vec3<double> intersection_shifted = intersection + normal*0.001;
	list<light*>& lights = sc->lights;
	list<body*>& bodies = sc->bodies;
	list<light*>::iterator itt = lights.begin();
	list<light*>::iterator itte = lights.end();
	while(itt != itte){
		vec3<double> vec_light = (*itt)->pos-intersection;
		dir3<double> dir_light = vec_light;
		//shadows
		int is_visible = 1;
		list<body*>::iterator iitt = bodies.begin();
		list<body*>::iterator iitte = bodies.end();
		while(iitt != iitte){
			if(((*iitt)->intersect(intersection_shifted, dir_light)) != inf){ // there is a shadow
				is_visible =0;
				break;
			}
			iitt ++;
		}
	
		double lambert = normal * dir_light;
		lambert = lambert < 0 ? 0 : lambert;
		vec3<double> lightdiff = diffuse_color(intersection) * (is_visible * lambert);

		color += vec3<double>((*itt)->color.x*lightdiff.x, (*itt)->color.y*lightdiff.y, (*itt)->color.z*lightdiff.z)*(1-reflectivity); 

		itt++;
	}
	return color;
}
