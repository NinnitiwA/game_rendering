#pragma once
#include "enemy.h"
#include "snakeHead.h"

class SnakeBody : public Enemy
{
private:
	D3DXVECTOR3 m_PreSnakePosition; // ���O�փI�u�W�F�N�g�̍��W
	int         m_Number;           // ���̂̏���
	SNAKEPARAMETER m_Param;

public:
	void SetPreSnakePosition(D3DXVECTOR3 Position) { m_PreSnakePosition = Position; }
	void SetSnakeBodyNumber(int num) { m_Number = num; }

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadowMapping();
	void DrawZPrePass();

	// �󋵂����ăG�l�~�[�̍s�������肷��֐�
	void EnemyActionDecider();
	// ���ڍU�����ꂽ�ꍇ�̃_���[�W����
	void SetDamage(float);
	// ��������Ă΂ꂽ�ꍇ�̃_���[�W����
	void SetDamageFromHead(float);
	// �X�l�[�N�̃X�e�[�g��ݒ�
	void SetSnakeState(SNAKE_STATE SnakeState) { m_Param.State = SnakeState; }
};

