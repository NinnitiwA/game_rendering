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
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // �G�l�~�[����v���C���[�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecEnemyToPlayer); // �v���C���[�Ƃ̋���

	m_StoneFrame++;

	// �Ή���ԉ���
	if (m_StoneFrame >= ENEMY_MAX_STONE_FRAME)
	{
		// �ҋ@��Ԃ܂��͍U��������ԂŃv���C���[�Ƌ��������ꂽ��
		if (length < m_Enemy->GetAttackRange())
		{
			m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}

		// �v���C���[�Ƃ̋���������Ă���Ƃ�
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
