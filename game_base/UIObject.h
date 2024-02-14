#pragma once
#include "gameObject.h"

class UIObject : public GameObject
{
protected:
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

private:
	ID3D11Buffer* m_VertexBuffer{};

public:
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void SetVertexBuffer();

	void Init(float x, float y, float Width, float Height);
	void Uninit() override;
};