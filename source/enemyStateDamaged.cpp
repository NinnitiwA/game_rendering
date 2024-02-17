#include "main.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "enemyStateIdle.h"
#include "enemyStateDamaged.h"


void EnemyStateDamaged::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 enemyPosition = m_Enemy->GetPosition();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - enemyPosition; // エネミーからプレイヤーに向かうベクトル


	// ひるみ
	D3DXVECTOR3 vector = vecEnemyToPlayer;
	vector.y = 0.0f; // Y軸の移動量は考慮しない
	D3DXVec3Normalize(&vector, &vector);
	m_Enemy->SetVelocity(-vector * 0.25f + D3DXVECTOR3(0.0f, 0.7f * cosf(D3DX_PI * ((float)m_DamageFrame * 2 / (float)ENEMY_MAX_DAMAGE_FRAME)), 0.0f));
	m_DamageFrame++;

	// 終了処理
	if (m_DamageFrame >= ENEMY_MAX_DAMAGE_FRAME)
	{
		m_Enemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_Enemy->SetNextEnemyState(new EnemyStateIdle(m_Enemy));
	}
}

EnemyStateName EnemyStateDamaged::GetStateName()
{
	return m_EnemyStateName;
}
