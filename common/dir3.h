#ifndef _FRENDERER_dir3_H_
#define _FRENDERER_dir3_H_

template <class T>
class dir3{
	public:
	T x;
	T y;
	T z;
	dir3(T,T,T);
	dir3(vec3<T>);
	dir3();
	dir3<T> operator+(dir3<T>);
	dir3<T> operator-(dir3<T>);
	vec3<T> operator*(T);
	T operator*(dir3<T>);
	void operator+=(dir3<T>);
	void operator-=(dir3<T>);
	void pointTowards(vec3<T>);
};

#endif
