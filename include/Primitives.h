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

#ifndef __PRIMITIVES__
#define __PRIMITIVES__

#include <Operations.h>

///////////////////////////////////////////////////////////////////////////

// helper function for iterating over std::vector<>
// it makes iterating over STL elements much easier
// instead of 
//
//		_type::iterator _var = _coll.begin();
//		for(; _var != _coll.end(); _var++) {
//			...
//		}
//
//	you can just use
//
//		foreach(_var, _coll, _type) {
//			...
//		}
#define foreach(_var, _coll, _type) \
	for (_type::iterator _var = _coll.begin(); \
         _var != _coll.end(); ++_var)

///////////////////////////////////////////////////////////////////////////

class Ray {
public:

	// constructor
	Ray(double x, double y, double z, double dir_x, double dir_y, double dir_z)
	{
		origin[0]=x;
		origin[1]=y;
		origin[2]=z;
		direction[0]=dir_x;
		direction[1]=dir_y;
		direction[2]=dir_z;
	};
	// destructor
	~Ray() {};

	// ray's origin
	double origin[3];
	// direction
	double direction[3];
};

///////////////////////////////////////////////////////////////////////////

class Material {

public:

	Material() {};
	~Material() {};

	// ID
	int	id;
	// 4 component ambient color  
	double ambient[4];
	// 4 component diffuse color
	double diffuse[4];
	// 4 component specular color
	double specular[4];
	// 4 component emission color
	double emission[4];
	// 4 shininess factor
	double shininess;
	// shadow [0...no shadow, 1...black shadow, 
	// everything in between is blended by that
	// factor with the surface color
	double shadow;
	// shadow [0...no reflection, 1...only reflection, 
	// everything in between is blended by that
	// factor with the surface color
	double reflect;
};

///////////////////////////////////////////////////////////////////////////

class Primitive {

public:
	Primitive() {};
	~Primitive() {};

	// returns wether the ray intersects the primitive
	// if that was successful pos_x,y,z will have the closest interesction point
	// and normal_x,y,z the corresponding normal
	virtual bool intersect(	Ray ray, double *depth,
							double *posX, double *posY, double *posZ,
							double *normalX, double *normalY, double *normalZ)=0;

	Material material;
	int materialID;
};

///////////////////////////////////////////////////////////////////////////

class Sphere : public Primitive {

public:
	// constructor
	Sphere() :  Primitive() {}
	// destructor
	~Sphere() {};

	// intersection test
	bool intersect(	Ray ray, double *depth,
					double *posX, double *posY, double *posZ,
					double *normalX, double *normalY, double *normalZ);

	// the sphere's radius
	double radius;
	// center point
	double center[3];
};

///////////////////////////////////////////////////////////////////////////

// Ax + By + Cz + D = 0
class Plane : public Primitive {

public:
	// constructor
	Plane() :  Primitive() {}
	// destructor
	~Plane() {};

	// intersection test
	bool intersect(	Ray ray, double *depth,
					double *posX, double *posY, double *posZ,
					double *normalX, double *normalY, double *normalZ);

	// the 4 parameters
	double params[4];
};

///////////////////////////////////////////////////////////////////////////

class PointLight {

public:
	// constructor
	PointLight() {};
	// destructor
	~PointLight() {};

	// position
	double position[3];
	// ambient color  
	double ambient[3];
	// diffuse color
	double diffuse[3];
	// specular color
	double specular[3];
	// light attenuation
	double attenuation[3];
};

///////////////////////////////////////////////////////////////////////////

class Camera {

public:
	
	// constructor
	Camera() {};
	// destructor
	~Camera() {};

	// position x,y,z
	double position[3];
	// lookat point [center]
	double center[3];
	// up vector
	double up[3];

	// vertical field of view
	double fovy;
	// aspect ration, i.e. width:height
	double aspect;
	// near clipping plane
	double zNear;
	// far clipping plane
	double zFar;

};

///////////////////////////////////////////////////////////////////////////

#endif
