#pragma once
#include "MyDef.h"
#include "MyD3DUtil.h"
#include "Camera.h"

class Frustum
{
public:
	Frustum();
	~Frustum();
	// 카메라 * 프로젝션 행렬을 입력받아 6개의 평면을 만든다.
	BOOL Make(D3DXMATRIXA16* pmatViewProj);

	// 한 점 v가 절두체 안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	BOOL IsIn(D3DXVECTOR3* pv);

	// 중심과 반지름을 갖는 경계구가 절두체 안에 있으면 TRUE, 아니면 FALSE를 반환한다.
	BOOL IsInSphere(D3DXVECTOR3* pv, float radius);

	// 절두체를 화면에 그려준다.
	BOOL Draw(LPDIRECT3DDEVICE9 pDev);

	// 현재 카메라의 월드 좌표를 얻어준다.
	D3DXVECTOR3* GetPos() { return &m_vPos; }

private:
	D3DXVECTOR3 m_vtx[8];
	D3DXVECTOR3 m_vPos;
	D3DXPLANE m_plane[6];

};
