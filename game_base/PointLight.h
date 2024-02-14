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
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;
	void DrawLighting();
};