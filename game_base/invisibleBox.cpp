#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "gui.h"
#include "invisibleBox.h"
#include "model.h"


void InvisibleBox::Init()
{
	// �����ȕǏ��̐ݒ�
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
		// �R�Â�����Ă���C�x���g���I�����Ă���ꍇ���̃I�u�W�F�N�g���폜
		if (m_Event->GetDestroy()) SetDestroy();
	}

	GameObject::Update();
}

void InvisibleBox::Draw()
{
	if (!GUI::drawInvisibleBox) return; // �f�o�b�O�p

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �J���[�ݒ�
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
	Renderer::SetPostProcessParameter(param);

	// �|���S���`��
	Resource::GetGroundModel()->Draw();
}

void InvisibleBox::DrawZPrePass()
{
	if (!GUI::drawInvisibleBox) return; // �f�o�b�O�p

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetGroundModel()->Draw();
}
