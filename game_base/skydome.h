#pragma once
#include "gameObject.h"

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