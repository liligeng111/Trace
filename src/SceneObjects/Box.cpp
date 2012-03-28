 #include <cmath>
#include <assert.h>

#include "Box.h"

int max_of(const double* d)
{
	if (d[0] > d[1])
	{
		if (d[0] > d[2]) return 0;
		return 2;
	}
	else
	{
		if (d[1] > d[2]) return 1;
		return 2;
	}
}

int min_of(const double* d)
{
	if (d[0] < d[1])
	{
		if (d[0] < d[2]) return 0;
		return 2;
	}
	else
	{
		if (d[1] < d[2]) return 1;
		return 2;
	}
}

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	vec3f d = r.getDirection();
	vec3f p = r.getPosition();

	if (d.n[0] == 0.0) d.n[0] = RAY_EPSILON; 
	if (d.n[1] == 0.0) d.n[1] = RAY_EPSILON; 
	if (d.n[2] == 0.0) d.n[2] = RAY_EPSILON; 

	double min[3] = {};
	double max[3] = {};

	for (int j = 0; j < 3; j++)
	{
		double size = 0.5;
		if (d.n[j] > 0)
		{
			min[j] = (-size - p.n[j]) / d.n[j];
			max[j] = (size - p.n[j]) / d.n[j];
		}
		else
		{
			min[j] = (size - p.n[j]) / d.n[j];
			max[j] = (-size - p.n[j]) / d.n[j];
		}
	}

	int max_of_min = max_of(min);
	int min_of_max = min_of(max);

	//no intersection point
	if (max[min_of_max] < min[max_of_min]) return false;

	//t < 0
	if (max[min_of_max] < RAY_EPSILON) return false;
	
	i.obj = this;
	if (min[max_of_min] > 0)
	{
		i.t = min[max_of_min];
		int a = d[max_of_min] > 0 ? -1 : 1;
		switch (max_of_min)
		{
		case 0: 
			i.N = vec3f(a, 0, 0);
			return true;
		case 1:
			i.N = vec3f(0, a, 0);
			return true;
		default:
			i.N = vec3f(0, 0, a);
			return true;
		}
	}
	else
	{
		i.t = max[min_of_max];
		int a = d[max_of_min] > 0 ? 1 : -1;
		switch (max_of_min)
		{
		case 0: 
			i.N = vec3f(a, 0, 0);
			return true;
		case 1:
			i.N = vec3f(0, a, 0);
			return true;
		default:
			i.N = vec3f(0, 0, a);
			return true;
		}
	}

}
