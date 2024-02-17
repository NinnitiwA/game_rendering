#pragma once
#include "gameObject.h"

class Rock : public GameObject
{
private:
	float m_HitPoint; // �ϋv�l

	//class Audio* m_BreakSE;

public:
	void SetDamage(float damage){ m_HitPoint -= damage; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};