#pragma once
#include "enemyState.h"

class EnemyStateStone : public EnemyState
{
private:
	int m_StoneFrame = 0;  // 攻撃を受けた後の無敵時間フレーム
public:
	EnemyStateStone(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_STONE; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};