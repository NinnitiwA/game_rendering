#include "main.h"
#include "manager.h"
#include "scene.h"
#include "enemy.h"
#include "player.h"
#include "enemyStateReady.h"
#include "enemyStateWalk.h"
#include "enemyStateIdle.h"
#include "dataStorage.h"

void EnemyStateWalk::StateAction()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Enemy->GetPosition(); // �G�l�~�[����v���C���[�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecEnemyToPlayer); // �v���C���[�Ƃ̋���


	// �v���C���[�̕���������
	m_Enemy->RotateToTarget(&vecEnemyToPlayer);

	// �ړ���
	{
		D3DXVECTOR3 vector = vecEnemyToPlayer;
		vector.y = 0.0f; // Y���̈ړ��ʂ͍l�����Ȃ�
		D3DXVec3Normalize(&vector, &vector);
		m_Enemy->SetVelocity(0.1 * vector);

		// �ړ��A�j���[�V����
		m_Enemy->SetPosition(m_Enemy->GetPosition() + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_AnimFrame]);
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_AnimFrame]);
		m_AnimFrame++;
		// �A�j���[�V�����t���[���J�E���g���Z�b�g
		if (m_AnimFrame >= ENEMY_MAX_WALK_FRAME) m_AnimFrame = 0;
	}

	// �v���C���[�Ƃ̋��������ȓ��̏ꍇ�U��������Ԃ�
	if (length < m_Enemy->GetAttackRange())
	{
		m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, m_AnimFrame));
	}

	// �v���C���[�Ƃ̋���������Ă���Ƃ�
	if (length >= m_Enemy->GetWalkRange())
	{
		m_Enemy->SetNextEnemyState(new EnemyStateIdle(m_Enemy));
	}
}

EnemyStateName EnemyStateWalk::GetStateName()
{
	return m_EnemyStateName;
}
