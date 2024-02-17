#pragma once
#include "gameObject.h"

// 地面生成時に影響する地面のタイプ
enum GROUND_TYPE
{
	GROUNDING_TYPE, // 接地
	FLOATING_TYPE,  // 浮遊
};

class Ground : public GameObject
{
private:
	static GROUND_TYPE m_GroundType;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	// 地面生成
	static void CreateGround(Scene* Scene, int X, int Z, D3DXVECTOR3 Position);
	// 地面のタイプ設定
	static void SetGroundType(GROUND_TYPE GroundType) { m_GroundType = GroundType; }
};