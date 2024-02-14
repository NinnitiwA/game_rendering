#pragma once
#include "gameObject.h"


#define ENEMY_ATTACK_ADDITIONAL_LENGTH 1.2f // �G�l�~�\�U�����̕␳����

#define ENEMY_MAX_ATTACK_FRAME 32       // �U���A�j���[�V�����ő�t���[����
#define ENEMY_MAGIC_MAX_ATTACK_FRAME 54 // ���@�U���A�j���[�V�����ő�t���[����
#define ENEMY_MAX_WALK_FRAME 14         // �ړ��A�j���[�V�����ő�t���[����
#define ENEMY_MAX_DAMAGE_FRAME 20       // ��_���[�W��ԍő�t���[����
#define ENEMY_MAX_STONE_FRAME 90        // �Ή���ԍő�t���[����

#define ENEMY_RADIUS 1.0f         // �G�l�~�[�̔��a


// Enemy�̎��
enum ENEMY_TYPE
{
	ENEMY_NORMAL,
	ENEMY_MAGIC,
	ENEMY_STONE,
};


// ���ׂĂ�Enemy�̊�{�ƂȂ�N���X
class Enemy : public GameObject
{
protected:
	bool        m_isTarget = false; // �v���C���[�̃^�[�Q�b�g�ɂ���Ă��邩�ǂ����̎��ʎq
	float       m_HitPoint = 5.0f;  // �q�b�g�|�C���g

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_AttackVector{};   // �U�������x�N�g��

	ENEMY_TYPE        m_EnemyType{};      // �G�l�~�[�̎��
	class EnemyState* m_EnemyState{};     // �G�l�~�[�X�e�[�g
	class EnemyState* m_NextEnemyState{}; // ���̃G�l�~�[�X�e�[�g

	float m_AttackRange{}; // �U���͈�
	float m_WalkRange{};   // ���F�͈�
private:

public:
	Enemy()
	{
		SetTag(ENEMY);
		m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
	}


	void SetVelocity(D3DXVECTOR3 vec) { m_Velocity = vec; }
	void SetAttackVector(D3DXVECTOR3 attackVector) { m_AttackVector = attackVector; }
	D3DXVECTOR3 GetAttackVector() { return m_AttackVector; }
	EnemyState* GetEnemyState() { return m_EnemyState; }
	void SetNextEnemyState(EnemyState* state) { m_NextEnemyState = state; }
	void SetEnemyState();
	ENEMY_TYPE GetEnemyType() { return m_EnemyType; }
	// �v���C���[�̃^�[�Q�b�g�ɂȂ�
	void SetEnemyIsTarget(bool isTarget) { m_isTarget = isTarget; }
	// �U�����̓����蔻��
	void AttackCollisionDetection();
	// �^�[�Q�b�g�̕��֏�������]����
	void RotateToTarget(D3DXVECTOR3* targetRot);
	// �U���͈͎擾
	float GetAttackRange() { return m_AttackRange; }
	// ���F�͈͎擾
	float GetWalkRange() { return m_WalkRange; }

	// �_���[�W����
	virtual void SetDamage(float);
	// ���S���菈��
	virtual void JudgeDeath();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};

enum SNAKE_STATE
{
	IDLE_STATE,
	WALK_STATE,
	READY_STATE,
	ATTACK_STATE,
	DAMAGED_STATE,
};

struct SNAKEPARAMETER
{
	SNAKE_STATE State;

	int DamageFrame;
	int AttackFrame;
	int WalkFrame;
	int ReadyFrame;
};
