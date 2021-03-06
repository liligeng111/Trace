// The main ray tracer.

#include <Fl/fl_ask.h>
#include <fl/fl.h>
#include "ui\TraceUI.h";
#include "ui\TraceGLWindow.h"
#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"

extern TraceUI* traceUI;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	ray r( vec3f(0,0,0), vec3f(0,0,0) );
	scene->getCamera()->rayThrough( x,y,r );
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), maxDepth ).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth)
{
	isect i;

	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		const Material& m = i.getMaterial();

		vec3f I = m.shade(scene, r, i);
		if (depth <= 0) return I;
		
		//reflection
		const double NL = -i.N.dot(r.getDirection());
		vec3f ref = i.N * (2 * NL) + r.getDirection();
		ray R = ray(r.at(i.t), ref);
		I += (m.kr.time(traceRay(scene, R, thresh, depth - 1))).clamp(); 

		//refraction		
		if (abs(m.index - 1.0) < NORMAL_EPSILON && m.kt.iszero()) return I;
		double n;
		if (NL > 0)
		{
			n = 1.0 / m.index;
			double LONG_TERM = n * NL - sqrt(1 - n * n * (1 - NL * NL));
			ray T = ray(r.at(i.t), (i.N * LONG_TERM + r.getDirection() * n));
			I += (m.kt.time(traceRay(scene, T, thresh, depth - 1))).clamp();
		}
		else
		{
			n = m.index;
			if (1 - n * n * (1 - NL * NL) < 0)
			{
				return I;
			}			
			double LONG_TERM = -(n * (-NL) - sqrt(1 - n * n * (1 - NL * NL)));
			ray T = ray(r.at(i.t), (i.N * LONG_TERM + r.getDirection() * n));
			I += (m.kt.time(traceRay(scene, T, thresh, depth - 1))).clamp();
		} 
		return I;
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;
	col = 0;

	maxDepth = 0;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete [] col;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	col = new vec3f[buffer_width * buffer_height];

	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
		delete [] col;		
		col = new vec3f[buffer_width * buffer_height];
	}
	memset( buffer, 0, w*h*3 );

}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	int n = i + j * buffer_width;
	col[n] = trace( scene,x,y );

	unsigned char *pixel = buffer + (n) * 3;

	pixel[0] = (int)( 255.0 * col[n][0]);
	pixel[1] = (int)( 255.0 * col[n][1]);
	pixel[2] = (int)( 255.0 * col[n][2]);
}

void RayTracer::antiAliasing(int part)
{
	char title[256];
	double x_gap = 0.5 / double(buffer_width);
	double y_gap = 0.5 / double(buffer_height);

	int end = (part == 0) ? buffer_width / 2 : buffer_width - 1;
	int start = (part == 0) ? 1 : buffer_width / 2;

	for(int i = start; i < end; i++)
	{
		for (int j = 1; j < buffer_height - 1; j++)
		{
			bool check = false;
			double d = 0.1;
			int n = i + j * buffer_width;
			if ((col[n] - col[n + 1]).length() > d) check = true;
			if ((col[n] - col[n - 1]).length() > d) check = true;
			if ((col[n] - col[n + buffer_height]).length() > d) check = true;
			if ((col[n] - col[n - buffer_height]).length() > d) check = true;

			if (check)
			{
				double x = double(i)/double(buffer_width);
				double y = double(j)/double(buffer_height);
				vec3f color = trace( scene,x,y ) +  trace( scene,x + x_gap,y ) +  trace( scene,x - x_gap,y ) +  trace( scene,x,y + y_gap ) + trace( scene,x,y - y_gap );
				color /= 5;
				
				unsigned char *pixel = buffer + (i + j * buffer_width) * 3;
				pixel[0] = (int)( 255.0 * color[0]);
				pixel[1] = (int)( 255.0 * color[1]);
				pixel[2] = (int)( 255.0 * color[2]);
			}
		}
	}
}