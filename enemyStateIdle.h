#pragma once
#include "enemyState.h"

class EnemyStateIdle : public EnemyState
{
public:
	EnemyStateIdle(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_IDLE; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};