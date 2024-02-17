#pragma once
#include "gameObject.h"

enum UI_TYPE
{
	NORMAL_TYPE, // 常時表示
	BLINK_TYPE,  // 点滅表示
};

class UIObject : public GameObject
{
protected:
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	int m_Frame = 0;  // フレーム数
	int m_Number = 0; // texture番号

private:
	ID3D11Buffer* m_VertexBuffer{};
	UI_TYPE m_Type;

public:
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void SetVertexBuffer();
	void SetUIType(UI_TYPE type, int num) { m_Type = type; m_Number = num; }

	void Init() {};
	void Init(float x, float y, float Width, float Height);
	void Uninit() override;
	void Update()override;
	void Draw() override;
};