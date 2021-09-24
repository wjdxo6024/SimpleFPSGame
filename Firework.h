#pragma once
#include "Particle.h"
class Firework :
	public Particle
{
public:
	Firework();
	~Firework();
	void InitFirework(D3DXVECTOR3* origin, int numParticles);
	void resetParticle(Attribute* attribute);

	void OnUpdate(float timeDelta);
	void preRender();
	void postRender();

	void setPos(float x, float y, float z);
};

