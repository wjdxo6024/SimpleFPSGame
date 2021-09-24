#include "stdafx.h"
#include "Snow.h"


Snow::Snow()
{
}


Snow::~Snow()
{
}

Snow::Snow(BoundingBox* boundingBox, int numParticles)
{
	m_boundingBox = *boundingBox;
	m_size = 0.8f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Snow::InitSnow(BoundingBox* boundingBox, int numParticles)
{
	m_boundingBox = *boundingBox;
	m_size = 0.8f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Snow::resetParticle(Attribute* attribute)
{
	// y축이 z축이고 z축이 y축임.

	attribute->isAlive = true;

	GetRandomVector(&attribute->position,
		m_boundingBox.GetMin(),
		m_boundingBox.GetMax());

	attribute->position.y = 0.0f; //m_boundingBox.GetMax()->y;
	//attribute->position.x = 0.0f;
	attribute->position.z = m_boundingBox.GetMax()->z;

	attribute->velocity.x = GetRandomFloat(0.0f, 1.0f) * -1.0f;
	attribute->velocity.y = GetRandomFloat(-1.0f, 1.0f) * -1.0f;
	attribute->velocity.z = GetRandomFloat(0.0f, 1.0f) * -3.0f;

	attribute->color = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
}
void Snow::OnUpdate(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = m_particles.begin(); i != m_particles.end(); i++)
	{
		i->position += i->velocity * timeDelta;

		if (m_boundingBox.isPointInside(i->position) == false)
		{
			resetParticle(&(*i));
		}
	}
}

void Snow::preRender()
{
	Particle::preRender();

	m_pd3dDevice->SetFVF(D3DFVF_SNOW);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixScaling(&matWorld, 10.0f, 10.0f, 10.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}