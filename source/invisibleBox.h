#pragma once
#include "gameObject.h"
#include "event.h"

// �e�����ȕǂ̏��
struct InvisibleBoxParameter
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
	float       Length;
	bool        Collision;
};

// �����I�u�W�F�N�g�N���X
class InvisibleBox : public GameObject
{
private:
	InvisibleBoxParameter m_Param{};

	// �C�x���g�I�u�W�F�N�g
	Event* m_Event;

public:
	// �I�u�W�F�N�g�p�����[�^�擾
	InvisibleBoxParameter GetInvisibleBoxParameter(void) { return m_Param; }
	// �C�x���g�ݒ�
	void SetEvent(Event* event) { m_Event = event; }
	// �R���W�����ݒ�
	void SetCollision(bool flag) { m_Param.Collision = flag; }
	// �C�x���g���s��
	void RunningEvent(void) { if (!m_Event->GetDestroy()) m_Event->SetEventStateReady(); }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;
};