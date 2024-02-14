#pragma once
#include "scene.h"

// ゲームシーン用クラス
class Tutorial : public Scene
{
private:
	class Audio* m_BGM{};

	class Fade* m_Fade{};
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};