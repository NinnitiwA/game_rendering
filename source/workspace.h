#pragma once
#include "scene.h"

// 技術検証シーン用クラス
class WorkSpace : public Scene
{
private:
	class Audio* m_BGM{};
	class Fade* m_Fade{};

public:
	void Init() override;
	void Update() override;
	void Draw() override;
};