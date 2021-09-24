#pragma once
#ifndef _MULTIANIMMANAGER_H_
#define _MULTIANIMMANAGER_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "MultiAllocateHierarchy.h"
#include "MultiAnimStructs.h"
#include "AnimInstance.h"

class MultiAnimManager
{
	friend class MultiAllocateHierarchy;
	friend class AnimInstance;
	friend struct MultiAnimFrame;
	friend struct MultiAnimMC;

protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;

	LPD3DXEFFECT m_pEffect;
	char* m_sTechnique;
	DWORD m_dwWorkingPaletteSize;
	D3DXMATRIX* m_amxWorkingPalette;

	std::vector <AnimInstance*> m_v_pAnimInstances;

	MultiAnimFrame* m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAC;

	float m_fBoundingRadius;

private:
	HRESULT CreateInstance(AnimInstance** ppAnimInstance);
	HRESULT SetupBonePtrs(MultiAnimFrame* pFrame);

public:
	MultiAnimManager();
	~MultiAnimManager();

	virtual HRESULT Setup(LPDIRECT3DDEVICE9 pDevice, TCHAR  sFile[], TCHAR sFxFile[],
		MultiAllocateHierarchy* pAH, LPD3DXLOADUSERDATA pLUD = NULL);

	virtual HRESULT Cleanup(MultiAllocateHierarchy* pAH);

	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXEFFECT GetEffect();
	DWORD GetNumInstances();
	AnimInstance* GetInstance(DWORD dwldx);
	float GetBoundingRadius();

	virtual HRESULT CreateNewInstance(DWORD* pdwNewIdx);

	virtual void SetTechnique(char* sTechnique);

	virtual HRESULT Draw();
};

#endif // !_MULTIANIMMANAGER_H_