#include "main.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "enemyStateStone.h"
#include "enemyStateReady.h"
#include "enemyStateIdle.h"
#include "dataStorage.h"

void EnemyStateStone::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // エネミーからプレイヤーに向かうベクトル
	float length = D3DXVec3Length(&vecEnemyToPlayer); // プレイヤーとの距離

	m_StoneFrame++;

	// 石化状態解除
	if (m_StoneFrame >= ENEMY_MAX_STONE_FRAME)
	{
		// 待機状態または攻撃準備状態でプレイヤーと距離が離れた時
		if (length < m_Enemy->GetAttackRange())
		{
			m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}

		// プレイヤーとの距離が離れているとき
		else
		{
			m_Enemy->SetNextEnemyState(new EnemyStateIdle(m_Enemy));
		}
	}
}

EnemyStateName EnemyStateStone::GetStateName()
{
	return m_EnemyStateName;
}
