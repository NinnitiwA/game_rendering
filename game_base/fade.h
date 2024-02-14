#pragma once
#include "UIObject.h"

class Fade : public UIObject
{
private:
	float m_Alpha = 1.0f;
	bool  m_Fade = false;          // true : in, false : out
	bool  m_FadeFinished = false;  // フェイドが終わったかどうか

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetFadeIn() { m_Fade = true;}
	void SetFadeOut() { m_Fade = false;}
	bool GetFadeFinished() { return m_FadeFinished; }
};