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

#include <FileParser.h>

using namespace std;
string dc="";
bool 
	FileParser::parse(	const char *filename,		std::vector<PointLight> *lights, 
	std::vector<Plane> *planes,	std::vector<Sphere> *spheres,
	Camera *camera, int *resolution)
{

	ifstream in;
	in.open(filename, ios::in);
	if(!in.is_open()) {
		cout << "Error: FileParser - could not open file " << filename << endl; 
		return false;
	}

	char currentLine[256];
	vector<Material> materials;

	cout << "====================================================" << endl;
	cout << "=================== parsing file ===================" << endl;
	cout << "====================================================" << endl;

	while(in.getline(currentLine,256)) {

		string line(currentLine);

		/////////////////////////////////////////////////////////////////////////////////	
		//	the parse code for a light source is given 
		//	with this you have to implement a proper parsing for Spheres,
		//	Planes, Materials, the Camera and the image Resolution
		/////////////////////////////////////////////////////////////////////////////////
		// light
		if(line.substr(0,10).compare("PointLight") == 0) {

			PointLight light;

			// read next line - position
			in.getline(currentLine,256);
			istringstream(currentLine) >> light.position[0] >> light.position[1] >> light.position[2];

			// read next line - ambient
			in.getline(currentLine,256);
			istringstream(currentLine) >> light.ambient[0] >> light.ambient[1] >> light.ambient[2];

			// read next line - diffuse
			in.getline(currentLine,256);
			istringstream(currentLine) >> light.diffuse[0] >> light.diffuse[1] >> light.diffuse[2];

			// read next line - specular
			in.getline(currentLine,256);
			istringstream(currentLine) >> light.specular[0] >> light.specular[1] >> light.specular[2];

			// read next line - emission
			in.getline(currentLine,256);
			istringstream(currentLine) >> light.attenuation[0] >> light.attenuation[1] >> light.attenuation[2];

			lights->push_back(light);
		}

		/////////////////////////////////////////////////////////////////////////////////
		//	parse a material here, the format is
		//
		//	Material ID								## were ID is an integer 
		//	ambientRed ambientGreen ambientBlue		## 3 double values
		//	diffuseRed diffuseGreen diffuseBlue		## 3 double values
		//	specularRed specularGreen specularBlue	## 3 double values
		//	emissionRed emissionGreen emissionBlue	## 3 double values
		//	shininess								## 1 double value
		//	shadow									## 1 double value
		//	reflect									## 1 double value
		//
		/////////////////////////////////////////////////////////////////////////////////
		// material
		else if(line.substr(0,8).compare("Material") == 0) {

			Material material;

			//////////*********** START OF CODE TO CHANGE *******////////////
			istringstream(currentLine) >> dc >> material.id;
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.specular[0] >> material.specular[1] >> material.specular[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.emission[0] >> material.emission[1] >> material.emission[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.shininess;
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.shadow;
			in.getline(currentLine,256);
			istringstream(currentLine) >> material.reflect;
			//////////*********** END OF CODE TO CHANGE *******////////////

			materials.push_back(material);

			cout << "material: " << material.id << endl;
			cout << material.ambient[0] << ", " << material.ambient[1] << ", " << material.ambient[2] << endl;
			cout << material.diffuse[0] << ", " << material.diffuse[1] << ", " << material.diffuse[2] << endl;
			cout << material.specular[0] << ", " << material.specular[1] << ", " << material.specular[2] << endl;
			cout << material.emission[0] << ", " << material.emission[1] << ", " << material.emission[2] << endl;
			cout << material.shininess << endl;
			cout << material.shadow << endl;
			cout << material.reflect << endl << endl;
		}

		/////////////////////////////////////////////////////////////////////////////////
		//	parse a Sphere here, the format is
		//
		//	Sphere radius positionX positionY positionZ materialID	 
		//	## radius is a double value
		//	## positionX, Y and Z are double values
		//	## materialID is an integer refering to a material
		//
		/////////////////////////////////////////////////////////////////////////////////
		// sphere
		else if(line.substr(0,6).compare("Sphere") == 0) {

			Sphere sphere;			

			//////////*********** START OF CODE TO CHANGE *******////////////
			istringstream(currentLine) >> dc >> sphere.radius >> sphere.center[0] >> sphere.center[1] >> sphere.center[2] >> sphere.materialID;
			//////////*********** END OF CODE TO CHANGE *******////////////

			spheres->push_back(sphere);

			cout << "Sphere " << sphere.radius << " " << sphere.center[0] << " " << sphere.center[1] << " " << sphere.center[2] << " " << sphere.materialID << endl << endl;
		}

		/////////////////////////////////////////////////////////////////////////////////
		//	parse a Plane of the form Ax+By+Cz+D=0 here, the format is
		//
		//	Plane A B C D materialID	 
		//	## A, B, C, D are double values
		//	## materialID is an integer refering to a material
		//
		/////////////////////////////////////////////////////////////////////////////////
		// plane
		else if(line.substr(0,5).compare("Plane") == 0) {

			Plane plane;			

			//////////*********** START OF CODE TO CHANGE *******////////////
			istringstream(currentLine) >> dc >> plane.params[0] >> plane.params[1] >> plane.params[2] >> plane.params[3] >> plane.materialID;

			//////////*********** END OF CODE TO CHANGE *******////////////

			planes->push_back(plane);

			cout << "Plane " << plane.params[0] << " " << plane.params[1] << " " << plane.params[2] << " " << plane.params[3] << " " << plane.materialID << endl << endl;
		}

		/////////////////////////////////////////////////////////////////////////////////
		//	parse a Camera here, the format is
		//
		//	Camera								
		//	positionX positionY positionZ			## 3 double values
		//	lookAtX lookAtY lookAtZ					## 3 double values
		//	upX upY upZ								## 3 double values
		//	verticalFieldOfView	aspect				## 2 double values
		//	zNear zFar								## 2 double value
		//
		/////////////////////////////////////////////////////////////////////////////////
		// camera
		else if(line.substr(0,6).compare("Camera") == 0) {

			//////////*********** START OF CODE TO CHANGE *******////////////

			// insert your code here and write result in variable camera!
			in.getline(currentLine,256);
			istringstream(currentLine) >> camera->position[0] >> camera->position[1] >> camera->position[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> camera->center[0] >> camera->center[1] >> camera->center[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> camera->up[0] >> camera->up[1] >> camera->up[2];
			in.getline(currentLine,256);
			istringstream(currentLine) >> camera->fovy >> camera->aspect;
			in.getline(currentLine,256);
			istringstream(currentLine) >> camera->zNear >> camera->zFar;
			//////////*********** END OF CODE TO CHANGE *******////////////			

			cout << "Camera: " << endl;
			cout << camera->position[0] << ", " << camera->position[1] << ", " << camera->position[2]<< endl;
			cout << camera->center[0] << ", " << camera->center[1] << ", " << camera->center[2]<< endl;
			cout << camera->up[0] << ", " << camera->up[1] << ", " << camera->up[2]<< endl;
			cout << camera->fovy << ", " << camera->aspect << endl;
			cout << camera->zNear << ", " << camera->zFar << endl << endl;

		}

		/////////////////////////////////////////////////////////////////////////////////
		//	parse image resolution, the format is
		//
		//	Resolution resX resY	 
		//	## resX resY are integer values
		//
		/////////////////////////////////////////////////////////////////////////////////
		// resolution
		else if (line.substr(0,10).compare("Resolution") == 0) {

			//////////*********** START OF CODE TO CHANGE *******////////////

			// insert your code here and write result in resolution[0] and resolution[1]
			istringstream(currentLine) >> dc >> resolution[0] >> resolution[1];

			//////////*********** END OF CODE TO CHANGE *******////////////	

			//cout << "Resolution : " << resolution[0] << "x" << resolution[1] << endl << endl;
		}
	}

	in.close();

	// assign correct materials to primitives
	foreach(sphere, (*spheres), vector<Sphere>) { 
		foreach(material, materials, vector<Material>) {
			if(material->id == sphere->materialID){
				sphere->material = *material;
			}
		}
	}

	// assign correct materials to primitives
	foreach(plane, (*planes), vector<Plane>) { 
		foreach(material, materials, vector<Material>) {
			if(material->id == plane->materialID){
				plane->material = *material;
			}
		}
	}

	cout << "====================================================" << endl;
	cout << "==================== parsed file ===================" << endl;
	cout << "====================================================" << endl;

	return true;
}
