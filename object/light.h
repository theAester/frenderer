#ifndef _FRENDERER_LIGHT_H_
#define _FRENDERER_LIGHT_H_
class light : public object{
	public:
	vec3<double> color;
	light(vec3<double>, vec3<double>); 
};
#endif
