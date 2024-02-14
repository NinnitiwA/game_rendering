#include "main.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "enemyStateIdle.h"
#include "enemyStateWalk.h"

void EnemyStateIdle::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // エネミーからプレイヤーに向かうベクトル
	float length = D3DXVec3Length(&vecEnemyToPlayer); // プレイヤーとの距離

	// プレイヤーとの距離が一定以内の場合実行
	if (length < m_Enemy->GetWalkRange())
	{
		// プレイヤーの方向を向く
		m_Enemy->RotateToTarget(&vecEnemyToPlayer);

		// 範囲内にプレイヤーが入っている場合
		if (length < 15.0f)
		{
			m_Enemy->SetNextEnemyState(new EnemyStateWalk(m_Enemy));
		}
	}
}

EnemyStateName EnemyStateIdle::GetStateName()
{
	return m_EnemyStateName;
}
