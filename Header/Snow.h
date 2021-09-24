#pragma once
#include "Particle.h"
class Snow :
	public Particle
{
public:
	Snow();
	~Snow();

	Snow(BoundingBox* boundingBox, int numParticles);
	void InitSnow(BoundingBox* boundingBox, int numParticles);
	void resetParticle(Attribute* attribue);
	void OnUpdate(float timeDelta);
	void preRender();
};

