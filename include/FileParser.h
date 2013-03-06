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

#ifndef __FILE_PARSER__
#define __FILE_PARSER__

#include <Primitives.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class FileParser {

public:
	FileParser() {};
	~FileParser() {};

	// parse a file and write the output in the vectors
	bool parse(	const char *filename,		std::vector<PointLight> *lights, 
				std::vector<Plane> *planes,	std::vector<Sphere> *spheres,
				Camera *camera, int *resolution);
};

#endif
