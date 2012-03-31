#ifndef __TORUS_H__
#define __TORUS_H__

#include "../scene/scene.h"

class Torus
	: public MaterialSceneObject
{
public:
	Torus( Scene *scene, Material *mat, 
			double r = 0.3 )
		: MaterialSceneObject( scene, mat ), radius(r)
	{
	}

	virtual bool intersectLocal( const ray& r, isect& i ) const;
	virtual bool hasBoundingBoxCapability() const { return true; }

    virtual BoundingBox ComputeLocalBoundingBox()
    {
        BoundingBox localbounds;

		localbounds.min = vec3f(-radius - 1, -radius - 1, -radius);
		localbounds.max = vec3f(radius + 1, radius + 1, radius);
        return localbounds;
    }

protected:
    
	double radius;

};

#endif // __CONE_H__
