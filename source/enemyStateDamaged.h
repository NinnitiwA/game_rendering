#pragma once
#include "enemyState.h"

class EnemyStateDamaged : public EnemyState
{
private:
	int m_DamageFrame;  // 攻撃を受けた後の無敵時間フレーム
public:
	EnemyStateDamaged(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_DAMAGED; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};