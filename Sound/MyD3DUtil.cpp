#include "stdafx.h"
#include "MyD3DUtil.h"

LPD3DXMESH LoadModel(const char * filename, LPDIRECT3DDEVICE9 m_pd3dDevice)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromXA(filename, D3DXMESH_SYSTEMMEM, m_pd3dDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugStringA("모델 로딩 실패: ");
		OutputDebugStringA(filename);
		OutputDebugStringA("\n");
	}

	return ret;
}

LPDIRECT3DTEXTURE9 LoadTexture(const char* filename, LPDIRECT3DDEVICE9 m_pd3dDevice)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice, filename, &ret)))
	{
		OutputDebugStringA("텍스쳐 로딩 실패: ");
		OutputDebugStringA(filename);
		OutputDebugStringA("\n");
	}

	return ret;
}


 LPD3DXEFFECT LoadShader(const char * filename, LPDIRECT3DDEVICE9 m_pd3dDevice)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFileA(m_pd3dDevice, filename, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf_s(str, size, "%s", (const char*)ack);
			OutputDebugStringA(str);
			delete[] str;
		}
	}
	return ret;

}

 BOOL CheckSphereIntersect(D3DXVECTOR3 vCenter1, float fRadius1, D3DXVECTOR3 vCenter2,
	 float fRadius2)
 {
	 float fDistance;
	 D3DXVECTOR3 vDiff;
	 vDiff = vCenter2 - vCenter1;
	 fDistance = D3DXVec3Length(&vDiff);
	 if (fDistance <= (fRadius1 + fRadius2))
		 return TRUE;
	 return FALSE;
 }

BOOL CheckCubeIntersection(
	 D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1, D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2)
 {
	 if (vMin1->x <= vMax2->x && vMax1->x >= vMin2->x &&
		 vMin1->y <= vMax2->y && vMax1->y >= vMin2->y &&
		 vMin1->z <= vMax2->z && vMax1->z >= vMin2->z)
		 return TRUE;
	 return FALSE;
 }

//template<typename T>
//T AbsoluteNum(T Num)
//{
//	if (Num < 0)
//		return -Num;
//	return Num;
//}

const float AbsoluteNum(float Num)
{
	if (Num < 0)
		return -Num;
	return Num;
}

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z); 
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

float Linear(float v0, float v1, float t)
{
	return v0 + (1.0f - t) + v1 * t;
}