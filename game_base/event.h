#pragma once
#include "gameObject.h"

// イベントの状態列挙体
enum EVENT_STATE
{
	EVENT_STATE_OFF,   // イベント休止中
	EVENT_STATE_READY, // イベント起動準備
	EVENT_STATE_ON,    // イベント起動中
};

// すべてのイベントの基本となるクラス
class Event : public GameObject
{
protected:
	EVENT_STATE m_EventState = EVENT_STATE_OFF; // イベントの状態
	bool m_Contact = false;                     // イベントのトリガーとなるオブジェクトの衝突識別子
public:
	// イベントの状態を変更
	virtual void SetEventStateReady(void) {};
	// 衝突判定を設定
	void SetContact(bool isContact) { m_Contact = isContact; }

	void SetEventStateOff(void) { m_EventState = EVENT_STATE_OFF; }
	void SetEventStateOn(void) { m_EventState = EVENT_STATE_ON; }
	EVENT_STATE GetEventState(void) { return m_EventState; }
};