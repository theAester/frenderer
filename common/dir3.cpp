#include <complex>
#include "vec3.h"
#include "dir3.h"
//===========dir3=================

template<class T>
	dir3<T>::dir3(T x, T y, T z){
		this->pointTowards(vec3<T>(x,y,z));
	}

template<class T>
	dir3<T>::dir3(vec3<T> p){
		this->pointTowards(p);
	}

template<class T>
	dir3<T>::dir3(){
		x=0;
		y=0;
		z=0;
	}

template<class T>
	dir3<T> dir3<T>::operator+(dir3<T> p){
		return dir3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
	}

template<class T>
	dir3<T> dir3<T>::operator-(dir3<T> p){
		return dir3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
	}

template<class T>
	T dir3<T>::operator*(dir3<T> p){
		return this->x * p.x + this->y * p.y + this->z * p.z;
	}

template<class T>
	vec3<T> dir3<T>::operator*(T f){
		return vec3<T>(this->x *f, this->y *f, this->z *f);
	}

template<class T>
	void dir3<T>::operator+=(dir3<T> p){
		this->pointTowards(vec3<T>(this->x + p.x, this->y+ p.y, this->z + p.z));
	}

template<class T>
	void dir3<T>::operator-=(dir3 p){
		this->pointTowards(vec3<T>(this->x - p.x, this->y -p.y, this->z - p.z));
	}

template<class T>
	void dir3<T>::pointTowards(vec3<T> p){
		T size = sqrt(p * p);
		this->x = p.x/size;
		this->y = p.y/size;
		this->z = p.z/size;
	}


template class dir3<double>;
template class dir3<std::complex<double>>;
