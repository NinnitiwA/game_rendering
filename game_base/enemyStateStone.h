#pragma once
#include "enemyState.h"

class EnemyStateStone : public EnemyState
{
private:
	int m_StoneFrame = 0;  // �U�����󂯂���̖��G���ԃt���[��
public:
	EnemyStateStone(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_STONE; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};