#pragma once
#include "enemyState.h"

class EnemyStateDamaged : public EnemyState
{
private:
	int m_DamageFrame;  // �U�����󂯂���̖��G���ԃt���[��
public:
	EnemyStateDamaged(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_DAMAGED; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};