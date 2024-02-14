#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "playerUI.h"
#include "player.h"

int PlayerUI::m_Count = 0;
float PlayerUI::m_Alpha = 1.0f;

void PlayerUI::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_PlayerStatus.HP = D3DXVECTOR4(PLAYER_MAX_HP, PLAYER_MAX_HP, 0.0f, 0.0f);
	m_PlayerStatus.HP.y = player->GetPlayerParameter().HitPoint;
	m_PlayerStatus.Status = D3DXVECTOR4(3.0f, 3.0f, 0.0f, 0.0f);

	{
		VERTEX_3D vertex[36];

		// HP
		for (int i = 0; i < 5; i++)
		{
			vertex[0 + i * 4].Position = D3DXVECTOR3(50.0f + i * 55.0f, 50.0f, 0.0f);
			vertex[0 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[0 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[0 + i * 4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

			vertex[1 + i * 4].Position = D3DXVECTOR3(100.0f + i * 55.0f, 50.0f, 0.0f);
			vertex[1 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[1 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1 + i * 4].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

			vertex[2 + i * 4].Position = D3DXVECTOR3(60.0f + i * 55.0f, 100.0f, 0.0f);
			vertex[2 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[2 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[2 + i * 4].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

			vertex[3 + i * 4].Position = D3DXVECTOR3(110.0f + i * 55.0f, 100.0f, 0.0f);
			vertex[3 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[3 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3 + i * 4].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		}

		// Attack
		{
			vertex[32].Position = D3DXVECTOR3(50.0f, 105.0f, 0.0f);
			vertex[32].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[32].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[32].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

			vertex[33].Position = D3DXVECTOR3(320.0f, 105.0f, 0.0f);
			vertex[33].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[33].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[33].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

			vertex[34].Position = D3DXVECTOR3(60.0f, 115.0f, 0.0f);
			vertex[34].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[34].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[34].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

			vertex[35].Position = D3DXVECTOR3(330.0f, 115.0f, 0.0f);
			vertex[35].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[35].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[35].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		}

		// Stamina
		for (int i = 0; i < 3; i++)
		{
			vertex[20 + i * 4].Position = D3DXVECTOR3(50.0f + i * 65.0f, 120.0f, 0.0f);
			vertex[20 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[20 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[20 + i * 4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

			vertex[21 + i * 4].Position = D3DXVECTOR3(110.0f + i * 65.0f, 120.0f, 0.0f);
			vertex[21 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[21 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[21 + i * 4].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

			vertex[22 + i * 4].Position = D3DXVECTOR3(60.0f + i * 65.0f, 150.0f, 0.0f);
			vertex[22 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[22 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[22 + i * 4].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

			vertex[23 + i * 4].Position = D3DXVECTOR3(120.0f + i * 65.0f, 150.0f, 0.0f);
			vertex[23 + i * 4].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vertex[23 + i * 4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[23 + i * 4].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 36;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_PlayerUIVertexBuffer);
	}
}

void PlayerUI::Uninit()
{
	m_PlayerUIVertexBuffer->Release();

	GameObject::Uninit();
}

void PlayerUI::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();


	// �v���C���[�X�e�[�^�X�X�V
	// HP
	if (m_PlayerStatus.HP.y > player->GetPlayerParameter().HitPoint)   // HP�����������ꍇ
	{
		m_PlayerStatus.HP.z += m_PlayerStatus.HP.y - player->GetPlayerParameter().HitPoint;
		m_PlayerStatus.HP.y = player->GetPlayerParameter().HitPoint;
	}
	if (m_PlayerStatus.HP.y < player->GetPlayerParameter().HitPoint)   // HP���񕜂����ꍇ
	{
		m_PlayerStatus.HP.y = player->GetPlayerParameter().HitPoint;
		if (PLAYER_MAX_HP < m_PlayerStatus.HP.y) m_PlayerStatus.HP.y = PLAYER_MAX_HP; // �ő�HP�͉z���Ȃ�
	}
	if (m_PlayerStatus.HP.z > 0.0f) m_PlayerStatus.HP.z -= 0.03f;

	// �X�^�~�i
	if (m_PlayerStatus.Status.y < m_PlayerStatus.Status.x)       // �X�^�~�i�����������ꍇ
	{
		m_PlayerStatus.Status.w += 0.0075;
		if (m_PlayerStatus.Status.w > 1.0f)
		{
			m_PlayerStatus.Status.w = 0.0f;
			m_PlayerStatus.Status.y += 1.0f;
		}
	}
	if (m_PlayerStatus.Status.z > 0.0f) m_PlayerStatus.Status.z -= 0.03;


	// �v���C���[���S
	if (m_PlayerStatus.HP.y <= 0.0f && player->GetCanControl())
	{
		// ���X�|�[������
		player->SetRespawn();
	}

	// �J�E���g
	m_Count++;
	// ��莞�Ԉȏ�v���C���[�X�e�[�^�X�ɕϓ����Ȃ��ꍇ��UI������
	if (m_Count > 360 && m_Alpha > 0.0f)m_Alpha -= 0.015f;

	GameObject::Update();
}

void PlayerUI::Draw()
{
	// workspace�ł͕`�悵�Ȃ�
	if (Manager::GetScene()->GetSceneName() == WORFSPACE_SCENE) return;



	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetGaugeVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetGaugePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_PlayerUIVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// HP�`��
	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, m_Alpha); // HP�J���[
	Renderer::SetMaterial(material);
	for (int i = 0; i < 5; i++)
	{
		// PlayerStatus�ݒ�
		D3DXVECTOR4 hp = m_PlayerStatus.HP;
		PLAYERPARAMETER param{};
		hp = D3DXVECTOR4(1.0f, hp.y - (5.0f - 1 - i), 1.0f - (hp.y - (5.0f - 1 - i)) < hp.z ? 1.0f - (hp.y - (5.0f - 1 - i)) : hp.z, 0.0f);
		param.Status = hp;
		// PlayerStatus�ݒ�
		Renderer::SetPlayerParameter(param);

		// �`��
		Renderer::GetDeviceContext()->Draw(4, (5 - 1 - i) * 4);
	}


	// Attack�`��
	// �}�e���A���ݒ�
	material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha); // HP�J���[
	Renderer::SetMaterial(material);
	// PlayerStatus�ݒ�
	PLAYERPARAMETER param{};
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	param.Status = D3DXVECTOR4(1.0f, (float)player->GetPlayerParameter().AttackFrame / (float)PLAYER_ATTACK_INTERVAL, 0.0f, 0.0f);
	// PlayerStatus�ݒ�
	Renderer::SetPlayerParameter(param);
	// �`��
	Renderer::GetDeviceContext()->Draw(4, 32);


	// Stamina�`��
	// �}�e���A���ݒ�
	material.Diffuse = D3DXCOLOR(0.0f, 0.2f, 1.0f, m_Alpha); // �X�^�~�i�J���[
	Renderer::SetMaterial(material);
	for (int i = 1; i < 4; i++)
	{
		// PlayerStatus�ݒ�
		PLAYERPARAMETER param{};
		D3DXVECTOR4 ps = m_PlayerStatus.Status + D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.0f);
		D3DXVECTOR4 finalps;
		if (ps.y + ps.w >= i && ps.y + ps.w < i + 1)  // �X�^�~�i�񕜒�
		{
			finalps = D3DXVECTOR4(1.0f, 0.0f, 0.0f, ps.w);
		}
		else if (ps.y < i)                            // �X�^�~�i���O�̎�
		{
			finalps = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (ps.y >= i)                           // �X�^�~�i�����^���̎�
		{
			finalps = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.0f);
		}
		param.Status = finalps;
		Renderer::SetPlayerParameter(param);

		// �`��
		Renderer::GetDeviceContext()->Draw(4, i * 4 + 16);
	}
}
