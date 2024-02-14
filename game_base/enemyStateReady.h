#pragma once
#include "enemyState.h"

class EnemyStateReady : public EnemyState
{
private:
	int m_AnimFrame = 0;  // �ړ��A�j���[�V�����t���[��
	int m_ReadyFrame = 0; // �U���Ԋu�v���p�t���[��
public:
	EnemyStateReady(Enemy* enemy, int animFrame) : EnemyState(enemy) {
		m_AnimFrame = animFrame; 
		m_EnemyStateName = ENEMY_STATE_READY;
	}
	void StateAction() override;
	EnemyStateName GetStateName() override;
};
