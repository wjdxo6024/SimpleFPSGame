#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#define WIN_WIDTH		800
#define WIN_HEIGHT		600

#define PI 3.14159265f
#define FOV (PI/4.0f)
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)
#define NEAR_PLANE 1
#define FAR_PLANE 10000

static D3DXMATRIXA16 g_matProj;

enum GameState {TESTMODE, MAINMENU, SINGLE, MULTI, OPTIONS, EXIT, GAMELOADING, GAMESTART};
enum ClientState {
	BEFORELOGIN = 1000, INLOGIN = 1001,
	INROOM = 1002, INROOMREADY = 1003,
	INPLAYINIT = 1004, INPLAYREADY = 1005, INPLAYING = 1006
};

struct XYZ
{
	float x;
	float y;
	float z;
};

class ModelVertex
{
public:

	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	ModelVertex()
	{

	}

	ModelVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
		_u = u;
		_v = v;
	}
};

#define D3DFVF_MODELVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct TERRAINVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;
};

#define D3DFVF_TERRAINVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct TERRAININDEX
{
	WORD _0, _1, _2;
};

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
