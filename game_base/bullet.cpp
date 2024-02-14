#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "enemy.h"
#include "enemyMagic.h"
#include "player.h"
#include "bullet.h"
#include "model.h"
#include "effect.h"

void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.1f);

	// Audio�̃��[�h
	//m_AttackSE = AddComponent<Audio>();
	//m_AttackSE->Load("asset\\audio\\se\\enemy_attack.wav");
}

void Bullet::Uninit()
{
	GameObject::Uninit();
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	m_Position += m_Velocity;
	m_frame++;

	// Bullet�͈�莞�Ԍ����
	if (m_frame > 240)
	{
		SetDestroy();
	}

	// �v���C���[�Ƃ̏Փ˔���
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 playerPos = player->GetPosition();
	D3DXVECTOR3 direction = m_Position - playerPos;
	float length = D3DXVec3Length(&direction);

	if (length < 1.5f && player->GetPlayerIsDodge() == false)
	{
		player->SetDamage(1.0f);
		player->SetAttackedVector(D3DXVECTOR3(0.0f, 0.5f, 0.0f));

		SetDestroy();
	}

	GameObject::Update();
}

void Bullet::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
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
	param.color = D3DXVECTOR4(1.5f, 0.2f, 0.2f, 1.0f);
	Renderer::SetPostProcessParameter(param);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Roughness = 0.5f;
	material.Metalic = 0.12f;

	// �|���S���`��
	Resource::GetBulletModel()->Draw(material);
}


void Bullet::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetBulletModel()->Draw();
}

void Bullet::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetBulletModel()->Draw();
}