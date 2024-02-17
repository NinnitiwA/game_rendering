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
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - enemyPosition; // �G�l�~�[����v���C���[�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecEnemyToPlayer); // �v���C���[�Ƃ̋���


	if (m_Enemy->GetEnemyType() == ENEMY_MAGIC)
	{
		// �U���A�j���[�V����
		m_Enemy->SetPosition(enemyPosition + m_Enemy->GetAttackVector() * DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos[m_AttackFrame].z);                                                  // �U������O
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale[m_AttackFrame]);
		m_AttackFrame++;

		// �I������
		if (m_AttackFrame >= ENEMY_MAGIC_MAX_ATTACK_FRAME)
		{
			m_Enemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}

		// �U������O�ɍU�������x�N�g���𓾂�
		if (m_AttackFrame == 16 || m_AttackFrame == 27 || m_AttackFrame == 38)
		{
			m_Enemy->RotateToTarget(&vecEnemyToPlayer);
			D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);
			m_Enemy->SetAttackVector(vecEnemyToPlayer);
		}
		// ���@���v���C���[�Ɍ����Ĕ�΂�
		// �v���C���[�Ƃ̏Փ˔����bullet���s��
		if (m_AttackFrame == 25 || m_AttackFrame == 36 || m_AttackFrame == 47) // ����̂R�t���[���œ����蔻����s��
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Enemy->GetPosition() + m_Enemy->GetAttackVector() * 2.6f);
			bullet->SetVelocity(m_Enemy->GetAttackVector() * 0.2f);
			Resource::GetEnemyAttackSEAudio()->Play();
		}
	}
	else
	{
		// �U���A�j���[�V����
		if (m_AttackFrame >= 20)
			m_Enemy->SetPosition(enemyPosition - DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos[m_AttackFrame].z * m_Enemy->GetAttackVector()); // �U�������v���C���[�֌������Ĉړ�����
		else
			m_Enemy->SetPosition(enemyPosition + DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos[m_AttackFrame]);                       // �U������O
		m_Enemy->SetScale(DataStorage::GetEnemyFieldDataStorage()->AttackAnimScale[m_AttackFrame]);
		m_AttackFrame++;

		// �U������O�ɍU�������x�N�g���𓾂�
		if (m_AttackFrame == 6)
		{
			D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);
			m_Enemy->SetAttackVector(vecEnemyToPlayer);
		}
		// �v���C���[�Ƃ̏Փ˔���
		if (m_AttackFrame == 21) // ����̂P�t���[���œ����蔻����s��
		{
			m_Enemy->AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// �I������
		if (m_AttackFrame >= ENEMY_MAX_ATTACK_FRAME)
		{
			m_Enemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			// ��m���ōēx�U��
			if (Random(0, 10) < 2) m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 60));
			else m_Enemy->SetNextEnemyState(new EnemyStateReady(m_Enemy, 0));
		}
	}

}

EnemyStateName EnemyStateAttack::GetStateName()
{
	return m_EnemyStateName;
}
