#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "effect.h"

void Effect::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; // DYNAMIC : ���_�o�b�t�@���������\
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // D3D11_CPU_ACCESS_WRITE : ���_�o�b�t�@���������\

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �V�F�[�_�ݒ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	Renderer::CreateVertexShader(&m_DepthVertexShader, &m_VertexLayout, "shader\\depthVS.cso");
	Renderer::CreatePixelShader(&m_DepthPixelShader, "shader\\depthPS.cso");
}

void Effect::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_DepthVertexShader->Release();
	m_DepthPixelShader->Release();
}

void Effect::Update()
{
	m_Frame++;

	if (m_Frame >= m_EffectInfo->m_DivideX * m_EffectInfo->m_DivideY) SetDestroy();
}

void Effect::Draw()
{
	// �e�N�X�`�����W�Z�o
	float x = m_Frame % m_EffectInfo->m_DivideX * (1.0f / m_EffectInfo->m_DivideX);
	float y = m_Frame / m_EffectInfo->m_DivideY * (1.0f / m_EffectInfo->m_DivideY);

	// ���_�f�[�^
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / m_EffectInfo->m_DivideX), y);

		vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / m_EffectInfo->m_DivideY));

		vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / m_EffectInfo->m_DivideX), y + (1.0f / m_EffectInfo->m_DivideY));

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	if (Renderer::GetRenderState() == RENDER_STATE_GENERAL)
	{
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	}
	else if (Renderer::GetRenderState() == RENDER_STATE_DEPTH)
	{
		Renderer::GetDeviceContext()->VSSetShader(m_DepthVertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_DepthPixelShader, NULL, 0);
	}
	else return;

	// �u�����h�X�e�[�gATC�ݒ�
	Renderer::SetATCEnable(TRUE);

	// �r���{�[�h�ݒ�
	// �J�����̃r���[�}�g���b�N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX view = camera->GetViewMatrix();
	// �r���[�̋t�s��
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view); // �t�s��
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;
	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

	// �u�����h�X�e�[�g�ݒ�
	Renderer::SetATCEnable(FALSE);
}

void Effect::SetEffect(D3DXVECTOR3 pos, EffectType effectType)
{
	// ���W�ݒ�
	SetPosition(pos);

	// �G�t�F�N�g�p�����[�^�ݒ�
	static EffectInfo et;
	m_EffectInfo = &et;
	switch (effectType)
	{
	case EFFECT_EXPLOSION:
		strcpy(m_EffectInfo->m_TextureName, "asset/texture/volcano.png");
		m_EffectInfo->m_EffectType = EFFECT_EXPLOSION;
		m_EffectInfo->m_DivideX = 5;
		m_EffectInfo->m_DivideY = 5;
		break;
	case EFFECT_IMPULSE:
		strcpy(m_EffectInfo->m_TextureName, "asset/texture/impulse.png");
		m_EffectInfo->m_EffectType = EFFECT_IMPULSE;
		m_EffectInfo->m_DivideX = 10;
		m_EffectInfo->m_DivideY = 1;
		break;
	case EFFECT_NON:
		break;
	default:
		break;
	}

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		m_EffectInfo->m_TextureName,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}