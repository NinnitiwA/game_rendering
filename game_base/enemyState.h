#pragma once
class Enemy;

// �G�l�~�[�̏�Ԃ�\���񋓑�
enum EnemyStateName
{
	ENEMY_STATE_IDLE,         // �ҋ@
	ENEMY_STATE_WALK,         // �ړ�
	ENEMY_STATE_READY,        // �U������
	ENEMY_STATE_ATTACK,       // �U��
	ENEMY_STATE_DAMAGED,      // �U�����󂯂Ă���
	ENEMY_STATE_STONE,        // �^�b�N���U��

	ENEMY_STATE_NUM,
};

// �G�l�~�[�X�e�[�g�̊��N���X
class EnemyState
{
protected:
	Enemy* m_Enemy{};
	EnemyStateName m_EnemyStateName{};
public:
	EnemyState(Enemy* enemy) : m_Enemy(enemy) {}
	~EnemyState() { delete[] this; }

	virtual void StateAction() = 0;
	virtual EnemyStateName GetStateName() = 0;
};