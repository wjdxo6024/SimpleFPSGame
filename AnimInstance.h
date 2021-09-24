#pragma once
#ifndef _ANIMINSTANCE_H_
#define _ANIMINSTANCE_H_

#include <d3dx9.h>
#include <d3d9.h>
#include "AnimInstance.h"
#include "MultiAnimManager.h"
#include "MultiAnimStructs.h"

class AnimInstance
{
	friend class MultiAnimManager;

protected:

	MultiAnimManager* m_pMAnimManager;
	D3DXMATRIX m_mxWorld;
	LPD3DXANIMATIONCONTROLLER m_pAC;

private:

	virtual HRESULT Setup(LPD3DXANIMATIONCONTROLLER pAC);
	virtual void UpdateFrames(MultiAnimFrame* pFrame, D3DXMATRIX* pmxBase);
	virtual void DrawFrames(MultiAnimFrame* pFrame);
	virtual void DrawMeshFrame(MultiAnimFrame* pFrame);

public:
	AnimInstance(MultiAnimManager* pMultiAnimManager);
	~AnimInstance();

	MultiAnimManager* GetMultiAnimManager();
	void GetAnimController(LPD3DXANIMATIONCONTROLLER* ppAC);

	D3DXMATRIX GetWorldTransform();
	void SetWorldTransform(const D3DXMATRIX* pmxWorld);
	
	virtual void Cleanup();

	virtual HRESULT AdvanceTime(DOUBLE dTimeDelta, ID3DXAnimationCallbackHandler* pCH);
	virtual HRESULT ResetTime();
	virtual HRESULT Draw();

};

#endif

