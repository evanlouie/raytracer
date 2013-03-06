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

#ifndef __RAYTRACER__
#define __RAYTRACER__

#include <Primitives.h>
#include <vector>

// maximum number of reflected rays for each pixels
#define MAX_RAY_RECURSION 10

class Raytracer {

public:
	// constructor
	Raytracer() {};
	~Raytracer() {};

	// raytrace a scene with lights, objects, a given camera and pixel resolution
	void	raytraceScene(	const char *filename,
							const char *depth_filename,
							std::vector<PointLight> *lights, 
							std::vector<Plane> *planes,	
							std::vector<Sphere> *spheres,
							Camera *camera, int *resolution	);

private:

	// write the output file 
	bool	writeImage(	const char *filename, 
						unsigned char *imageBuffer, 
						int *resolution, 
						bool greyscale=false);

	// shoot a ray into the scene and calculate the color value
	void	traceRay(	Ray pixelRay, 
						std::vector<PointLight> *lights, 
						std::vector<Plane> *planes,	
						std::vector<Sphere> *spheres, 
						Camera *camera,
						int *currRayRecursion,
						double *red , double *green, double *blue, double *depth );

	// compute the shading at a given point and normal
	// you also need the light sources and the other objects for
	// shadows and reflections 
	void	shade(	double posX, double posY, double posZ,
					double normalX, double normalY, double normalZ,
					Material material, Camera *camera, 
					std::vector<PointLight> *lights, 
					std::vector<Plane>		*planes,	
					std::vector<Sphere>		*spheres,
					double *red, double *green, double *blue); 

};

#endif
