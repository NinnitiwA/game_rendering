#pragma once
#include "gameObject.h"


#define PLAYER_RADIUS 1.0f        // �v���C���[�̔��a
#define PLAYER_MAX_HP 5.0f        // �v���C���[�̍ő�HP

#define PLAYER_ATTACK_INTERVAL 40           // �v���C���[�̍U���Ԋu
#define PLAYER_MAX_MAGIC_FRAME 86			// ���@�r���ő�t���[����
#define PLAYER_MAGIC_MAX_ATTACK_FRAME 54    // ���@�U���A�j���[�V�����ő�t���[����
#define PLAYER_MAX_PRESS_ATTACK_FRAME 14    // �v���X�U���A�j���[�V�����ő�t���[����
#define PLAYER_MAX_DASH_ATTACK_FRAME 40		// �_�b�V���U���A�j���[�V�����ő�t���[����
#define PLAYER_MAX_DASH_ATTACK_PRE_FRAME 32	// �_�b�V���U�������A�j���[�V�����ő�t���[����
#define PLAYER_MAX_JUMP_FRAME 13			// �W�����v�A�j���[�V�����ő�t���[����
#define PLAYER_MAX_WALK_FRAME 14			// �ړ��A�j���[�V�����ő�t���[����
#define PLAYER_MAX_DODGE_FRAME 8			// ����A�j���[�V�����ő�t���[����
#define PLAYER_MAX_DAMAGE_FRAME 12			// ��_���[�W��ԍő�t���[����

enum PlayerState
{
	PLAYER_STATE_GROUND, // �ڒn��
	PLAYER_STARE_JUMP,   // �W�����v��
	PLAYER_STATE_WALK,   // �ړ���
	PLAYER_STATE_DODGE,  // ���

	PLAYER_STATE_PRESSATTACK,  // �v���X�U����
	PLAYER_STATE_DASHATTACK,   // �_�b�V���U����

	PLAYER_STATE_NUM,
};

// �v���C���[�U�����
enum PlayerAttack
{
	// �t�B�[���h
	PLAYER_PRESS_ATTACK,
	PLAYER_DASH_ATTACK,

	// �퓬
	PLAYER_MAGIC_CHANT,
	PLAYER_MAGIC_ATTACK,


	PLAYER_NON_ATTACK,
};

// �v���C���[�p�����[�^�\����
struct PlayerParameter
{
	float HitPoint;     // �q�b�g�|�C���g

	int  AttackFrame;   // �U���Ԋu�p�t���[��
	bool canAttack;     // �U���\���ʎq

	float dashLevel;    // �U�����̃_�b�V���̃��x��

	bool  isFindEnemy;  // �^�[�Q�b�g�ߑ��p���ʎq

	bool  isDamaged;    // �U�����󂯂Ă��邩�ǂ���
	int   DamageFrame;  // �U�����󂯂���̖��G����
};

// �v���C���[�A�j���[�V�����\����
struct PlayerAnim
{
	bool isAttack;      // �U�������ǂ���
	int  AttackFrame;   // �U���t���[��
	float AttackRot;    // �U������
	PlayerAttack PlayerAttack; // �U���̎��

	bool isGround;      // �ڒn���Ă��邩�ǂ���
	bool isJump;        // �W�����v�����ǂ���
	int  JumpFrame;     // �W�����v���̃A�j���[�V�����t���[��

	bool isWalk;        // �ړ������ǂ���  
	int  AnimFrame;     // �ړ����̃A�j���[�V�����t���[��

	bool isDodge;       // ��𒆂��ǂ���
	int  DodgeFrame;    // ��𒆂̃A�j���[�V�����t���[��
};



class Player : public GameObject 
{
private:
	D3DXVECTOR3 m_Velocity{};         // �ړ��x�N�g��
	D3DXVECTOR3 m_AttackedVector{};   // �U�����󂯂������x�N�g��
	D3DXVECTOR3 m_DodgeVector{};      // �������̃x�N�g��;
									  
	PlayerParameter m_Param{};        // �v���C���[�p�����[�^
	PlayerAnim      m_PlayerAnim{};   // �v���C���[�A�j���[�V����

	class Enemy*    m_Target{};       // ���b�N�I�����̃^�[�Q�b�g�G�l�~�[
	D3DXVECTOR3     m_TargetVector{}; // �^�[�Q�b�g�����̃x�N�g��
	float           m_TargetRot{};    // �v���C���[�ƃ^�[�Q�b�g�̊p�x

	static D3DXVECTOR3 m_RespwanPosition; // ���X�|�[�����W
	static bool        m_CanControl;      // �v���C���[����t���O

public:
	// ���X�|�[�����W�ݒ�
	void SetRespawnPosition(D3DXVECTOR3 position) { m_RespwanPosition = position; }

	// �U�����󂯂������x�N�g�����Z�b�g
	void SetAttackedVector(D3DXVECTOR3 vec)
	{
		D3DXVec3Normalize(&vec, &vec); // ���K��
		m_AttackedVector += vec;
		D3DXVec3Normalize(&m_AttackedVector, &m_AttackedVector); // ���Z�����x�N�g���𐳋K��
	}

	// ��𒆎��ʎq���擾
	bool GetPlayerIsDodge() { return m_PlayerAnim.isDodge; }
	// �v���C���[�p�����[�^���擾
	PlayerParameter GetPlayerParameter() { return m_Param; }
	// ����\�p���ʎq�̐ݒ�
	void SetCanControl(bool use) { m_CanControl = use; }
	// ����\�p���ʎq�̎擾
	bool GetCanControl() { return m_CanControl; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	// �^�[�Q�b�g�X�V����
	void UpdateTargetEnemy();
	// �U�����̓����蔻��
	void AttackCollisionDetection();
	// �_���[�W����
	void SetDamage(float damage);
	// �v���C���[�̉񕜗ʏ���
	void SetHealHP(float heal);
	// ���X�|�[�������ݒ�
	void SetRespawn();
	// ���X�|�[������
	void Respawn();
};



