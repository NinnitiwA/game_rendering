#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "ground.h"
#include "model.h"

void Ground::Init()
{
	m_Scale = D3DXVECTOR3(20.0f, 20.0f, 20.0f);
}

void Ground::Uninit()
{
	GameObject::Uninit();
}

void Ground::Update()
{
	Scene* scene = Manager::GetScene();

	GameObject::Update();
}

void Ground::Draw()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// テクスチャ設定
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Metalic = 0.02f;
	material.Roughness = 0.52f;
	Renderer::SetMaterial(material);

	// ポリゴン描写
	Resource::GetGroundModel()->Draw(material);
}

void Ground::DrawShadowMapping()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetGroundModel()->Draw();
}

void Ground::DrawZPrePass()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetGroundModel()->Draw();
}

void Ground::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetGroundModel()->Draw();
}


void Ground::CreateGround(Scene* Scene, int X, int Z, D3DXVECTOR3 Position)
{
	// 地面の値を設定
	D3DXVECTOR3 basePosition = D3DXVECTOR3((X - 1) / 2.0f, -1.0f, (Z - 1) / 2.0f);

	// 地面生成
	for (int z = 0; z < Z; z++)
	{
		for (int x = 0; x < X; x++)
		{
			Ground* ground = Scene->AddGameObject<Ground>(1);
			ground->SetScale(D3DXVECTOR3(20.0f, 1.0f + Position.y / 2.0f, 20.0f));
			ground->SetPosition(D3DXVECTOR3(Position.x - 40.0f * (X - 1) / 2.0f + x * 40.0f, Position.y - ground->GetScale().y * 2.0f, Position.z - 40.0f * (Z - 1) / 2.0f + z * 40.0f));
		}
	}
}
