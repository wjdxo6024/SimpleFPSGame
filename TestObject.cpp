#include "stdafx.h"
#include "TestObject.h"
#include "MyD3DUtil.h"

TestObject::TestObject()
{
}


TestObject::~TestObject()
{
}

HRESULT TestObject::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pd3dDevice->CreateVertexBuffer(m_Vertices.size() * sizeof(ModelVertex), D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEX, D3DPOOL_MANAGED, &pVB, 0);

	ModelVertex* paVertices;
	pVB->Lock(0, 0, (VOID**)&paVertices, 0);
	memcpy(paVertices, &m_Vertices[0], sizeof(ModelVertex) * m_Vertices.size());
	pVB->Unlock();

	return S_OK;
}

void TestObject::OnRender()
{
	/*D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, m_sizeX, m_sizeY, m_sizeZ);

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, m_x, m_y, m_z);*/
	//-90도 회전하는 이유는 블렌더로 파일을 추출 할 시 90도 회전하여 나타남.
	//만약 성능을 높이고 싶으면 블렌더 파일에서 직접 -90도 회전 할 것.
	D3DXMATRIXA16 matRotationX;
	D3DXMATRIXA16 matRotationY;
	D3DXMatrixRotationX(&matRotationX, D3DXToRadian(-90));

	/*D3DXMATRIXA16 matWorld = matScale * matRotationX *matTrans;*/
	D3DXMATRIXA16 matWorld = matRotationX;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Vertices.size());

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (pVB != NULL)
		pVB->Release();
}

void TestObject::OnUpdate()
{

}

void TestObject::OnRelease()
{
}