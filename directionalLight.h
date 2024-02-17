#pragma once
#include "gameObject.h"

class Camera;

class DirectionalLight : public GameObject
{

private:
	ID3D11Buffer* m_VertexBuffer = NULL;


public:
	void Init();
	void Uninit();
	void Update();
	void DrawLighting();
};