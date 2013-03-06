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

#ifndef __OPERATIONS__
#define __OPERATIONS__

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

///////////////////////////////////////////////////////////////////////////

class Vec3 {
public:
	// constructor
	Vec3(double x, double y, double z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	};
	// destructor
	~Vec3() {};

	// access to individual elements
	double& operator[] (int i) {
		return coord[i];
	}; 

	// dot product with other vector
	double dot(Vec3 other) {
		return coord[0]*other[0] + coord[1]*other[1] + coord[2]*other[2];
	};

	// cross product with other vector
	Vec3 cross(Vec3 other) {
		return Vec3(coord[1]*other[2] - coord[2]*other[1],
					coord[2]*other[0] - coord[0]*other[2],
					coord[0]*other[1] - coord[1]*other[0] );
	};

	// magnitude/length of this vector
	double length() {
		return sqrt(coord[0]*coord[0] + coord[1]*coord[1] + coord[2]*coord[2]);
	};

	// normalized vector
	void normalize() {
		double l=length();
		coord[0] /= l;
		coord[1] /= l;
		coord[2] /= l;
	};

	double coord[3];

};

///////////////////////////////////////////////////////////////////////////

static double rad2deg(double rad) {
	return rad*360.0/(2*PI);
};

static double deg2rad(double deg) {
	return deg*2*PI/360.0;
};

///////////////////////////////////////////////////////////////////////////

#endif
