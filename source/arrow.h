#pragma once
#include "gameObject.h"

// 進む方向を示すための矢印看板クラス
class Arrow : public GameObject
{
private:
	float m_HitPoint; // 耐久値

public:
	void SetDamage(float damage) { m_HitPoint -= damage; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};