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
	// イベントの状態を変更
	void SetEventStateReady(void) override {
		// 衝突している場合実行
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