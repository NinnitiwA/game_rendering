#pragma once
#include "gameObject.h"

// �C�x���g�̏�ԗ񋓑�
enum EVENT_STATE
{
	EVENT_STATE_OFF,   // �C�x���g�x�~��
	EVENT_STATE_READY, // �C�x���g�N������
	EVENT_STATE_ON,    // �C�x���g�N����
};

// ���ׂẴC�x���g�̊�{�ƂȂ�N���X
class Event : public GameObject
{
protected:
	EVENT_STATE m_EventState = EVENT_STATE_OFF; // �C�x���g�̏��
	bool m_Contact = false;                     // �C�x���g�̃g���K�[�ƂȂ�I�u�W�F�N�g�̏Փˎ��ʎq
public:
	// �C�x���g�̏�Ԃ�ύX
	virtual void SetEventStateReady(void) {};
	// �Փ˔����ݒ�
	void SetContact(bool isContact) { m_Contact = isContact; }

	void SetEventStateOff(void) { m_EventState = EVENT_STATE_OFF; }
	void SetEventStateOn(void) { m_EventState = EVENT_STATE_ON; }
	EVENT_STATE GetEventState(void) { return m_EventState; }
};