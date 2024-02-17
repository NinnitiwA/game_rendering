#pragma once
#include "gameObject.h"

class Polygon2D : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawDirectionalLighting();
};