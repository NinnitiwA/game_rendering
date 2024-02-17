#pragma once
#include "UIObject.h"

class Fade : public UIObject
{
private:
	float m_Alpha = 1.0f;
	bool  m_Fade = false;          // true : in, false : out
	bool  m_FadeFinished = false;  // フェイドが終わったかどうか
	static bool  m_FadeTexture;    // true : pn, false : kenshou

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetFadeIn() { m_Fade = true;}
	void SetFadeOut() { m_Fade = false;}
	bool GetFadeFinished() { return m_FadeFinished; }
	static void SetFadeTexture(bool flag) { m_FadeTexture = flag; }
};