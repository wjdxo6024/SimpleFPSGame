#pragma once
#include "MyDef.h"
#include "MyD3DUtil.h"
#include "Camera.h"

class Frustum
{
public:
	Frustum();
	~Frustum();
	// ī�޶� * �������� ����� �Է¹޾� 6���� ����� �����.
	BOOL Make(D3DXMATRIXA16* pmatViewProj);

	// �� �� v�� ����ü �ȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL IsIn(D3DXVECTOR3* pv);

	// �߽ɰ� �������� ���� ��豸�� ����ü �ȿ� ������ TRUE, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL IsInSphere(D3DXVECTOR3* pv, float radius);

	// ����ü�� ȭ�鿡 �׷��ش�.
	BOOL Draw(LPDIRECT3DDEVICE9 pDev);

	// ���� ī�޶��� ���� ��ǥ�� ����ش�.
	D3DXVECTOR3* GetPos() { return &m_vPos; }

private:
	D3DXVECTOR3 m_vtx[8];
	D3DXVECTOR3 m_vPos;
	D3DXPLANE m_plane[6];

};
