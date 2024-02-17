#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "enemyStateAttack.h"
#include "enemyStateReady.h"
#include "enemyStateWalk.h"
#include "random.h"
#include "dataStorage.h"

void EnemyStateAttack::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 enemyPosition = m_Enemy->GetPosition();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - enemyPosition; // エネミーからプレイヤーに向かうベクトル
	float length = D3DXVec3Length(&vecEnemyToPlayer); // プレイヤーとの距離


	if (m_Enemy->GetEnemyType() == ENEMY_MAGIC)
	{
		// 攻撃アニメーション
		m_Enemy->SetPosition(enemyPosition + m_Enemy->GetAttackVector() * DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos[m_AttackFrame].z);                                                  // 攻撃判定前
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale[m_AttackFrame]);
		m_AttackFrame++;

		// 終了処理
		if (m_AttackFrame >= ENEMY_MAGIC_MAX_ATTACK_FRAME)
		{
			m_Enemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}

		// 攻撃判定前に攻撃方向ベクトルを得る
		if (m_AttackFrame == 16 || m_AttackFrame == 27 || m_AttackFrame == 38)
		{
			m_Enemy->RotateToTarget(&vecEnemyToPlayer);
			D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);
			m_Enemy->SetAttackVector(vecEnemyToPlayer);
		}
		// 魔法をプレイヤーに向けて飛ばす
		// プレイヤーとの衝突判定はbulletが行う
		if (m_AttackFrame == 25 || m_AttackFrame == 36 || m_AttackFrame == 47) // 特定の３フレームで当たり判定を行う
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Enemy->GetPosition() + m_Enemy->GetAttackVector() * 2.6f);
			bullet->SetVelocity(m_Enemy->GetAttackVector() * 0.2f);
			Resource::GetEnemyAttackSEAudio()->Play();
		}
	}
	else
	{
		// 攻撃アニメーション
		if (m_AttackFrame >= 20)
			m_Enemy->SetPosition(enemyPosition - DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos[m_AttackFrame].z * m_Enemy->GetAttackVector()); // 攻撃判定後プレイヤーへ向かって移動する
		else
			m_Enemy->SetPosition(enemyPosition + DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos[m_AttackFrame]);                       // 攻撃判定前
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->AttackAnimScale[m_AttackFrame]);
		m_AttackFrame++;

		// 攻撃判定前に攻撃方向ベクトルを得る
		if (m_AttackFrame == 6)
		{
			D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);
			m_Enemy->SetAttackVector(vecEnemyToPlayer);
		}
		// プレイヤーとの衝突判定
		if (m_AttackFrame == 21) // 特定の１フレームで当たり判定を行う
		{
			m_Enemy->AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// 終了処理
		if (m_AttackFrame >= ENEMY_MAX_ATTACK_FRAME)
		{
			m_Enemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// 低確率で再度攻撃
			if (Random(0, 10) < 2) m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 60));
			else m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}
	}

}

EnemyStateName EnemyStateAttack::GetStateName()
{
	return m_EnemyStateName;
}
