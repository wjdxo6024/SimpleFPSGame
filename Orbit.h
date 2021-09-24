#pragma once
#include <iostream>
#include <d3dx9math.h>
#include <fbxsdk.h>

class Orbit
{
public:
	Orbit();
	~Orbit();
	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual D3DXVECTOR3 GetLocation() = 0;

protected:
	float x;
	float y;
	float z;
	float r;

	float speed;
};

