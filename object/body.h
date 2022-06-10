#ifndef _FRENDERER_BODY_H_
#define _FRENDERER_BODY_H_
#include <limits>
#include <complex>
#define inf numeric_limits<double>::max()
class scene;
class body : public object{
	public:
	double reflectivity;
	body(vec3<double>, vec3<double>,double); // done
	virtual vec3<double> diffuse_color(vec3<double>)=0;
	virtual cplx surf_func(vec3<cplx>)=0;
	virtual vec3<double> shader(scene*, double, dir3<double>, int)=0;
	vec3<double> lambert_shader(scene*, double, dir3<double>, int, int); // done
//	dir3<double> phong_shader(scene&, double, dir3<double>, int, double); // todo
	double intersect(vec3<double>, dir3<double>); // done
	void change_angle(vec3<double>); // done
	private:
	cplx g(vec3<double>, dir3<double>, cplx); // done
	cplx TG(vec3<cplx>);
	dir3<double> get_normal(vec3<double>);
	vec3<double> angle;
	double cosa;
	double sina;
	double cosb;
	double sinb;
	double cosc;
	double sinc;
};
#endif
