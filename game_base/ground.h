#pragma once
#include "gameObject.h"

class Ground : public GameObject
{
private:

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	static void CreateGround(Scene* Scene, int X, int Z, D3DXVECTOR3 Position);
};