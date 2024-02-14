#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "rock.h"
#include "effect.h"
#include "particle.h"
#include "model.h"
#include "random.h"

void Rock::Init()
{
	float random = Random(0, 360);
	m_Rotation = D3DXVECTOR3(0.0f, random * D3DX_PI / 180.0f, 0.0f);
	m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	m_HitPoint = 1.0f;

	// Audioのロード
	//if (!m_BreakSE)
	//{
	//	m_BreakSE = AddComponent<Audio>();
	//	m_BreakSE->Load("asset\\audio\\se\\rock_break.wav");
	//}
}

void Rock::Uninit()
{
	GameObject::Uninit();
}

void Rock::Update()
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

void Rock::Draw()
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
	material.Roughness = 0.42f;
	Renderer::SetMaterial(material);

	// ポリゴン描写
	Resource::GetRockModel()->Draw(material);
}

void Rock::DrawShadowMapping()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetRockModel()->Draw();
}

void Rock::DrawZPrePass()
{
	// シェーダ設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetRockModel()->Draw();
}

void Rock::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetRockModel()->Draw();
}
