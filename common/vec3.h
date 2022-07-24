#ifndef _FRENDERER_VEC3_H_
#define _FRENDERER_VEC3_H_

template<class T>
class dir3;

template<class T>
class vec3{
	public:
	T x;
	T y;
	T z;
	vec3(T,T,T);
	vec3();
	vec3(dir3<T>);

	vec3<T> operator+(vec3<T>);
	vec3<T> operator-(vec3<T>);
	T operator*(vec3<T>);
	vec3<T> operator*(T);

	void operator+=(vec3<T>);
	void operator-=(vec3<T>);
	void operator*=(T);

};
#endif
