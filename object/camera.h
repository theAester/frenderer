#ifndef _FREDNDERER_CAMERA_H_
#define _FREDNDERER_CAMERA_H_
class camera : public object{
	public:
	vec3<double> angle;

	camera(vec3<double>, vec3<double>, double, double);
	camera();
	double fov;
	double lense_dist;

	dir3<double> proj_dir();
	dir3<double> up_dir();
};
#endif
