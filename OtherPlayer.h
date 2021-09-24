#pragma once
#include "Player.h"
class OtherPlayer :
	public Player
{
private:
	LPDIRECT3DTEXTURE9 m_pSprite[56];

	D3DXVECTOR3 m_otherDirView;
	D3DXVECTOR3 m_pos;

	
public:
	OtherPlayer();
	~OtherPlayer();

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* texture);
	void OnRender(Camera* camera, D3DLIGHT9* light);
	void OnUpdate();
	void OnUpdate(D3DXVECTOR3& pos, D3DXVECTOR3& dir);
	void OnRelease();
};

