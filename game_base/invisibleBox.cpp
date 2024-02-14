#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "gui.h"
#include "invisibleBox.h"
#include "model.h"


void InvisibleBox::Init()
{
	// 透明な壁情報の設定
	m_Param.Position = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	m_Param.Scale = D3DXVECTOR3(5.0f, 2.0f, 5.0f);
	D3DXVECTOR3 length = m_Param.Scale - D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_Param.Length = D3DXVec3Length(&length);
	m_Param.Collision = false;
}

void InvisibleBox::Uninit()
{
	GameObject::Uninit();
}

void InvisibleBox::Update()
{
	if (m_Event)
	{
		// 紐づけされているイベントが終了している場合このオブジェクトも削除
		if (m_Event->GetDestroy()) SetDestroy();
	}

	GameObject::Update();
}

void InvisibleBox::Draw()
{
	if (!GUI::drawInvisibleBox) return; // デバッグ用

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// カラー設定
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
	Renderer::SetPostProcessParameter(param);

	// ポリゴン描写
	Resource::GetGroundModel()->Draw();
}

void InvisibleBox::DrawZPrePass()
{
	if (!GUI::drawInvisibleBox) return; // デバッグ用

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetGroundModel()->Draw();
}
