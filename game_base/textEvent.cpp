#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "event.h"
#include "eventManager.h"
#include "textEvent.h"
#include "player.h"
#include "camera.h"


void TextEvent::Uninit()
{
	m_VertexBuffer->Release();

	GameObject::Uninit();
}

void TextEvent::Update()
{
	// �C�x���g���ҋ@���̏ꍇ���s���Ȃ�
	if (m_EventState == EVENT_STATE_OFF) return;
	// �C�x���g���������̏ꍇ��ʂɕ\������C�x���gUI��؂�ւ���
	else if (m_EventState == EVENT_STATE_READY)
	{
		EventManager::SwitchTextEvent();
		m_EventState = EVENT_STATE_ON;
	}

	// �C�x���g���s��
	m_Alpha -= 0.00075f;

	// Alpha�l��0.0f�����������I�u�W�F�N�g�폜
	if (m_Alpha < 0.5f) SetDestroy();



	// �C�x���g���̌ŗL����------------------------------------------------------------------
	// �Q�[���N���A���̃��U���g
	if (m_TextEvent == TEXTEVENT_END)
	{
		m_Alpha = 0.75f;
	}
	// ���S��
	else if (m_TextEvent == TEXTEVENT_DEATH)
	{
		Scene* scene = Manager::GetScene();

		if (m_Destroy)
		{
			// ���X�|�[��
			scene->GetGameObject<Player>()->Respawn();
		}
		else
		{
			scene->GetGameObject<Camera>()->SetCameraOscillationMode(CO_MEDIUM_MODE);
			m_Alpha -= 0.0025f;
		}
	}
	// --------------------------------------------------------------------------------------




	GameObject::Update();
}

void TextEvent::Draw()
{
	// �C�x���g�����s���̏ꍇ�̂ݎ��s
	if (m_EventState == EVENT_STATE_OFF) return;


	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// �e�N�X�`���ݒ�
	switch (m_TextEvent)
	{
	case TEXTEVENT_TUTORIAL_JUMP:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetJumpTutorialTexture());
		break;
	case TEXTEVENT_TUTORIAL_PRESSATTACK:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetPressAttackTutorialTexture());
		break;
	case TEXTEVENT_TUTORIAL_CHARGEATTACK:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetChargeAttackTutorialTexture());
		break;
	case TEXTEVENT_TUTORIAL_DODGE:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetDashTutorialTexture());
		break;
	case TEXTEVENT_TLOB_TITLE:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetTLOBTitleTexture());
		break;
	case TEXTEVENT_TCF_TITLE:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetTCFTitleTexture());
		break;
	case TEXTEVENT_TSI_TITLE:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetTSITitleTexture());
		break;
	case TEXTEVENT_END:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetEndTexture());
		break;
	case TEXTEVENT_DEATH:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetDeathTexture());
		break;
	}

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, m_Alpha);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void TextEvent::InitTextEvent(float x, float y, float Width, float Height)
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

void TextEvent::SetTextEvent(TEXTEVENT eventNumber)
{
	switch (eventNumber)
	{
		// �`���[�g���A��
	case TEXTEVENT_TUTORIAL_JUMP:
		m_TextEvent = TEXTEVENT_TUTORIAL_JUMP;
		InitTextEvent(0.0f, 200.0f, 500.0f, 200.0f);

		break;
	case TEXTEVENT_TUTORIAL_PRESSATTACK:
		m_TextEvent = TEXTEVENT_TUTORIAL_PRESSATTACK;
		InitTextEvent(0.0f, 200.0f, 500.0f, 200.0f);

		break;
	case TEXTEVENT_TUTORIAL_CHARGEATTACK:
		m_TextEvent = TEXTEVENT_TUTORIAL_CHARGEATTACK;
		InitTextEvent(0.0f, 200.0f, 500.0f, 200.0f);

		break;
	case TEXTEVENT_TUTORIAL_DODGE:
		m_TextEvent = TEXTEVENT_TUTORIAL_DODGE;
		InitTextEvent(0.0f, 200.0f, 500.0f, 200.0f);

		 // �^�C�g���{�[�h
	case TEXTEVENT_TLOB_TITLE:
		m_TextEvent = TEXTEVENT_TLOB_TITLE;
		InitTextEvent(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case TEXTEVENT_TCF_TITLE:
		m_TextEvent = TEXTEVENT_TCF_TITLE;
		InitTextEvent(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case TEXTEVENT_TSI_TITLE:
		m_TextEvent = TEXTEVENT_TSI_TITLE;
		InitTextEvent(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

		break;
	case TEXTEVENT_END:
		m_TextEvent = TEXTEVENT_END;
		InitTextEvent(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case TEXTEVENT_DEATH:
		m_TextEvent = TEXTEVENT_DEATH;
		InitTextEvent(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	}
}