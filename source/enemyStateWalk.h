#pragma once
#include "enemyState.h"

class EnemyStateWalk : public EnemyState
{
private:
	int m_AnimFrame = 0; // 移動アニメーションフレーム
public:
	EnemyStateWalk(Enemy* enemy, int animFrame) : EnemyState(enemy) {
		m_AnimFrame = animFrame; 
		m_EnemyStateName = ENEMY_STATE_WALK;
	}
	EnemyStateWalk(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_WALK; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};
