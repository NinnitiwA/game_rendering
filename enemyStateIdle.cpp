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
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // �G�l�~�[����v���C���[�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecEnemyToPlayer); // �v���C���[�Ƃ̋���

	// �v���C���[�Ƃ̋��������ȓ��̏ꍇ���s
	if (length < m_Enemy->GetWalkRange())
	{
		// �v���C���[�̕���������
		m_Enemy->RotateToTarget(&vecEnemyToPlayer);

		// �͈͓��Ƀv���C���[�������Ă���ꍇ
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
