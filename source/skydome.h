#pragma once
#include "gameObject.h"

// �X�J�C�h�[���p�N���X
class Sky : public GameObject
{
private:

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};