#ifndef _FRENDERER_BODY_H_
#define _FRENDERER_BODY_H_
class scene;
class body : public object{
	public:
	int pdegree; 
	double reflectivity;
	body(vec3<double>, vec3<double>,double,int,bool=false,double=0,double=0,double=0); // done
	virtual vec3<double> diffuse_color(vec3<double>)=0;
	virtual cplx surf_func(vec3<cplx>)=0;
	virtual vec3<double> shader(scene*, double, dir3<double>, int)=0;
	vec3<double> lambert_shader(scene*, double, dir3<double>); // done
//	dir3<double> phong_shader(scene&, double, dir3<double>, int, double); // todo
	virtual double intersect(vec3<double>, dir3<double>); // done
	void change_angle(vec3<double>); // done
	virtual dir3<double> get_normal(vec3<double>);
	bool hitbox;
	double a;
	double b;
	double c;
	bool intersect_hitbox(vec3<double>,dir3<double>);
	vec3<cplx> TG(vec3<cplx>);
	vec3<double> TG(vec3<double>);
	private:
	cplx g(vec3<double>, dir3<double>, cplx); // done
	vec3<double> angle;
	double cosa;
	double sina;
	double cosb;
	double sinb;
	double cosc;
	double sinc;
	double R[3][3];
};
#endif
