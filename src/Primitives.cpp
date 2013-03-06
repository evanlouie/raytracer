///////////////////////////////////////////////////////////////////////////
//
//	simple raytracer
//
//	UBC CS 314 - Feburary 2013
//	instructor: Tamara Munzner [tmm@cs.ubc.ca] 
//	assignment 3
//
//	originally written by Gordon Wetzstein
//	updated by and questions should go to: Yufeng Zhu [mike323@cs.ubc.ca]
//	
///////////////////////////////////////////////////////////////////////////

#include <Primitives.h>

#include <math.h>

///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//	calculate the intersection of a plane the given ray
//	the ray has an origin and a direction, ray = origin + t*direction
//	find the t parameter, return true if it is between 0.0 and 1.0, false 
//	otherwise, write the results in following variables:
//	depth	- t \in [0.0 1.0]
//	posX	- x position of intersection point, nothing if no intersection
//	posY	- y position of intersection point, nothing if no intersection
//	posZ	- z position of intersection point, nothing if no intersection
//	normalX	- x component of normal at intersection point, nothing if no intersection
//	normalX	- y component of normal at intersection point, nothing if no intersection
//	normalX	- z component of normal at intersection point, nothing if no intersection
//
/////////////////////////////////////////////////////////////////////////////////
bool 
	Plane::intersect(Ray ray, double *depth,
	double *posX, double *posY, double *posZ,
	double *normalX, double *normalY, double *normalZ)

{
	//////////*********** START OF CODE TO CHANGE *******////////////
	double t = (-this->params[3] - this->params[0]*ray.origin[0] - this->params[1]*ray.origin[1] - this->params[2]*ray.origin[2])/(this->params[0]*ray.direction[0] + this->params[1]*ray.direction[1] + this->params[2]*ray.direction[2]);

	if(t < 0 || t > 1)
	{
		return false;
	}
	else
	{
		*depth = t;
		*posX = ray.origin[0] + ray.direction[0]*t;
		*posY = ray.origin[1] + ray.direction[1]*t;
		*posZ = ray.origin[2] + ray.direction[2]*t;

		double side = this->params[0]*ray.origin[0] + this->params[1]*ray.origin[1] + this->params[2]*ray.origin[2] + this->params[3];
		double length = sqrt(this->params[0]*this->params[0] + this->params[1]*this->params[1] + this->params[2]*this->params[2]);

		if(side >= 0)
		{
			*normalX = this->params[0]/length;
			*normalY = this->params[1]/length;
			*normalZ = this->params[2]/length;
		}
		else
		{
			*normalX = -this->params[0]/length;
			*normalY = -this->params[1]/length;
			*normalZ = -this->params[2]/length;
		}
		return true;
	}
	//////////*********** END OF CODE TO CHANGE *******////////////

	return true;
}

///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//	calculate the intersection of a sphere the given ray
//	the ray has an origin and a direction, ray = origin + t*direction
//	find the t parameter, return true if it is between 0.0 and 1.0, false 
//	otherwise, write the results in following variables:
//	depth	- t \in [0.0 1.0]
//	posX	- x position of intersection point, nothing if no intersection
//	posY	- y position of intersection point, nothing if no intersection
//	posZ	- z position of intersection point, nothing if no intersection
//	normalX	- x component of normal at intersection point, nothing if no intersection
//	normalX	- y component of normal at intersection point, nothing if no intersection
//	normalX	- z component of normal at intersection point, nothing if no intersection
//
//	attention: a sphere has usually two intersection points make sure to return 
//	the one that is closest to the ray's origin and still in the viewing frustum
//
/////////////////////////////////////////////////////////////////////////////////
bool 
	Sphere::intersect(Ray ray, double *depth,	
	double *posX, double *posY, double *posZ,
	double *normalX, double *normalY, double *normalZ)

{
	//////////*********** START OF CODE TO CHANGE *******////////////
	Vec3 rDirection (ray.direction[0],ray.direction[1],ray.direction[2]);
	Vec3 rOrigin (ray.origin[0],ray.origin[1],ray.origin[2]);
	double discrim, a, b, c;

	Vec3 temp (rOrigin[0]-this->center[0], rOrigin[1]-this->center[1], rOrigin[2]-this->center[2]);
	a = rDirection.length()*rDirection.length();
	b = 2 * rDirection.dot(temp);
	c = temp.length()*temp.length() - this->radius * this->radius;
	discrim = b*b-4*a*c;

	if (discrim <0)
	{
		return false;
	}
	double t0 = (-b - sqrt(discrim)) / (2*a);
	double t1 = (-b + sqrt(discrim)) / (2*a);
	double t;

	if (t0 < t1)
	{
		t = t0;
	}
	else
	{
		t = t1;
	}
	if(t >= 0)
	{
		*depth = t;
		//using the ray parametric formula
		*posX = rOrigin[0] + rDirection[0] * *depth;
		*posY = rOrigin[1] + rDirection[1] * *depth;
		*posZ = rOrigin[2] + rDirection[2] * *depth;

		//sphere normal = intersect point - sphere centre
		Vec3 sphereN((*posX - this->center[0]),(*posY - this->center[1]),(*posZ - this->center[2]));
		sphereN.normalize();
		*normalX = sphereN[0];
		*normalY = sphereN[1];
		*normalZ = sphereN[2];

		return true;
	}
	else
		return false;
	//////////*********** END OF CODE TO CHANGE *******////////////

}

///////////////////////////////////////////////////////////////////////////
