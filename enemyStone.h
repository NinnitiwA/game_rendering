#pragma once
#include "enemy.h"


class EnemyStone : public Enemy
{
protected:

private:

public:
	// ƒ_ƒ[ƒWˆ—
	void SetDamage(float) override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
