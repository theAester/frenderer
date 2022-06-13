#include "../common/comm.h"
#include "object.h"
#include "light.h"
light::light(vec3<double> pos, vec3<double> color) : object(pos){
	this->color = color;
}
