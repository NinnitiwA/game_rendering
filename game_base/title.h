#pragma once
#include "scene.h"

// タイトルシーン用クラス
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

	// タイトル画面の背景用としてゲームオブジェクトを初期設定・配置
	//void TitleStageInit();
};