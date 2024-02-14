#pragma once
#include "gameObject.h"

class Camera;
#define POINTLIGHT_NUM 200

class PointLight : public GameObject
{

private:
	ID3D11Buffer* m_VertexBuffer = NULL;

	D3DXVECTOR3 m_MoveVector[POINTLIGHT_NUM];
	bool    m_ModelFlag;
	bool    m_UseFlag;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawLighting();
};