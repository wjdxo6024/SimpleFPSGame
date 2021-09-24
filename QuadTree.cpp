#include "stdafx.h"
#include "QuadTree.h"

QuadTree::QuadTree()
{

}


QuadTree::~QuadTree()
{
	Destroy();
}

QuadTree::QuadTree(int cx, int cy)
{
	int i;
	m_nCenter = 0;
	m_pParent = NULL;
	for (i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
	}
	m_nCorner[CORNER_TL] = 0;
	m_nCorner[CORNER_TR] = cx - 1;
	m_nCorner[CORNER_BL] = cx * (cy - 1);
	m_nCorner[CORNER_BR] = cx * cy - 1;
	m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	m_fRadius = 0.0f;
	m_bCulled = FALSE;
}

QuadTree::QuadTree(QuadTree* pParent)
{
	int i;
	m_nCenter = 0;
	m_pParent = pParent;
	for (i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
		m_nCorner[i] = 0;
	}

	m_fRadius = 0.0f;
	m_bCulled = FALSE;
}

BOOL QuadTree::Build(TERRAINVERTEX* pHeightMap)
{
	BuildQuadTree(pHeightMap);
	BuildNeighborNode(this, pHeightMap, m_nCorner[CORNER_TR] + 1);
	/*if (SubDivide())
	{
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TL])) - *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BR]));
		
		m_fRadius = D3DXVec3Length(&v) / 2.0f;
		m_pChild[CORNER_TL]->Build(pHeightMap);
		m_pChild[CORNER_TR]->Build(pHeightMap);
		m_pChild[CORNER_BL]->Build(pHeightMap);
		m_pChild[CORNER_BR]->Build(pHeightMap);
	}*/
	return TRUE;
}

int QuadTree::GenTriIndex(int nTris, LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	if (m_bCulled)
	{
		/*if(m_pParent)
			if (m_pParent->IsCulled())*/
			{
				m_bCulled = FALSE;
				return nTris;
			}
	}

	LPWORD p = ((LPWORD)pIndex) + nTris * 3;

	if (IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio))
	{

		if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;

			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];

			nTris++;
			return nTris;
		}

		BOOL b[4] = { 0, 0, 0, 0 };
		if (m_pNeighbor[EDGE_UP])
			b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_DN])
			b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_LT])
			b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_RT])
			b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);



		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;

			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}

		int n;

		if (!b[EDGE_UP])
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n;
			nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR];
			nTris++;
		}
		else
		{
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_TL];
			*p++ = m_nCorner[CORNER_TR];
			nTris++;
		}

		if (!b[EDGE_DN])
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n;
			nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL];
			nTris++;
		}
		else
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL];
			nTris++;
		}

		if (!b[EDGE_LT])
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n;
			nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL];
			nTris++;
		}
		else
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL];
			nTris++;
		}

		if (!b[EDGE_RT])
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n;
			nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR];
			nTris++;
		}
		else
		{
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR];
			nTris++;
		}

		return nTris;
	}

	

	if (m_pChild[CORNER_TL]) nTris = m_pChild[CORNER_TL]->GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_TR]) nTris = m_pChild[CORNER_TR]->GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BL]) nTris = m_pChild[CORNER_BL]->GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BR]) nTris = m_pChild[CORNER_BR]->GenTriIndex(nTris, pIndex, pHeightMap, pFrustum, fLODRatio);

	return nTris;
}

int QuadTree::GenerateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	FrustumCull(pHeightMap, pFrustum);
	return GenTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
}

QuadTree* QuadTree::AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	QuadTree* pChild;

	pChild = new QuadTree(this);
	pChild->SetCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);

	return pChild;
}

BOOL QuadTree::SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;
	m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	return TRUE;
}

BOOL QuadTree::SubDivide()
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
	// 하단변 가운데
	nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
	// 좌측변 가운데
	nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
	// 우측변 가운데
	nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
	// 한 가운데
	nCentralPoint = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1)
	{
		return FALSE;
	}

	m_pChild[CORNER_TL] = AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[CORNER_TR] = AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
	m_pChild[CORNER_BL] = AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
	m_pChild[CORNER_BR] = AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

	return TRUE;
}

void QuadTree::Destroy()
{
	for (int i = 0; i < 4; i++)
	{
		if(m_pChild[i] != NULL)
			delete m_pChild[i];
	}
}

