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

#include <Raytracer.h>
#include <Operations.h>

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////

void	
	Raytracer::raytraceScene(	const char *filename,
	const char *depth_filename,
	std::vector<PointLight> *lights, 
	std::vector<Plane> *planes,	
	std::vector<Sphere> *spheres,
	Camera *camera, int *resolution	)
{

	// pixel color buffer
	unsigned char *colorbuffer = new unsigned char[resolution[0]*resolution[1]*3];
	for(int i=0; i<3*resolution[0]*resolution[1]; i++) {
		colorbuffer[i] = (unsigned char)0;
	}
	// depth buffer
	double *depthbuffer = new double[resolution[0]*resolution[1]];
	for(int i=0; i<resolution[0]*resolution[1]; i++) {
		depthbuffer[i] = 1.0;
	}

	// the magical ray setup
	Vec3 f(	camera->center[0]-camera->position[0], 
		camera->center[1]-camera->position[1], 
		camera->center[2]-camera->position[2]);
	Vec3 up(camera->up[0],camera->up[1],camera->up[2]);
	// normalized up vector
	up.normalize();
	Vec3 right = f.cross(up);
	// normalized vector pointing to the side
	right.normalize();

	// step for each pixel
	double stepx = 2*tan(deg2rad(camera->fovy)*camera->aspect/2.0)*f.length() / resolution[0];
	double stepy = 2*tan(deg2rad(camera->fovy)/2.0)*f.length() / resolution[1];

	for(int y=0; y<resolution[1]; y++ ) {
		for(int x=0; x<resolution[0]; x++) {

			//////////////////////////////////////////////////////////////////////////////////
			//	the following lines implement a simple orthographic camera
			//	you have to modify the code for generating pespective rays
			//	i.e. all rays (theoretical) origin from the camera position and 
			//	propagate into the pixelLookAt
			//
			//	make sure that the ray's origin is not the camera center but 
			//	on the near plane. the direction of the ray should be a vector,
			//	that is computed as pointOnFarPlane-pointOnNearPlane, i.e. 
			//	adding it to the t parameter in the ray equation 
			//	point = pointOnNearPlane + t*rayDirection
			//	is always between 0 and 1. This will be the value in the depth
			//	buffer.
			//////////////////////////////////////////////////////////////////////////////////
			// generate the appropriate ray for this pixel

			//////////*********** START OF CODE TO CHANGE *******////////////

			int alpha = x-resolution[0]/2;
			int beta  = y-resolution[1]/2;
			Vec3 pixelLookAt(	camera->center[0] + alpha*stepx*right[0] + beta*stepy*up[0],
				camera->center[1] + alpha*stepx*right[1] + beta*stepy*up[1],
				camera->center[2] + alpha*stepx*right[2] + beta*stepy*up[2]);			


			Vec3 pixelStart(	camera->position[0],// + alpha*stepx*right[0] + beta*stepy*up[0],
				camera->position[1],// + alpha*stepx*right[1] + beta*stepy*up[1],
				camera->position[2]);// + alpha*stepx*right[2] + beta*stepy*up[2]);

			Vec3 rayDirection(	pixelLookAt[0]-pixelStart[0], 
				pixelLookAt[1]-pixelStart[1], 
				pixelLookAt[2]-pixelStart[2]);
			rayDirection.normalize();

			Ray pixelRay(	pixelStart[0] + camera->zNear*rayDirection[0], 
				pixelStart[1] + camera->zNear*rayDirection[1],
				pixelStart[2] + camera->zNear*rayDirection[2],
				(camera->zFar-camera->zNear)*rayDirection[0],
				(camera->zFar-camera->zNear)*rayDirection[1],
				(camera->zFar-camera->zNear)*rayDirection[2]	);

			//////////*********** END OF CODE TO CHANGE *******////////////

			//////////////////////////////////////////////////////////////////////////////////

			int currentRayRecursion=0;
			double red=0.0, green=0.0, blue=0.0, depth=1.0;

			// calculate the pixel value by shooting the ray into the scene
			traceRay(	pixelRay, lights, planes, spheres, camera, &currentRayRecursion, 
				&red ,&green, &blue, &depth);

			// depth test
			if( (depth<depthbuffer[x+y*resolution[0]]) && (depth>=0.0)) {

				// red
				colorbuffer[3*(x+y*resolution[0])]=(unsigned char)(255.0*red);
				// green
				colorbuffer[3*(x+y*resolution[0])+1]=(unsigned char)(255.0*green);
				// blue
				colorbuffer[3*(x+y*resolution[0])+2]=(unsigned char)(255.0*blue);

				// depth 
				depthbuffer[x+y*resolution[0]] = depth;
			}
		}
	}
	writeImage(filename, colorbuffer, resolution);

	unsigned char *depthbuffer_uc = new unsigned char[resolution[0]*resolution[1]];
	for(int i=0; i<resolution[0]*resolution[1]; i++) {
		depthbuffer_uc[i] = (unsigned char)(255.0*depthbuffer[i]);
	}
	writeImage(depth_filename, depthbuffer_uc, resolution, true);

	delete [] depthbuffer_uc;
	delete [] depthbuffer;
	delete [] colorbuffer;
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//	this function shoots a ray into the scene and calculates
//	the closest point of intersection
//	
//	if the ray hits a surface that is reflecting the rays have to be recursively
//	shot into the scene starting from that point into the proper direction
//	(using Snell's law of reflection). you need to implement the ray reflection 
//	and color blending between the reflected color and the local color (i.e.
//	direct lighting and indirect lighting). weight the reflection term by
//	1-material.reflect
//
/////////////////////////////////////////////////////////////////////////////////
void	
	Raytracer::traceRay(	Ray pixelRay, 
	std::vector<PointLight> *lights, 
	std::vector<Plane> *planes,	
	std::vector<Sphere> *spheres, 
	Camera *camera,
	int *currRayRecursion,
	double *red , double *green, double *blue, double *depth )
{

	// did it intersect at all?
	bool bIntersect=false;
	// depth of the closest intersection
	double intersectDepth=*depth;
	// 3D position of the closest intersection
	double intersectPos[3];
	// normal at the point of closest intersection
	double intersectNormal[3];
	// the material of the object at the intersection point
	Material intersectMaterial;

	// calculate intersection of ray with all planes
	foreach(p, (*planes), vector<Plane>) {

		double iDepth, iPos[3], iNormal[3];

		if( p->intersect(	pixelRay, &iDepth, 
			&iPos[0], &iPos[1], &iPos[2],
			&iNormal[0], &iNormal[1], &iNormal[2]))
		{

			// depth test
			if(iDepth<intersectDepth) {
				bIntersect = true;
				intersectDepth = iDepth;
				intersectPos[0]=iPos[0]; intersectPos[1]=iPos[1]; intersectPos[2]=iPos[2];
				intersectNormal[0]=iNormal[0]; intersectNormal[1]=iNormal[1]; intersectNormal[2]=iNormal[2];
				intersectMaterial = p->material;
			}
		}		
	}

	// calculate intersection of ray with all spheres
	foreach(s, (*spheres), vector<Sphere>) {

		double iDepth, iPos[3], iNormal[3];

		if( s->intersect(	pixelRay, &iDepth, 
			&iPos[0], &iPos[1], &iPos[2],
			&iNormal[0], &iNormal[1], &iNormal[2]))
		{

			// depth test
			if(iDepth<intersectDepth) {
				bIntersect = true;
				intersectDepth = iDepth;
				intersectPos[0]=iPos[0]; intersectPos[1]=iPos[1]; intersectPos[2]=iPos[2];
				intersectNormal[0]=iNormal[0]; intersectNormal[1]=iNormal[1]; intersectNormal[2]=iNormal[2];
				intersectMaterial = s->material;
			}
		}
	}

	if(bIntersect) {

		*depth = intersectDepth;

		shade(	intersectPos[0], intersectPos[1], intersectPos[2],
			intersectNormal[0], intersectNormal[1], intersectNormal[2],
			intersectMaterial, camera, 
			lights, planes,	spheres,
			red, green, blue);

		/////////////////////////////////////////////////////////////////////////////////////
		// recurse for reflections
		if( (*currRayRecursion<MAX_RAY_RECURSION) && (intersectMaterial.reflect>0.0) ) {				
			(*currRayRecursion) += 1;

			//////////*********** START OF CODE TO CHANGE *******////////////
			Vec3 i(pixelRay.direction[0], pixelRay.direction[1], pixelRay.direction[2]);
			Vec3 n(intersectNormal[0],intersectNormal[1], intersectNormal[2]);
			double idotn = 2* i.dot(n);
			Vec3 reflect(i[0]-idotn*n[0], i[1]-idotn*n[1], i[2]-idotn*n[2]);
			Ray rReflect(intersectPos[0],intersectPos[1], intersectPos[2], reflect[0], reflect[1], reflect[2]);
			double r = 0.0;
			double g = 0.0;
			double b = 0.0;
			double d = 9000.0;
			this->traceRay(rReflect, lights,planes,spheres,camera,currRayRecursion,&r,&g,&b,&d);
			*red = (1- intersectMaterial.reflect)*(*red) + intersectMaterial.reflect*r;
			*green = (1- intersectMaterial.reflect)*(*green) + intersectMaterial.reflect*g;
			*blue = (1- intersectMaterial.reflect)*(*blue) + intersectMaterial.reflect*b;
			//////////*********** END OF CODE TO CHANGE *******////////////

		}
		/////////////////////////////////////////////////////////////////////////////////////

		// clip colors between 0.0 and 1.0
		*red = (*red > 1.0) ? 1.0 : *red;
		*red = (*red < 0.0) ? 0.0 : *red;
		*green = (*green > 1.0) ? 1.0 : *green;
		*green = (*green < 0.0) ? 0.0 : *green;
		*blue = (*blue > 1.0) ? 1.0 : *blue;
		*blue = (*blue < 0.0) ? 0.0 : *blue;

	}
}

///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//	here you need to calculate the proper shading of a given point at posX, posY,
//	posZ and corresponding normal normalX, normalY and normalZ
//	
//	the material of the point is given as material, the camera as camera
//	and all objects in the scenes as planes and spheres
//	
//	lights is a list of all the light source, make sure to calculate the 
//	lighting for each one of them and add the color values, the material
//	emission part is only added once!
// 
//	write the color values in red, green and blue
// 
//	you also have to add the code that calculates if the point is in the 
//	shadow for each light source and weight the computed color value
//	bu 1-material.shadow
/////////////////////////////////////////////////////////////////////////////////
void	
	Raytracer::shade(	double posX, double posY, double posZ,
	double normalX, double normalY, double normalZ,
	Material material, Camera *camera, 
	std::vector<PointLight> *lights, 
	std::vector<Plane>		*planes,	
	std::vector<Sphere>		*spheres,
	double *red, double *green, double *blue)
{
	// calculate emissive part here
	*red   = material.emission[0];
	*green = material.emission[1];
	*blue  = material.emission[2];
	double ambient[3], diffuse[3], specular[3], total[3];//r,g,b
	double dc;

	foreach(light, (*lights), vector<PointLight>) {

		//////////*********** START OF CODE TO CHANGE *******////////////
		// add color value of this light source to color

		//Phong Lighting
		Vec3 n(normalX, normalY, normalZ);
		Vec3 l(light->position[0] - posX, light->position[1] - posY, light->position[2] - posZ);
		double  attenuation;
		n.normalize();
		// include ambient, diffuse and specular
		// make sure to add the light attenuation
		// effect for the diffuse and specular term
		// attenuation = 1/ar^2+br+c
		attenuation = 1 / (light->attenuation[2] * (l.length() * l.length()) + light->attenuation[1] * l.length() + light->attenuation[0]);
		l.normalize();
		double ldotn = l.dot(n);
		if(ldotn<0)
		{
			ldotn=0;
		}
		Vec3 r(l[0]- 2*ldotn*n[0], l[1] - 2*ldotn*n[1], l[2] - 2*ldotn*n[2]);
		r.normalize();
		Vec3 v(camera->position[0]-posX,camera->position[1]-posY, camera->position[2]-posZ);
		v.normalize();
		double vdotr = v.dot(r);

		for (int i = 0; i < 3; i++) 
		{
			//ambient = ka * Ia
			ambient[i] = material.ambient[i] * light->ambient[i];
			//diffuse=kd*IL*(N.l)*attenuation
			diffuse[i] = (material.diffuse[i] * (light->diffuse[i])* (ldotn)) * attenuation ;
			//specular = ks*IL*(v.r)^nshiny
			specular[i] = (material.specular[i] * (light->specular[i])* pow(vdotr, material.shininess))* attenuation;
		}
		/////////////////////////////////////////////////////////////////////////////////////
		// shoot a ray to every light source to see if the point is in shadow

		//////////*********** END OF CODE TO CHANGE *******////////////
		bool inShadow=false;
		if(material.shadow != 0.0) {

			//////////*********** START OF CODE TO CHANGE *******////////////

			// add your code for shadows here
			Ray shadowRay(posX,posY,posZ,(light->position[0] - posX),(light->position[1] - posY),(light->position[2] - posZ));
			// calculate intersection of ray with all spheres
			bool intersected = false;
			foreach(s, (*spheres), vector<Sphere>) 
			{
				if (s->intersect(shadowRay, &dc, &dc, &dc,&dc, &dc, &dc, &dc) && intersected==false)
				{
					for(int i =0;i<3;i++)
					{
						diffuse[i] *= (1-material.shadow);
						ambient[i] *= (1-material.shadow);
					}
					inShadow = true;
					intersected = true;
				}
			}
			intersected = true;
			//////////*********** END OF CODE TO CHANGE *******////////////
		}

		// end of shadows
		/////////////////////////////////////////////////////////////////////////////////////

		//////////*********** START OF CODE TO CHANGE *******////////////

		// add calculated color to final color
		if (!inShadow) 
		{


		}
		*red += (ambient[0] + diffuse[0] + specular[0]);
		*green +=(ambient[1] + diffuse[1] + specular[1]);
		*blue += (ambient[2] + diffuse[2] + specular[2]);
		//////////*********** END OF CODE TO CHANGE *******////////////
	}	
}

///////////////////////////////////////////////////////////////////////////

bool	
	Raytracer::writeImage(	const char *filename, 
	unsigned char *imageBuffer, 
	int *resolution, 
	bool greyscale) {

		FILE *fp = fopen(filename,"wb");
		if (!fp) {
			printf("Unable to open file '%s'\n",filename);
			return false;
		}

		const int maxVal=255;

		fprintf(fp, "P6 ");
		fprintf(fp, "%d %d ", resolution[0], resolution[1]);
		fprintf(fp, "%d", maxVal);
		putc(13,fp);

		int numChannels=3;
		if(greyscale)
			numChannels=1;

		for(int y=resolution[1]-1; y>=0; y-- ) {
			for(int x=0; x<resolution[0]; x++) {
				// red or depth
				putc(imageBuffer[numChannels*(x+y*resolution[0])],fp);
				if(!greyscale) {
					// green
					putc(imageBuffer[numChannels*(x+y*resolution[0])+1],fp);
					// blue
					putc(imageBuffer[numChannels*(x+y*resolution[0])+2],fp);
				} else {
					// depth
					putc(imageBuffer[numChannels*(x+y*resolution[0])],fp);
					// depth
					putc(imageBuffer[numChannels*(x+y*resolution[0])],fp);
				}
			}
		}
		fclose(fp);
		return true;
}

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
