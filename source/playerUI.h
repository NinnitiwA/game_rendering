#pragma once
#include "UIObject.h"

struct PLAYERSTATUS
{
	D3DXVECTOR4 HP;     // x : �ő�HP, y : ���݂�HP, z : ��������HP, w : 
	D3DXVECTOR4 Status; // x : �ő�X�^�~�i, y : ���݂̃X�^�~�i, z : �������̃X�^�~�i, w : ���݂̃X�^�~�i�̂�����
};

// �v���C���[�X�e�[�^�X�p��UI�N���X
class PlayerUI : public UIObject
{
private:
	ID3D11Buffer* m_PlayerUIVertexBuffer{};

	PLAYERSTATUS  m_PlayerStatus;
	static int    m_Count; // �v���C���[�X�e�[�^�X�̕ϓ����Ȃ����ԃJ�E���g
	static float  m_Alpha; // �}�e���A���̃��l
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR4 GetPlayerStatus() { return m_PlayerStatus.Status; }
	void SetPlayerStatus(D3DXVECTOR4 ps) { m_PlayerStatus.Status += ps; }

	static void ResetStatusUICount(void)
	{
		m_Count = 0;
		m_Alpha = 1.0f;
	}
};