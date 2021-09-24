#pragma once
#include "Frustum.h"
#include "MyDef.h"

class QuadTree
{
	enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR};
	enum {EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT};
	enum QuadLocation {FRUSTUM_OUT = 0, // ����ü�� ������ ���
	FRUSTUM_PARTIALLY_IN =1, // ����ü�� �κ������� ���Ե�
	FRUSTUM_COMPLETELY_IN = 2, // ����ü�� ������ ���Ե�
	FRUSTUM_UNKNOWN = -1}; // �𸣰���

public:
	QuadTree();
	~QuadTree();

	QuadTree(int cx, int cy);

	QuadTree(QuadTree* pParent);

	BOOL Build(TERRAINVERTEX* pHeightMap);

	int GenerateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio);

	int GenTriIndex(int nTris, LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio);

	// �ڳ� �ε��� ���� ���´�.
	void GetCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = m_nCorner[0]; _1 = m_nCorner[1]; _2 = m_nCorner[2]; _3 = m_nCorner[3];
	}

	// ���� ��尡 ����� ������ ����ΰ�?
	BOOL IsVisible(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio);

private:
	QuadTree* AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	BOOL SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	// ����Ʈ���� 4���� ���� Ʈ���� �κк����Ѵ�.
	BOOL SubDivide();

	void Destroy();

	int IsInFrustum(TERRAINVERTEX* pHeightMap, Frustum* pFrustum);

	BOOL IsCulled() { return m_bCulled; }

	void FrustumCull(TERRAINVERTEX* pHeightMap, Frustum* pFrustum);

	float GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2);

	int GetLODLevel(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio);
	
	// ��� m_bCulled ���� FALSE�� �Ѵ�.
	void AllInFrustum();

	// �̿���带 �����(�ﰢ�� ������ ������)
	void BuildNeighborNode(QuadTree* pRoot, TERRAINVERTEX* pHeightMap, int cx);

	// ����Ʈ���� �����(Build()�Լ����� �Ҹ���.)
	BOOL BuildQuadTree(TERRAINVERTEX* pHeightMap);

	// ����Ʈ���� �˻��ؼ� 4���� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
	QuadTree* FindNode(TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3);

	// 4�� ����(���, �ϴ�, ����, ����)�� �̿���� �ε����� ���Ѵ�.
	int GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
private: // Tmp Function ���߿� ������ ��!!
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

	//����ü �ø�
	BOOL m_bCulled; // ����ü���� �ø��� ����ΰ�?
	float m_fRadius; // ��带 ���δ� ��豸
};

