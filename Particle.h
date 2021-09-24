#pragma once
#include "Object.h"

struct ParticleObject
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
	float size;
};

#define D3DFVF_SNOW (D3DFVF_XYZ | D3DFVF_PSIZE)
#define D3DFVF_FIREWORK (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)
#define D3DFVF_PARTICLEGUN (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)
#define D3DFVF_PARTICLE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)

struct Attribute
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 acceleration;
	float lifeTime;
	float age;
	D3DXCOLOR color;
	D3DXCOLOR colorFade;
	bool isAlive;
};

class Particle :
	public Object
{
public:
	Particle();
	~Particle();

	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const char* texFileName);
	virtual void reset();
	virtual void resetParticle(Attribute* attribute) = 0;
	virtual void addParticle();

	virtual void OnUpdate(float timeDelta);

	virtual void preRender();
	virtual void OnRender();
	virtual void postRender();

	virtual void OnRelease();

	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticles();

protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DXVECTOR3 m_origin;
	BoundingBox m_boundingBox;
	float m_emitRate;
	float m_size;
	LPDIRECT3DTEXTURE9 m_tex;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	std::list<Attribute> m_particles;
	int m_maxParticles;

	DWORD m_vbSize;
	DWORD m_vbOffset;
	DWORD m_vbBatchSize;

};

