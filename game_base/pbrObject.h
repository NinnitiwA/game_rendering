#pragma once
#include "gameObject.h"

class PBRObject : public GameObject
{
private:
	float       m_Roughness;
	float       m_Metalic;

public:
	void Init() override {};
	void Init(D3DXVECTOR3 pos, float roughness, float metalic);
	void Uninit() override;
	void Update() override;
	void Draw() override; // PBRåüèÿóp
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
};