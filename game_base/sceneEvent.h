#pragma once
#include "event.h"

enum SCENEEVENT
{
	SCENEEVENT_TUTORIAL_GAME,
	SCENEEVENT_GAME_RESULT,
};

class SceneEvent : public Event
{
private:
	SCENEEVENT m_SceneEvent;

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

	void SetSceneEvent(SCENEEVENT eventNumber);
};