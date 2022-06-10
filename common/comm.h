
template<class T>
class dir3;

template<class T>
class vec3{
	public:
		T x;
		T y;
		T z;
		vec3(T x, T y, T z){
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3(){
			x=0;
			y=0;
			z=0;
		}
		vec3(dir3<T> d){
			x=d.x;
			y=d.y;
			z=d.z;
		}
		vec3<T> operator+(vec3 p){
			return vec3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
		}
		vec3<T> operator-(vec3 p){
			return vec3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
		}
		T operator*(vec3<T> p){
			return this->x * p.x + this->y * p.y + this->z * p.z;
		}
		vec3<T> operator*(T f){
			return vec3<T>(this->x *f, this->y *f, this->z *f);
		}

		void operator+=(vec3<T> p){
			this->x += p.x;
			this->y += p.y;
			this->z += p.z;
		}
		void operator-=(vec3<T> p){
			this->x -= p.x;
			this->y -= p.y;
			this->z -= p.z;
		}
		void operator*=(T f){
			this->x *= f;
			this->x *= f;
			this->x *= f;
		}

};

template <class T>
class dir3{
	public:
	T x;
	T y;
	T z;

	dir3(T x, T y, T z){
		this->pointTowards(vec3<T>(x,y,z));
	}
	dir3(vec3<T> p){
		this->pointTowards(p);
	}
	dir3(){
		x=0;
		y=0;
		z=0;
	}
	dir3<T> operator+(dir3<T> p){
		return dir3<T>(this->x + p.x, this->y + p.y, this->z + p.z);
	}
	dir3<T> operator-(dir3<T> p){
		return dir3<T>(this->x - p.x, this->y - p.y, this->z - p.z);
	}
	T operator*(dir3<T> p){
		return this->x * p.x + this->y * p.y + this->z * p.z;
	}
	vec3<T> operator*(T f){
		return vec3<T>(this->x *f, this->y *f, this->z *f);
	}

	void operator+=(dir3<T> p){
		this->pointTowards(vec3<T>(this->x + p.x, this->y+ p.y, this->z + p.z));
	}
	void operator-=(dir3 p){
		this->pointTowards(vec3<T>(this->x - p.x, this->y -p.y, this->z - p.z));
	}


	void pointTowards(vec3<T> p){
		T size = sqrt(p * p);
		this->x = p.x/size;
		this->y = p.y/size;
		this->z = p.z/size;
	}
};
