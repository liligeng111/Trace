#include <cmath>

#include "..\ui\TraceUI.h";
#include "light.h"

extern TraceUI* traceUI;

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
	ray r = ray(P, getDirection(P));
	double t = 1.0 / NORMAL_EPSILON; //large
    return scene->root->shadowAttenuation(r, t);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	
	double t = (position - P).length();
	double c = traceUI->get_slidervalue(ATTENU_CONSTANT);
	double l = traceUI->get_slidervalue(ATTENU_LINEAR);
	double q = traceUI->get_slidervalue(ATTENU_QUAD);
	double d = 1.0 / (c + l * t + q * t * t);
	if (d < 1) d = 1.0;

	return d;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
	ray r = ray(P, getDirection(P));
	double t = (position - P).length();
    return scene->root->shadowAttenuation(r, t);
}
