#include "stdafx.h"
#include "Object.h"

Object::Object()
{

}


Object::~Object()
{

}

void Object::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light)
{

}

void Object::OnRender(Camera& camera)
{

}

void Object::OnUpdate()
{

}

void Object::OnRelease()
{
	
}

std::pair<BOOL, float> Object::IntersectRay(D3DXVECTOR3* Ray)
{
	return std::pair<BOOL, float>(FALSE, 0.0f);
}