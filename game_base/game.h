#pragma once
#include "scene.h"

// �Q�[���V�[���p�N���X
class Game : public Scene
{
private:
	class Audio* m_BGM{};

	class Fade*  m_Fade{};
public:
	void Init() override;
	void Update() override;
	void Draw() override;

	// �C�x���g�̏����ݒ�
	void InitEvent();
};