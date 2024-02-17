#pragma once
#include "gameObject.h"

class Camera;
#define SPOTLIGHT_NUM 1

class SpotLight : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;

	bool    m_ModelFlag;
	bool    m_UseFlag;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawLighting();
	void DrawZPrePass();
};