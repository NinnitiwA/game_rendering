#pragma once
#include "enemy.h"

// ���ׂĂ�Enemy�̊�{�ƂȂ�N���X
class EnemyStone : public Enemy
{
protected:

private:

public:
	// �_���[�W����
	void SetDamage(float) override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
