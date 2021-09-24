#pragma once
#include "MyDef.h"
#include "QuadTree.h"
#include "Camera.h"
#define MAX_TERRAIN_NAME 4
#define MAX_TERRAIN_TEX 1


class Terrain
{
public:
	Terrain();
	~Terrain();

	HRESULT OnInit(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvfScale, LPCSTR lpBMPFilename, LPCSTR lpTexFilename, Camera* pCamera);
	HRESULT OnRender();
	HRESULT OnUpdate();
	HRESULT OnRelease();

	TERRAINVERTEX* GetVertex(int x, int z) {
		return (m_pvHeightMap + x + z * m_cxHeight);
	}

	float GetHeight(int x, int y) { return (GetVertex(x, y))->p.y; }

	int GetCXDIB() { return m_cxHeight; }
	int GetCZDIB() { return m_czHeight; }

	float GetCXTerrain() { return m_cxHeight * m_vfScale.x; }
	float GetCYTerrain() { return m_cyHeight * m_vfScale.y; }
	float GetCZTerrain() { return m_czHeight * m_vfScale.z; }

	HRESULT Draw();

private:
	HRESULT LoadTextures(LPCSTR lpTexFilename);
	HRESULT BuildHeightMap(LPCSTR lpFilename);
	HRESULT BuildQuadTree();
	HRESULT CreateVIB();
	
private:
	int m_cxHeight;
	int m_czHeight;
	int m_cyHeight;

	D3DXVECTOR3 m_vfScale;
	TERRAINVERTEX* m_pvHeightMap;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DTEXTURE9 m_pTex;
	LPDIRECT3DTEXTURE9 m_pTexHeight;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	int m_nTriangles;
	QuadTree* m_pQuadTree;

	Camera* m_Camera;
	Frustum* m_Frustum;
	float m_fLODRatio;
};

