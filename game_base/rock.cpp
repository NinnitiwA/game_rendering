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

	// Audio�̃��[�h
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

	// �j��
	if (m_HitPoint <= 0)
	{
		// �j�󎞂̃G�t�F�N�g���V�[���ɒǉ�
		scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 3.0f, 0.0f), PARTICLE_ROCK);
		//m_BreakSE->Play();

		SetDestroy();
	}

	GameObject::Update();
}

void Rock::Draw()
{
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �e�N�X�`���ݒ�
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// �J���[�ݒ�
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(0.8f, 0.515f, 0.148f, 0.0f);
	Renderer::SetPostProcessParameter(param);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Metalic = 0.02f;
	material.Roughness = 0.42f;
	Renderer::SetMaterial(material);

	// �|���S���`��
	Resource::GetRockModel()->Draw(material);
}

void Rock::DrawShadowMapping()
{
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetRockModel()->Draw();
}

void Rock::DrawZPrePass()
{
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetRockModel()->Draw();
}

void Rock::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetRockModel()->Draw();
}
