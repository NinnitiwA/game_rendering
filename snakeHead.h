#pragma once
#include "enemy.h"

#define SNAKE_MAX_ATTACK_FRAME 34   // �U�����ő�t���[����
#define SNAKE_MAX_DAMAGE_FRAME 20   // ��_���[�W��ԃt���[����
#define SNAKE_RADIUS 1.0f           // �X�l�[�N�I�u�W�F�N�g�̔��a
#define SNAKE_BODY_NUM 6            // ���̂̐��i�Ō���͋�̃I�u�W�F�N�g�����邽�ߎ��̂�6�́j
#define SNAKE_BODY_LAST_ALLAY_NUM 5 // ���̔z��̍Ō���ԍ��i6�́j

class SnakeHead : public Enemy
{
private:
	class SnakeBody* m_SnakeBodyList[SNAKE_BODY_NUM + 1]; // ���̃I�u�W�F�N�g�̔z��
	SNAKEPARAMETER m_Param;

	float        m_MoveSpeed; // �ړ����x

public:
	// ���̔z����擾
	SnakeBody* GetSnakeBody(int num) { return m_SnakeBodyList[num]; }
	// �ړ����x���擾
	float GetMoveSpeed(void) { return m_MoveSpeed; }

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadowMapping();
	void DrawZPrePass();

	// ���̃I�u�W�F�N�g����
	void CreateSnakeBody(void);
	// �󋵂������G�l�~�[�̍s������֐�
	void EnemyActionDecider();
	// �_���[�W����
	void SetDamage(float);
	// ���S���菈��
	void JudgeDeath();
	// �������̍Ō���I�u�W�F�N�g�̔z��ԍ����擾
	int GetAliveSnakeBodyLastArrayNumber();
	// �X�l�[�N�̃X�e�[�g��ݒ�
	void SetSnakeState(SNAKE_STATE SnakeState) { m_Param.State = SnakeState; }
};

