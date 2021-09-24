#include "stdafx.h"
#include "Particle.h"


Particle::Particle()
{
}


Particle::~Particle()
{
}

void Particle::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const char* texFileName)
{
	m_pd3dDevice = pd3dDevice;

	m_pd3dDevice->CreateVertexBuffer(m_vbSize * sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_PARTICLE,
		D3DPOOL_DEFAULT, &m_pVB, 0);

	D3DXCreateTextureFromFileA(m_pd3dDevice, texFileName, &m_tex);
}

void Particle::reset()
{
	std::list<Attribute>::iterator i;
	for (i = m_particles.begin(); i != m_particles.end(); i++)
	{
		resetParticle(&(*i));
	}
}
void Particle::resetParticle(Attribute* attribute)
{

}

void Particle::addParticle()
{
	Attribute attribute;

	resetParticle(&attribute);

	m_particles.push_back(attribute);
}

void Particle::OnUpdate(float timeDelta)
{

}

void Particle::preRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_size));
	m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
void Particle::OnRender()
{
	if (!m_particles.empty())
	{
		preRender();

		m_pd3dDevice->SetTexture(0, m_tex);
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(ParticleObject));

		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		ParticleObject* v = 0;

		m_pVB->Lock(m_vbOffset * sizeof(ParticleObject), m_vbBatchSize * sizeof(ParticleObject), (void**)&v, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		std::list<Attribute>::iterator i;
		for (i = m_particles.begin(); i != m_particles.end(); i++)
		{
			if (i->isAlive)
			{
				v->position = i->position;
				v->color = i->color;
				v->size = GetRandomFloat(1.0f, 1.0f);
				v++;
				numParticlesInBatch++;

				// 현재 단계가 모두 채워져 있는가?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					// 버텍스 버퍼로 복사된 마지막 단계의
					// 파티클들을 그린다.

					m_pVB->Unlock();

					m_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					// 단계가 그려지는 동안 다음 단계를
					// 파티클로 채운다.

					// 다음 단계의 처음 오프셋으로 이동한다.
					m_vbOffset += m_vbBatchSize;

					// 버텍스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 않는다.
					// 경계를 넘을 경우 처음부터 시작한다.
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(m_vbOffset * sizeof(ParticleObject), m_vbBatchSize * sizeof(ParticleObject), (void**)&v, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0;
				} // end if
			} // end if
		}// end for

		m_pVB->Unlock();
		if (numParticlesInBatch)
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		m_vbOffset += m_vbBatchSize;

		postRender();

	} // end if
} // end render()

void Particle::postRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool Particle::isEmpty()
{
	return m_particles.empty();
}

bool Particle::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = m_particles.begin(); i != m_particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if (i->isAlive)
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

void Particle::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = m_particles.begin();

	while (i != m_particles.end())
	{
		if (i->isAlive == false)
		{
			i = m_particles.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void Particle::OnRelease()
{
	if (m_pVB != NULL)
		m_pVB->Release();

	if (m_tex != NULL)
		m_tex->Release();
}