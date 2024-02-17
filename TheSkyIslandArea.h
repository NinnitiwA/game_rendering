#pragma once
#include "area.h"

class TheSkyIsland : public Area
{
private:
	static D3DXVECTOR3 m_AreaPosition;

public:
	static void InitEnvironment();
	static void InitEnemy();
};
