class point3f{
	float x;
	float y;
	float z;
	point3f(float,float,float);

	point3f operator=(point3f b);
};
class orient{
	float a;
	float b;
	float c;
	orient (float,float,float);
	
	orient operator=(orient);
	orient operator=(point3f);

	void pointTowards(point3f);
};
