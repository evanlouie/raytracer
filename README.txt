///////////////////////////////////////////////////////////////////////////
//
//	simple raytracer
//
//	UBC CS 314 - January 2010
//	instructor: Tamara Munzner [tmm@cs.ubc.ca] 
//	assignment 3
//
//	originally written by Gordon Wetzstein
//	updated by and questions should go to: Kai Ding [dingkai@cs.ubc.ca]
//	
///////////////////////////////////////////////////////////////////////////

compile
-------

LINUX: make or Eclipse
Windows: Visual Studio 2008
MAC: make or Eclipse

run
---

raytracer scene.ray outfilename.ppm 

e.g.: raytracer demo/testscene.ray demo/testscene.ppm

(If you build the project by makefile or Visual Studio, the
executable file will be generated under the uppest directory
/raytracer_template[_win|_linux|_mac])
(If you build the project by Eclipse, the executable file
will be generated under the directory /Debug or /Release,
copy it to the uppest directory /raytracer_template[_win|_linux|_mac])

[default outfilename is demo/testscene.ppm]
 