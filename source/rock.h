#pragma once
#include "gameObject.h"

// ��I�u�W�F�N�g�p�N���X
class Rock : public GameObject
{
private:
	float m_HitPoint; // �ϋv�l

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