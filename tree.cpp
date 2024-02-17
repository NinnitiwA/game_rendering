#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "tree.h"
#include "particle.h"
#include "model.h"
#include "random.h"

void Tree::Init()
{
	float random = Random(0, 360);
	m_Rotation = D3DXVECTOR3(0.0f, random * D3DX_PI / 180.0f, 0.0f);
	float scale = Random(20, 40);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * (scale / 20.0f);
	m_BaseScale = m_Scale;

	// �ڒn�ʒu�ɕύX
	m_Position.y -= m_Scale.y / 2.0f;

	// �؃��f���̎�ސݒ�
	m_TreeType = Random(0, 2);
}

void Tree::Uninit()
{
	GameObject::Uninit();
}

void Tree::Update()
{
	if (!m_isSway) return;


	// �h��̏���
	{
		float rad = 0.0f; // �h��̊p�x
		if (m_SwayFrame % 2 == 0) rad = m_SwayFrame / 8.0f;
		else rad -= m_SwayFrame / 8.0f;

		m_Scale = m_BaseScale + D3DXVECTOR3( cosf(D3DX_PI * 0.5f * rad) * 0.15f, cosf(D3DX_PI * 0.5f * rad) * 0.15f, cosf(D3DX_PI * 0.5f * rad) * 0.15f);

		// �t���[���J�E���g
		m_SwayFrame++;
		// �h��I��
		if (m_SwayFrame > 10)
		{
			m_SwayFrame = 0;
			m_isSway = false;
			m_Scale = m_BaseScale;
		}
	}

	GameObject::Update();
}

void Tree::Draw()
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
	param.color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	Renderer::SetPostProcessParameter(param);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Metalic = 0.02f;
	material.Roughness = 0.12f;
	Renderer::SetMaterial(material);

	// �|���S���`��
	Resource::GetTreeModel(m_TreeType)->Draw(material);
}

void Tree::DrawShadowMapping()
{
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetTreeModel(m_TreeType)->Draw();
}

void Tree::DrawZPrePass()
{
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetTreeModel(m_TreeType)->Draw();
}

void Tree::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetTreeModel(m_TreeType)->Draw();
}
