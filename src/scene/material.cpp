#include "ray.h"
#include "material.h"
#include "light.h"
#include <iterator>

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.
	
	//intersection point
	vec3f point = r.at(i.t);

	vec3f I = ke;

	// TODO: ambient

	list<Light*>::const_iterator begin = scene->beginLights();
	list<Light*>::const_iterator end = scene->endLights();
	while (begin != end)
	{
		vec3f atten = (*begin)->shadowAttenuation(point) * (*begin)->distanceAttenuation(point);
		vec3f L = (*begin)->getDirection(point);
		double NL = i.N.dot(L);

		//diffuse
		I += (atten * NL).time(kd).clamp();

		//specular
		vec3f R = i.N * (2 * NL) - L;
		double RV = -R.dot(r.getDirection());
		//TODO: where is n£¿
		double n = 64;
		I += (atten * pow(RV, n)).time(ks).clamp();

		begin++;
	}

	return I;
}
