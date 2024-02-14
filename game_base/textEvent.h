#pragma once
#include "event.h"

// テキストイベントの種類列挙体
enum TEXTEVENT
{
	TEXTEVENT_TUTORIAL_JUMP,
	TEXTEVENT_TUTORIAL_PRESSATTACK,
	TEXTEVENT_TUTORIAL_CHARGEATTACK,
	TEXTEVENT_TUTORIAL_DODGE,

	TEXTEVENT_END,

	TEXTEVENT_DEATH,
};

class TextEvent : public Event
{
private:
	ID3D11Buffer* m_VertexBuffer{};

	TEXTEVENT     m_TextEvent{}; // テキストイベント種類
	float m_Alpha = 0.8f; // イベント用画像のアルファ値
public:
	// イベントの状態を変更
	void SetEventStateReady(void) override {
		// 初めて衝突した場合のみ実行
		if (!m_Contact)
		{
			m_EventState = EVENT_STATE_READY;
			m_Contact = true;
		}
	}

	void Init() override {};
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InitTextEvent(float x, float y, float Width, float Height);
	void SetTextEvent(TEXTEVENT eventNumber);
	static void SwitchTextEvent();
};