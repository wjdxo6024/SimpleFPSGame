#pragma once
#include <d3dx9.h>

class CGround
{
private:
	DWORD m_dwNumVertices, m_dwNumIndices;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	int m_nCol, m_nRow;
	D3DMATERIAL9 mtrl;

public:
	void Create(LPDIRECT3DDEVICE9 pd3dDevice, int nRow, int nCol, float fSize);
	void OnRedner();
	void OnRelease();

public:
	CGround();
	~CGround();
};

