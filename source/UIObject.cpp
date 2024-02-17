#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "UIObject.h"


void UIObject::Init(float x, float y, float Width, float Height)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(x + Width, y, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(x, y + Height, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(x + Width, y + Height, 0.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	vertex[0].Normal = vertex[1].Normal = vertex[2].Normal = vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = vertex[1].Diffuse = vertex[2].Diffuse = vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void UIObject::Uninit()
{
	m_VertexBuffer->Release();
	GameObject::Uninit();
}

void UIObject::Update()
{
	// �t���[���X�V
	m_Frame++;


	// UI�^�C�v�ʏ���
	switch (m_Type)
	{
	case NORMAL_TYPE:
		if (m_Frame > 240) m_Frame = 240;
		break;

	case BLINK_TYPE:
		if (m_Frame > 240) m_Frame = 0;
		break;
	}
}

void UIObject::Draw()
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
	switch (m_Number)
	{
	case 0: // �^�C�g�����
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetTitleTexture());
		break;
	
	case 1: // �^�C�g���Z���N�g���
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetTitleSelectTexture());
		break;
	}

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fabs(cosf(180.0f * ((float)m_Frame / 240.0f) * D3DX_PI / 180.0f)) * 0.85f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}


void UIObject::SetVertexBuffer()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
}