int QuadTree::IsInFrustum(TERRAINVERTEX* pHeightMap, Frustum* pFrustum)
{
	BOOL b[4];
	BOOL bInSphere;

	bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);
	if (!bInSphere) return FRUSTUM_OUT;

	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

	if ((b[0] + b[1] + b[2] + b[3]) == 4) return FRUSTUM_COMPLETELY_IN;

	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::FrustumCull(TERRAINVERTEX* pHeightMap, Frustum* pFrustum)
{
	int ret;

	m_bCulled = FALSE;
	ret = IsInFrustum(pHeightMap, pFrustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:
		AllInFrustum();
		return;
	case FRUSTUM_PARTIALLY_IN:
		m_bCulled = FALSE;
		break;
	case FRUSTUM_OUT:
		m_bCulled = TRUE;
		return;
	}

	if (m_pChild[0]) m_pChild[0]->FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[1]) m_pChild[1]->FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[2]) m_pChild[2]->FrustumCull(pHeightMap, pFrustum);
	if (m_pChild[3]) m_pChild[3]->FrustumCull(pHeightMap, pFrustum);
}

BOOL QuadTree::IsVisible(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
{
	return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= GetLODLevel(pHeightMap, pCamera, fLODRatio));
}

float QuadTree::GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
{
	return D3DXVec3Length(&(*pv2 - *pv1));
}

int QuadTree::GetLODLevel(TERRAINVERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
{
	float d = GetDistance((D3DXVECTOR3*)(pHeightMap + m_nCenter), pCamera);
	return max((int)(d * fLODRatio), 1);
}

// 모든 m_bCulled 값을 FALSE로 한다.
void QuadTree::AllInFrustum()
{
	m_bCulled = FALSE;
	if (!m_pChild[0]) return;
	m_pChild[0]->AllInFrustum();
	m_pChild[1]->AllInFrustum();
	m_pChild[2]->AllInFrustum();
	m_pChild[3]->AllInFrustum();
}

// 이웃노드를 만든다(삼각형 찢어짐 방지용)
void QuadTree::BuildNeighborNode(QuadTree* pRoot, TERRAINVERTEX* pHeightMap, int cx)
{
	int n;
	int _0, _1, _2, _3;

	for (int i = 0; i < 4; i++)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
		// 이웃 노드의 4개의 코너값을 얻는다.
		n = GetNodeIndex(i, cx, _0, _1, _2, _3);
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if (n >= 0) m_pNeighbor[i] = pRoot->FindNode(pHeightMap, _0, _1, _2, _3);
	}

	if (m_pChild[0])
	{
		m_pChild[0]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[1]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[2]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[3]->BuildNeighborNode(pRoot, pHeightMap, cx);
	}
}

// 쿼드트리를 만든다(Build()함수에서 불린다.)
BOOL QuadTree::BuildQuadTree(TERRAINVERTEX* pHeightMap)
{
	if (SubDivide())
	{
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TL])) -
						*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BR]));

		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로,
		// 2로 나누어 반지름을 구한다.
		m_fRadius = D3DXVec3Length(&v) / 2.0f;
		m_pChild[CORNER_TL]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_TR]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BL]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BR]->BuildQuadTree(pHeightMap);
	}
	return TRUE;
}

// 쿼드트리를 검색해서 4개의 코너값과 일치하는 노드를 찾는다.
QuadTree* QuadTree::FindNode(TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3)
{
	QuadTree* p = NULL;
	if ((m_nCorner[0] == 0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3))
		return this;

	if (m_pChild[0])
	{
		RECT rc;
		POINT pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		pt.x = (int)pHeightMap[n].p.x;
		pt.y = (int)pHeightMap[n].p.z;

		SetRect(&rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.z);
		// pt값이 점유범위 안에 있다면 자식노드로 들어간다.

		if (IsInRect(&rc, pt))
			return m_pChild[0]->FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.z);
		// pt값이 점유범위 안에 있다면 자식노드로 들어간다.

		if (IsInRect(&rc, pt))
			return m_pChild[1]->FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.z);
		// pt값이 점유범위 안에 있다면 자식노드로 들어간다.

		if (IsInRect(&rc, pt))
			return m_pChild[2]->FindNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.z);
		// pt값이 점유범위 안에 있다면 자식노드로 들어간다.

		if (IsInRect(&rc, pt))
			return m_pChild[3]->FindNode(pHeightMap, _0, _1, _2, _3);

	}

	return NULL;
} 

// 4개 방향(상단, 하단, 좌측, 우측)의 이웃노드 인덱스를 구한다.
int QuadTree::GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3)
{
	int n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a; // 현재 노드의 좌우폭값

	switch (ed)
	{
	case EDGE_UP: // 위쪽 방향 이웃노드의 인덱스
		_0 = _a - cx * gap;
		_1 = _b - cx * gap;
		_2 = _a;
		_3 = _b;
		break;
	case EDGE_DN:
		_0 = _c;
		_1 = _d;
		_2 = _c + cx * gap;
		_3 = _d + cx * gap;
		break;
	case EDGE_LT:
		_0 = _a - gap;
		_1 = _a;
		_2 = _c - gap;
		_3 = _c;
		break;
	case EDGE_RT:
		_0 = _b;
		_1 = _b + gap;
		_2 = _d;
		_3 = _d + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return n;
}

