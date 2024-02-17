#pragma once
#include "gameObject.h"

// �i�ޕ������������߂̖��ŔN���X
class Arrow : public GameObject
{
private:
	float m_HitPoint; // �ϋv�l

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