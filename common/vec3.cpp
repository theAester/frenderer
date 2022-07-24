#include <complex>
#include "vec3.h"
#include "dir3.h"
//===========vec3=================
template<class T>
vec3<T>::vec3(T x, T y, T z){
	this->x = x;
	this->y = y;
	this->z = z;
}
template<class T>
vec3<T>::vec3(){
	x=0;
	y=0;
	z=0;
}
template<class T>
vec3<T>::vec3(dir3<T> d){
	x=d.x;
	y=d.y;
	z=d.z;
}
template<class T>
vec3<T> vec3<T>::operator+(vec3 p){
	return vec3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
}
template<class T>
vec3<T> vec3<T>::operator-(vec3 p){
	return vec3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
}
template<class T>
T vec3<T>::operator*(vec3<T> p){
	return this->x * p.x + this->y * p.y + this->z * p.z;
}
template<class T>
vec3<T> vec3<T>::operator*(T f){
	return vec3<T>(this->x *f, this->y *f, this->z *f);
}

template<class T>
void vec3<T>::operator+=(vec3<T> p){
	this->x += p.x;
	this->y += p.y;
	this->z += p.z;
}
template<class T>
void vec3<T>::operator-=(vec3<T> p){
	this->x -= p.x;
	this->y -= p.y;
	this->z -= p.z;
}
template<class T>
void vec3<T>::operator*=(T f){
	this->x *= f;
	this->x *= f;
	this->x *= f;
}

template class vec3<double>;
template class vec3<std::complex<double>>;
