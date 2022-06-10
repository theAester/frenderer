body::body(vec3<double> pos, vec3<double> angle,double reflectivity) : object(pos){
	this->reflectivity = reflectivity;
	this->change_angle(angle);
}

double body::intersect(vec3<double> o, dir3<double> d){ // perfom durand-kerner algorithm to find roots and look for real roots
	cplx q[4];
	q[0] = cplx(1.3,1.5);
	q[1] = pow(q[0],2);
	q[2] = pow(q[0],3);
	q[3] = pow(q[0],4);

	cplx qs[4];

	for(int i=0; i<40;i++){
		qs[0] = q[0] - g(o,d,q[0])/((q[0]-q[1])*(q[0]-q[2])*(q[0]-q[3]));
		qs[1] = q[1] - g(o,d,q[1])/((q[1]-q[0])*(q[1]-q[2])*(q[1]-q[3]));
		qs[2] = q[2] - g(o,d,q[2])/((q[2]-q[0])*(q[2]-q[1])*(q[2]-q[3]));
		qs[3] = q[3] - g(o,d,q[3])/((q[3]-q[0])*(q[3]-q[1])*(q[3]-q[2]));

		q[0] = qs[0];
		q[1] = qs[1];
		q[2] = qs[2];
		q[3] = qs[3];
	}
	double min = inf;

	for(int i=0;i<4;i++){
		if(abs(imag(q[i])) < 0.0000001)
			if(real(q[i]) < min && real(q[i]) > 0);
				min = real(q[i]);
	}
	return min;
}

cplx body::g(vec3<double> o, dir3<double> d, cplx c){
	// let r = o + s*d
	// g(s) = f(R_zR_yR_x[r.xhat;r.yhat;r.zhat] + pos)
	// where pos is the position vector and R_q indicates the rotation matrix about q axis
	vec3<cplx> O(o.x,o.y,o.z); // typecasting into complex number
	vec3<cplx> D(d.x,d.y,d.z); // typecasting into complex number
	vec3<cplx> q(D.x,D.y,D.z);
	vec3<cplx> r = O + q*c;
	return TG(r);
}

cplx body::TG(vec3<cplx> r){
	vec3<cplx> inpt(
		cosb*cosa*r.x + (sinc*sinb*cosa-cosc*sina)*r.y + (cosc*sinb*cosa+sinc*sina)*r.z - pos.x,
		cosb*cosa*r.x + (sinc*sinb*sina+cosc*cosa)*r.y + (cosc*sinb*sina-sinc*cosa)*r.z - pos.y,
		-1*sinb*r.x + sinc*cosb*r.y + cosc*cosb*r.z - pos.z
		);
	return surf_func(inpt);

}

void body::change_angle(vec3<double> d){
	this->angle =d;
	cosa = cos(d.x);
	cosb = cos(d.y);
	cosc = cos(d.z);
	sina = sin(d.x);
	sinb = sin(d.y);
	sinc = sin(d.z);
}

dir3<double> body::get_normal(vec3<double> point){
	double h = 0.0001;
	vec3<cplx> p(point.x,point.y,point.z);
	vec3<cplx> dx(h,0,0);
	vec3<cplx> dy(0,h,0);
	vec3<cplx> dz(0,0,h);
	double Dx = real(TG(p+dx) - TG(p-dx))/(2*h);
	double Dy = real(TG(p+dy) - TG(p-dy))/(2*h);
	double Dz = real(TG(p+dz) - TG(p-dz))/(2*h);
	return dir3<double>(Dx,Dy,Dz);
}

vec3<double> body::lambert_shader(scene* sc, double distance, dir3<double> ori, int bounce, int i_bounce){
	vec3<double> color = sc->ambient;
	vec3<double> intersection = sc->cam.pos + ori*distance;
	dir3<double> dir_cam = sc->cam.pos - intersection;
	dir3<double> normal = get_normal(intersection);
	vec3<double> intersection_shifted = intersection + normal*0.001;
	list<light*>& lights = sc->lights;
	list<body*>& bodies = sc->bodies;
	list<light*>::iterator itt = lights.begin();
	list<light*>::iterator itte = lights.end();
	while(itt != itte){
		vec3<double> vec_light = (*itt)->pos-intersection;
		dir3<double> dir_light = vec_light;
		//shadows
		int is_visible = 1;
		list<body*>::iterator iitt = bodies.begin();
		list<body*>::iterator iitte = bodies.end();
		while(iitt != iitte){
			if(isfinite((*iitt)->intersect(intersection_shifted, dir_light))){ // there is a shadow
				is_visible =0;
				break;
			}
			iitt ++;
		}
		vec3<double> lighti =vec3<double>(1,1,1) - (*itt)->color * (1/(vec_light*vec_light));
		double lambert = normal * dir_light;
		lambert = lambert < 0 ? 0 : lambert;
		vec3<double> lightdiff = diffuse_color(intersection) * (is_visible * lambert);

		color += vec3<double>(lighti.x*lightdiff.x, lighti.y*lightdiff.y, lighti.z*lightdiff.z)*(1-reflectivity); 

		if(bounce < i_bounce){
			dir3<double> reflect_dir = (normal * 2*(ori*normal))- ori;
			color += sc->raytrace_single(intersection_shifted, reflect_dir, bounce + 1) * reflectivity;
		}
	}
	return color;
}
