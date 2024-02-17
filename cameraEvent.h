#pragma once
#include "event.h"

enum CAMERAEVENT
{
	CAMERAEVENT_TUTORIAL_01, // チュートリアルステージの最後の道
	CAMERAEVENT_TUTORIAL_02,

	CAMERAEVENT_TLOB_01,     // はじまりの大地の最初
	CAMERAEVENT_TLOB_02,     // はじまりの大地の高台

	CAMERAEVENT_RESULT,      // リザルト時のズームアップ
};

class CameraEvent : public Event
{
private:
	static D3DXVECTOR3 m_Diff;      // イベントオフセットの変化量

	D3DXVECTOR3 m_IdealEventOffset; // イベントオフセットの値
	int         m_Frame = 0;        // カメライベントのフレーム

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

	void SetCameraEvent(CAMERAEVENT eventNumber);
};