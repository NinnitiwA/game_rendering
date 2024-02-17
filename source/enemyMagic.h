#pragma once
#include "enemy.h"


class EnemyMagic : public Enemy
{
protected:

private:

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
