#pragma once
#include "gameObject.h"

// 物理ベースレンダリングオブジェクトデバッグ用オブジェクト
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
	void Draw() override; // PBR検証用
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
};