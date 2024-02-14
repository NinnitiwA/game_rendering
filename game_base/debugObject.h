#pragma once
#include "gameObject.h"

class DebugObject : public GameObject
{
private:
	int m_DebugNumber; // �ŗL�̔ԍ�
	static int m_Num;  // �f�o�b�O�I�u�W�F�N�g�̑���

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawDebug();

	// �f�o�b�O�I�u�W�F�N�g���̃p�����[�^��ݒ�
	void CalcDebugParameter(int num);
};