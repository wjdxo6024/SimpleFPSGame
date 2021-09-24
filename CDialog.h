#pragma once
#include "CSprite.h"
#include "CText.h"

class CDialog
{
private:

public:
	virtual bool OnInit();
	virtual bool OnRender();
	virtual bool OnUpdate();
	virtual bool OnRelease();
};

