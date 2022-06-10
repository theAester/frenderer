#ifndef _FRENDERER_SCENE_H_
#define _FRENDERER_SCENE_H_
#include <list>
class scene{
	struct res{
		int x;
		int y;
	};
	vec3<double> raytrace_master(dir3<double>);
	vec3<double> clip(vec3<double>);
	public:
	vec3<double> raytrace_single(vec3<double>, dir3<double>, int);
	scene(int, int, camera,vec3<double>);
	res resolution;
	void render(uint32_t*);
	camera cam;
	list<body*> bodies;
	list<light*> lights;
	vec3<double> ambient;
};
#include "../object/body.cpp"
#include "scene.cpp"
#endif
