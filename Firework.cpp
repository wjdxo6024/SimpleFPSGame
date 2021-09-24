#include "stdafx.h"
#include "Firework.h"


Firework::Firework()
{
}


Firework::~Firework()
{
}

void Firework::InitFirework(D3DXVECTOR3* origin, int numParticles)
{
	m_origin = *origin;
	m_size = 0.9f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Firework::resetParticle(Attribute* attribute)
{
	attribute->isAlive = true;
	attribute->position = m_origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	GetRandomVector(&attribute->velocity, &min, &max);

	D3DXVec3Normalize(&attribute->velocity, &attribute->velocity);

	attribute->velocity *= 1.0f;

	attribute->color = D3DXCOLOR(GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		1.0f);
	
	attribute->age = 0.0f;
	attribute->lifeTime = 0.1f;
}

void Firework::OnUpdate(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_particles.begin(); i != m_particles.end(); i++)
	{
		if (i->isAlive)
		{
			i->position += i->velocity * timeDelta;

			i->age += timeDelta;

			if (i->age > i->lifeTime)
				i->isAlive = false;
		}
	}
}

void Firework::preRender()
{
	Particle::preRender();

	m_pd3dDevice->SetFVF(D3DFVF_FIREWORK);

	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	/*D3DXMATRIXA16 matWorld;
	D3DXMatrixScaling(&matWorld, 10.0f, 10.0f, 10.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);*/
}

void Firework::postRender()
{
	Particle::postRender();

	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void Firework::setPos(float x, float y, float z)
{
	m_origin.x = x  / 10;
	m_origin.y = -y / 10;
	m_origin.z = z / 10;
}