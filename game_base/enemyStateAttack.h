#pragma once
#include "enemyState.h"

class EnemyStateAttack : public EnemyState
{
private:
	int m_AttackFrame = 0;  // �ړ��A�j���[�V�����t���[��
public:
	EnemyStateAttack(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_ATTACK; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};
