#pragma once
#include "Frustum.h"
#include "MyDef.h"

class QuadTree
{
	enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR};
	enum {EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT};
	enum QuadLocation {FRUSTUM_OUT = 0, // 절두체에 완전히 벗어남
	FRUSTUM_PARTIALLY_IN =1, // 절두체에 부분적으로 포함됨
	FRUSTUM_COMPLETELY_IN = 2, // 절두체에 완전히 포함됨
	FRUSTUM_UNKNOWN = -1}; // 모르겠음

public:
	QuadTree();
	~QuadTree();

	QuadTree(int cx, int cy);

	QuadTree(QuadTree* pParent);

	BOOL Build(TERRAINVERTEX* pHeightMap);

	int GenerateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio);

	int GenTriIndex(int nTris, LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio);

	// 코너 인덱스 값을 얻어온다.
	void GetCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = m_nCorner[0]; _1 = m_nCorner[1]; _2 = m_nCorner[2]; _3 = m_nCorner[3];
	}

	// 현재 노드가 출력이 가능한 노드인가?
	BOOL IsVisible(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio);

private:
	QuadTree* AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	BOOL SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	// 쿼드트리를 4개의 하위 트리로 부분분할한다.
	BOOL SubDivide();

	void Destroy();

	int IsInFrustum(TERRAINVERTEX* pHeightMap, Frustum* pFrustum);

	BOOL IsCulled() { return m_bCulled; }

	void FrustumCull(TERRAINVERTEX* pHeightMap, Frustum* pFrustum);

	float GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2);

	int GetLODLevel(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio);
	
	// 모든 m_bCulled 값을 FALSE로 한다.
	void AllInFrustum();

	// 이웃노드를 만든다(삼각형 찢어짐 방지용)
	void BuildNeighborNode(QuadTree* pRoot, TERRAINVERTEX* pHeightMap, int cx);

	// 쿼드트리를 만든다(Build()함수에서 불린다.)
	BOOL BuildQuadTree(TERRAINVERTEX* pHeightMap);

	// 쿼드트리를 검색해서 4개의 코너값과 일치하는 노드를 찾는다.
	QuadTree* FindNode(TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3);

	// 4개 방향(상단, 하단, 좌측, 우측)의 이웃노드 인덱스를 구한다.
	int GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
private: // Tmp Function 나중에 정리할 것!!
	BOOL IsInRect(RECT* rc, POINT pt)
	{
		if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
			(rc->bottom <= pt.y) && (pt.y <= rc->top))
			return TRUE;

		return FALSE;
	}
private:
	QuadTree* m_pChild[4];
	QuadTree* m_pParent;
	QuadTree* m_pNeighbor[4];

	int m_nCenter;
	int m_nCorner[4];

	//절두체 컬링
	BOOL m_bCulled; // 절두체에서 컬링된 노드인가?
	float m_fRadius; // 노드를 감싸는 경계구
};

