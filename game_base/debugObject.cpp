#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "model.h"
#include "debugObject.h"
#include "camera.h"
#include "input.h"

int DebugObject::m_Num = 0;

void DebugObject::Init()
{
	m_Position = D3DXVECTOR3((rand() % 100) / 10.0f - 5.0f, (rand() % 50) / 10.0f, (rand() % 100) / 10.0f - 5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(4.0f, 4.0f, 4.0f);

	m_DebugNumber = m_Num;
	m_Num++;
}

void DebugObject::Uninit()
{
}


void DebugObject::Update()
{
}


void DebugObject::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	Renderer::SetWorldMatrix(&world);

	// �e�N�X�`���ݒ�
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}


	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	material.Roughness = 0.5f;
	material.Metalic = 0.5f;

	Resource::GetSphereModel()->Draw(material);
}


void DebugObject::DrawDebug()
{
	// �f�o�b�O�I�u�W�F�N�g���̃p�����[�^��ݒ�
	CalcDebugParameter(m_DebugNumber);

	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	Renderer::SetWorldMatrix(&world);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetRedGradationTexture());

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	material.Roughness = (0.5f);
	material.Metalic = (0.5f);

	Resource::GetSphereModel()->Draw(material);
}


void DebugObject::CalcDebugParameter(int num)
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();


	// �e�J�X�P�[�h�̒��S���W����������I�u�W�F�N�g
	if (num >= 0 && num < 3)
	{
		m_Position = GUI::pos[num];
	}
	
	// ������̒��S���ʂ̊e���_����������I�u�W�F�N�g
	if (num >= 3 && num < 7)
	{
		m_Position = camera->GetPosition() + (GUI::frustumPos[num - 3] / 2.0f);
		m_Scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	}
}
