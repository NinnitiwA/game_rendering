#pragma once
#include "event.h"

// �e�L�X�g�C�x���g�̎�ޗ񋓑�
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

	TEXTEVENT     m_TextEvent{}; // �e�L�X�g�C�x���g���
	float m_Alpha = 0.8f; // �C�x���g�p�摜�̃A���t�@�l
public:
	// �C�x���g�̏�Ԃ�ύX
	void SetEventStateReady(void) override {
		// ���߂ďՓ˂����ꍇ�̂ݎ��s
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