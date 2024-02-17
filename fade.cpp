#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "fade.h"

bool Fade::m_FadeTexture = true;

void Fade::Init()
{
	UIObject::Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Fade::Uninit()
{
	UIObject::Uninit();
}

void Fade::Update()
{
	// �t�F�C�h�C��
	if (m_Fade)
	{
		m_Alpha += 0.01f;
		if (m_Alpha >= 1.0f)
		{
			m_Alpha = 1.0f;
			m_FadeFinished = true;
		}
	}
	// �t�F�C�h�A�E�g
	else
	{
		m_Alpha -= 0.01f;
		if (m_Alpha <= 0.0f)
		{
			m_Alpha = 0.0f;
		}
	}

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha);
	GameObject::Update();
}

void Fade::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// ���_�o�b�t�@�ݒ�
	SetVertexBuffer();

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// �e�N�X�`���ݒ�
	if(m_FadeTexture) Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetFadeTexture());
	else Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetKenshouTexture());

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}