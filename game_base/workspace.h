#pragma once
#include "scene.h"

// �Z�p���؃V�[���p�N���X
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