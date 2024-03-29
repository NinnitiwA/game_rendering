#pragma once
#include "enemy.h"


class EnemyStone : public Enemy
{
protected:

private:

public:
	// ダメージ処理
	void SetDamage(float) override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
