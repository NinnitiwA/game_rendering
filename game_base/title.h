#pragma once
#include "scene.h"

// �^�C�g���V�[���p�N���X
class Title : public Scene
{
private:
	class PostProcess* m_PostProcess;
	class Fade* m_Fade{};

	bool m_NextScene = false; // false : workspace, true : game
public:
	void Init() override;
	void Update() override;
	void Draw() override;

	// �^�C�g����ʂ̔w�i�p�Ƃ��ăQ�[���I�u�W�F�N�g�������ݒ�E�z�u
	//void TitleStageInit();
};