#ifndef _FRENDERER_VEC3_H_
#define _FRENDERER_VEC3_H_
template<class T>
class vec3{
	public:
	T x;
	T y;
	T z;
	vec3(T,T,T);

	vec3<T> operator+(vec3);
	vec3<T> operator-(vec3);
	T operator*(vec3);
	vec3<T> operator*(T);

	void operator+=(vec3);
	void operator-=(vec3);
	void operator*=(T);

};
//#include "vec3.cpp"
#endif
