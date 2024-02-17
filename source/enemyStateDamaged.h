#pragma once
#include "enemyState.h"

class EnemyStateDamaged : public EnemyState
{
private:
	int m_DamageFrame;  // UŒ‚‚ğó‚¯‚½Œã‚Ì–³“GŠÔƒtƒŒ[ƒ€
public:
	EnemyStateDamaged(Enemy* enemy) : EnemyState(enemy) { m_EnemyStateName = ENEMY_STATE_DAMAGED; }
	void StateAction() override;
	EnemyStateName GetStateName() override;
};