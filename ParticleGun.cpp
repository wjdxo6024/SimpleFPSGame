#include "stdafx.h"
#include "ParticleGun.h"


ParticleGun::ParticleGun()
{
}


ParticleGun::~ParticleGun()
{
}

void ParticleGun::InitGun(Camera* camera)
{
	m_size = 0.8f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;

	m_camera = camera;
	
	m_Axis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void ParticleGun::resetParticle(Attribute* attribute)
{
	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	attribute->isAlive = true;

	D3DXVECTOR3 cameraPos;
	cameraPos = m_camera->GetEye();

	D3DXVECTOR3 cameraDir;
	cameraDir = m_camera->GetAt();

	// ±¤¼±ÃÑ ¹æÇâ °è»ê
	attribute->position = D3DXVECTOR3(cameraPos.x / 10 - (cameraPos.x - cameraDir.x), (cameraPos.y) / 10 - (cameraPos.y - cameraDir.y) , cameraPos.z / 10 - (cameraPos.z - cameraDir.z)); // camera_z -> y, camera_y -> z
  	attribute->velocity = D3DXVECTOR3(-(cameraPos.x - cameraDir.x), -(cameraPos.y - cameraDir.y),  -(cameraPos.z - cameraDir.z)) * 5.0f;
	attribute->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	attribute->age = 0.0f;
	attribute->lifeTime = 3.0f; // ¼ö¸í 1ÃÊ
}

void ParticleGun::OnUpdate(float timeDelta, D3DXVECTOR3 axis)
{
	m_Axis = axis;
	if (GetAsyncKeyState(VK_LBUTTON))
		addParticle();

	std::list<Attribute>::iterator i;

	for (i = m_particles.begin(); i != m_particles.end(); i++)
	{
		i->position += i->velocity * timeDelta;

		i->age += timeDelta;

		if (i->age > i->lifeTime)
			i->isAlive = false;
	}

	removeDeadParticles();
}

void ParticleGun::preRender()
{
	Particle::preRender();

	m_pd3dDevice->SetFVF(D3DFVF_PARTICLEGUN);
	D3DXMATRIXA16 matWorld;
	D3DXMatrixScaling(&matWorld, 10.0f, 10.0f, 10.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void ParticleGun::postRender()
{
	Particle::postRender();

}