camera::camera(point3f pos, orient dir, float fov, float lense_dist) : object(pos){
	this->dir = dir;
	this->fov = fov;
	this->lense_dist = lense_dist;
}
