scene::scene(int resolution_x, int resolution_y, camera cam, vec3<double> ambient){
	resolution.x = resolution_x;
	resolution.y = resolution_y;
	this->cam = cam;
	this->ambient = ambient;
}

void scene::render(uint32_t* screen_in){
	// calculate screen projection points
	double aspect_ratio = ((double)resolution.x)/((double)resolution.y);
	dir3<double> cam_proj = cam.proj_dir();
	dir3<double> cam_up = cam.up_dir();

	dir3<double> khat = dir3<double>(
			cam_proj.y - cam_up.z,
			cam_proj.z - cam_up.x,
			cam_proj.x - cam_up.y
			);//cross product

	double dist = cam.lense_dist;
	double l = dist*tan(cam.fov*M_PI/360);
	double ll = l / aspect_ratio;
	
	vec3<double> screen_corner = cam_up*ll - khat*l;

	double stepx = 2*l/resolution.x;
	double stepy = 2*ll/resolution.y;
	
	uint32_t* pp = screen_in;

	for (int i=0;i<resolution.x;i++){
		for(int j=0;j<resolution.y;j++){
			dir3<double> prdir ( screen_corner + khat*i*stepx - cam_up*j*stepy - cam.pos);
			vec3<double> acq_color = raytrace_master(prdir);
			acq_color = clip(acq_color);
			*pp= 0xFF000000 | ((uint8_t) (acq_color.x*255))<<16 | ((uint8_t) (acq_color.y*255))<<8 | ((uint8_t) acq_color.z*255);
			pp += sizeof(uint32_t);
		}
	}
	return;
}

vec3<double> scene::clip(vec3<double> v){
	if(v.x<0) v.x = 0;
	else if(v.x>1) v.x = 1;
	if(v.y<0) v.y = 0;
	else if(v.y>1) v.y = 1;
	if(v.z<0) v.z = 0;
	else if(v.z>1) v.z = 1;
	return v;
}

vec3<double> scene::raytrace_master(dir3<double> dir){
	return raytrace_single(cam.pos, dir, 0);
}

vec3<double> scene::raytrace_single(vec3<double> pos, dir3<double> dir, int bounce =0){
	list<body*>::iterator itt = bodies.begin();
	list<body*>::iterator itts = itt;
	list<body*>::iterator itte = bodies.end();
	double mindist = inf;
	while(itt != itte){
		double dist = (*itt)->intersect(pos,dir);
		if(isfinite(dist))
			if(dist<mindist){
				mindist = dist;
				itts = itt;
			}
		itt ++;
	}
	if(!isfinite(mindist))
		return vec3<double>(0,0,0); // into oblivion ...

	return (*itts)->shader(this, mindist, dir, bounce + 1);
}
