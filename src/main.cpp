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
#include <FileParser.h>
#include <Raytracer.h>

#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

static string scenefile("demo/testscene.ray");
static string outfilename("demo/testscene.ppm");
static string outfilename_depth("demo/testscene_depth.ppm");

int main(int argc, char **argv)
{	
	vector<PointLight>	lights;
	vector<Plane>		planes;
	vector<Sphere>		spheres;
	Camera				camera;
	int resolution[] = {100,100};
 
	FileParser parser;

	if(argc > 1) 
		scenefile = string(argv[1]);
	
	if(!parser.parse(scenefile.c_str(), &lights, &planes, &spheres, &camera, resolution))
		exit(-1);

	// output filename specified
	if(argc == 3) {
		outfilename = string(argv[2]);
		outfilename_depth = outfilename.substr(0, (int)(outfilename.length())-4);
		outfilename_depth.append("_depth.ppm");
	}

	cout << "save output image to " << outfilename.c_str() << " and " << outfilename_depth.c_str() << endl;

	Raytracer raytracer;
	raytracer.raytraceScene(	outfilename.c_str(), 
								outfilename_depth.c_str(), 
								&lights, 
								&planes, 
								&spheres, 
								&camera, 
								resolution	);

}
