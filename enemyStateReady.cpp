#include "main.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "enemyStateAttack.h"
#include "enemyStateReady.h"
#include "enemyStateWalk.h"
#include "enemyStateIdle.h"
#include "dataStorage.h"

void EnemyStateReady::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // エネミーからプレイヤーに向かうベクトル
	float length = D3DXVec3Length(&vecEnemyToPlayer); // プレイヤーとの距離


	// 攻撃準備
	{
		m_ReadyFrame++;

		// 攻撃開始
		if (m_ReadyFrame >= 90)
		{
			m_Enemy->SetNextEnemyState(new EnemyStateAttack(m_Enemy));
			m_ReadyFrame = 0;
		}
	}

	// プレイヤーの方向を向く
	m_Enemy->RotateToTarget(&vecEnemyToPlayer);


	// 移動中
	{
		D3DXVECTOR3 vector = vecEnemyToPlayer;
		vector.y = 0.0f; // Y軸の移動量は考慮しない
		D3DXVec3Normalize(&vector, &vector);
		m_Enemy->SetVelocity(0.1 * vector);

		// 移動アニメーション
		m_Enemy->SetPosition(m_Enemy->GetPosition() + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_AnimFrame]);
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_AnimFrame]);
		m_AnimFrame++;
		// アニメーションフレームカウントリセット
		if (m_AnimFrame >= ENEMY_MAX_WALK_FRAME) m_AnimFrame = 0;
	}


	// 待機状態または攻撃準備状態でプレイヤーと距離が離れた時
	if (length >= m_Enemy->GetAttackRange())
	{
		m_Enemy->SetNextEnemyState(new EnemyStateWalk(m_Enemy, m_AnimFrame));
	}

	// プレイヤーとの距離が離れているとき
	if (length >= m_Enemy->GetWalkRange())
	{
		m_Enemy->SetNextEnemyState(new EnemyStateIdle(m_Enemy));
	}
}

EnemyStateName EnemyStateReady::GetStateName()
{
	return m_EnemyStateName;
}
