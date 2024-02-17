#pragma once
#include "gameObject.h"

class Field : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawDebug();
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
};