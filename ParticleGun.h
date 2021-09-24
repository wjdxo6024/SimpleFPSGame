#pragma once
#include "Particle.h"

// TODO: ī�޶� ��ǥ�� ����� ��� ����� ������ �ȵ�.

class ParticleGun :
	public Particle
{
public:
	ParticleGun();
	~ParticleGun();

	void InitGun(Camera* camera);
	void resetParticle(Attribute* attribute);
	void OnUpdate(float timeDelta, D3DXVECTOR3 axis);
	void preRender();
	void postRender();

private:
	Camera* m_camera;
	D3DXVECTOR3 m_Axis;
};

