#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "arrow.h"
#include "particle.h"
#include "model.h"
#include "random.h"

void Arrow::Init()
{
	m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 90.0f * D3DX_PI / 180.0f, 0.0f);

	m_HitPoint = 1.0f;
}

void Arrow::Uninit()
{
	GameObject::Uninit();
}

void Arrow::Update()
{
	Scene* scene = Manager::GetScene();

	// 破壊
	if (m_HitPoint <= 0)
	{
		// 破壊時のエフェクトをシーンに追加
		scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 3.0f, 0.0f), PARTICLE_ROCK);
		//m_BreakSE->Play();

		SetDestroy();
	}

	GameObject::Update();
}

void Arrow::Draw()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// テクスチャ設定
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// カラー設定
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(0.8f, 0.515f, 0.148f, 0.0f);
	Renderer::SetPostProcessParameter(param);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Metalic = 0.02f;
	material.Roughness = 0.68f;
	Renderer::SetMaterial(material);

	// ポリゴン描写
	Resource::GetArrowModel()->Draw(material);
}

void Arrow::DrawShadowMapping()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetArrowModel()->Draw();
}

void Arrow::DrawZPrePass()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetArrowModel()->Draw();
}

void Arrow::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetArrowModel()->Draw();
}
