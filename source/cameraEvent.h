#pragma once
#include "event.h"

enum CAMERAEVENT
{
	CAMERAEVENT_TUTORIAL_01, // �`���[�g���A���X�e�[�W�̍Ō�̓�
	CAMERAEVENT_TUTORIAL_02,

	CAMERAEVENT_TLOB_01,     // �͂��܂�̑�n�̍ŏ�
	CAMERAEVENT_TLOB_02,     // �͂��܂�̑�n�̍���

	CAMERAEVENT_RESULT,      // ���U���g���̃Y�[���A�b�v
};

class CameraEvent : public Event
{
private:
	static D3DXVECTOR3 m_Diff;      // �C�x���g�I�t�Z�b�g�̕ω���

	D3DXVECTOR3 m_IdealEventOffset; // �C�x���g�I�t�Z�b�g�̒l
	int         m_Frame = 0;        // �J�����C�x���g�̃t���[��

public:
	// �C�x���g�̏�Ԃ�ύX
	void SetEventStateReady(void) override {
		// �Փ˂��Ă���ꍇ���s
		if (!m_Contact)
		{
			if (m_EventState == EVENT_STATE_OFF) m_EventState = EVENT_STATE_READY;
			m_Contact = true;
		}
	}

	void Init() override;
	void Uninit() override;
	void Update() override;

	void SetCameraEvent(CAMERAEVENT eventNumber);
};