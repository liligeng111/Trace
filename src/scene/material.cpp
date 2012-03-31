#include "ray.h"
#include "material.h"
#include "light.h"
#include "..\ui\TraceUI.h";
#include <iterator>

#define PI 3.141592653589793

extern TraceUI* traceUI;
double G(double a, double v)
{
	return v / (a - a * v + v);
}

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
	bool istransmissive = abs(index - 1.0) > NORMAL_EPSILON || !kt.iszero();
	vec3f rate = vec3f(1, 1, 1) - kt;

	vec3f I = ke;

	//ambient
	if (istransmissive) I += scene->ambient.time(ka).time(rate).clamp();
	else I += scene->ambient.time(ka).clamp();

	list<Light*>::const_iterator begin = scene->beginLights();
	list<Light*>::const_iterator end = scene->endLights();
	while (begin != end)
	{
		vec3f atten = (*begin)->shadowAttenuation(point) * (*begin)->distanceAttenuation(point);
		vec3f L = (*begin)->getDirection(point);
		double NL = i.N.dot(L);

		if (false && traceUI->get_buttonvalue(JITTER_B) && i.obj->isCaustic())
		{
			double a = 0.1;
			NL *= -1;
			double NR = -i.N.dot(r.getDirection());
			double Gvp = G(NL, a);
			double Gv = G(NR, a);
			double t = i.N.dot(r.getDirection() + L) / (r.getDirection() + L).length();
			double fr = (1 - Gv * Gvp) / PI + Gv * Gvp * (a / (1 + a * t * t - t * t) * (1 + a * t * t - t * t)) / (4 * PI * NL * NR);
			fr *= 0.1;
			//printf("%f\n", fr);
			I += (atten * fr).clamp();
		}
		else
		{
			//diffuse
			if (istransmissive) I += (atten * NL).time(kd).time(rate).clamp();
			else I += (atten * NL).time(kd).clamp();

			//specular
			vec3f R = i.N * (2 * NL) - L;
			double RV = -R.dot(r.getDirection());
		
			//TODO: where is n£¿
			double n = 64;
			I += (atten * pow(RV, n)).time(ks).clamp();
		}

		begin++;
	}

	return I;
}
