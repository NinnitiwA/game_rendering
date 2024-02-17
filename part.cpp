#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "manager.h"
#include "scene.h"
#include "random.h"
#include "camera.h"
#include "player.h"
#include "part.h"
#include "field.h"
#include "ground.h"
#include "invisibleBox.h"


Audio* Part::m_HealSE = NULL;

void Part::Init()
{
	// �p�����[�^�ݒ�
	float random = Random(0, 360);
	m_Rotation = D3DXVECTOR3(0.0f, random *  D3DX_PI / 180.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	m_RotateVector = D3DXVECTOR3(sinf(m_Rotation.y) / 10.0f, 0.55f + random / 3600.0f, cosf(m_Rotation.y) / 10.0f); // �e�p�[�e�B�N�����ƂɃ����_���Őݒ�
	m_MaterialColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Frame = 0;


	// Audio�̃��[�h
	//if (!m_HealSE)
	//{
	//	m_HealSE = AddComponent<Audio>();
	//	m_HealSE->Load("asset\\audio\\se\\heal.wav");
	//}
}

void Part::Uninit()
{
	GameObject::Uninit();
}

void Part::Update()
{
	Scene* scene = Manager::GetScene();
	float radius = m_Scale.y;

	// ��Q���Ƃ̏Փ˔���
	float groundHeight = 0.0f; // ���ʂ̍���
	// �n��
	std::vector<Ground*> grounds = scene->GetGameObjects<Ground>();
	for (Ground* ground : grounds)
	{
		D3DXVECTOR3 position = ground->GetPosition();
		D3DXVECTOR3 scale = ground->GetScale();

		if (position.x - scale.x - radius < m_Position.x &&
			m_Position.x < position.x + scale.x + radius &&
			position.z - scale.z - radius < m_Position.z &&
			m_Position.z < position.z + scale.z + radius)
		{
			groundHeight = position.y + scale.y * 2.0f;
		}
	}
	// �f�o�b�O�p�n��
	std::vector<Field*> fields = scene->GetGameObjects<Field>();
	for (Field* field : fields)
	{
		D3DXVECTOR3 position = field->GetPosition();
		D3DXVECTOR3 scale = field->GetScale();

		if (position.x - scale.x - radius < m_Position.x &&
			m_Position.x < position.x + scale.x + radius &&
			position.z - scale.z - radius < m_Position.z &&
			m_Position.z < position.z + scale.z + radius)
		{
			groundHeight = position.y + scale.y * 2.0f;
		}
	}
	// �����ȕ�
	std::vector<InvisibleBox*> invisibleBoxes = scene->GetGameObjects<InvisibleBox>();
	for (InvisibleBox* invisibleBox : invisibleBoxes)
	{
		// �Փ˔��肪�L���ŋ��������ȉ��̏ꍇ������s
		if (invisibleBox->GetInvisibleBoxParameter().Collision && invisibleBox->GetInvisibleBoxParameter().Length <= 20.0f)
		{
			D3DXVECTOR3 position = invisibleBox->GetPosition();
			D3DXVECTOR3 scale = invisibleBox->GetScale();

			if (position.x - scale.x - radius < m_Position.x &&
				m_Position.x < position.x + scale.x + radius &&
				position.z - scale.z - radius < m_Position.z &&
				m_Position.z < position.z + scale.z + radius)
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}
	// ��]
	m_Rotation.x += 0.25f;
	// ��]�����Ɉړ�
	m_Position += m_RotateVector;
	// �d�͈ړ�
	m_RotateVector.y -= 0.0245f;
	if (m_Position.y <= groundHeight + m_Scale.y) m_RotateVector.y *= -m_CoefficientOfRestitution;


	m_Frame++;
	// �I������
	if (m_Frame >= 90)
	{
		switch (m_ParticleType)
		{
		case PARTICLE_ROCK:
			// ���X�ɐF��ω�
			if (m_MaterialColor.a > 0.5f)
			{
				m_MaterialColor -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.01f);
			}
			// �I�u�W�F�N�g�폜
			else SetDestroy();
			break;


		case PARTICLE_PLAYERDEATH:
			// ���X�ɐF��ω�
			if (m_MaterialColor.a > 0.5f)
			{
				m_MaterialColor -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.01f);
			}
			// �I�u�W�F�N�g�폜
			else SetDestroy();
			break;


		case PARTICLE_ENEMYDEATH:
			// �v���C���[�̕����ֈړ�
			Player* player = scene->GetGameObject<Player>();
			D3DXVECTOR3 playerPos = player->GetPosition();
			D3DXVECTOR3 moveVector = playerPos - m_Position; // �ړ������x�N�g��
			float length = D3DXVec3Length(&moveVector);      // �v���C���[�Ƃ̋���
			D3DXVec3Normalize(&moveVector, &moveVector);
			m_Position -= m_RotateVector;                              // ��]�ړ����𖳌��ɂ���
			m_Position += (moveVector / 3.0f) * (m_Frame / 90) * 2.5f; // �v���C���[�����̈ړ������Z����

			// �v���C���[�Ƃ̓����蔻��
			if (length <= PLAYER_RADIUS + radius + 1.2f)
			{
				// �T�E���h
				//m_HealSE->Play();

				// �v���C���[��
				player->SetHealHP(0.05f);
				SetDestroy();
			}

			// �I�u�W�F�N�g�����폜
			if (m_Frame >= 2700) SetDestroy();
			break;
		}
	}
}


void Part::Draw()
{
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
	param.color = (D3DXVECTOR4)m_MaterialColor;
	Renderer::SetPostProcessParameter(param);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	if (m_ParticleType == PARTICLE_ENEMYDEATH) material.Shininess = 1.5f;
	material.Roughness = 0.5f;
	material.Metalic = 0.12f;

	// �|���S���`��
	Resource::GetEnemyParticleModel()->Draw(material);
}


void Part::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetEnemyParticleModel()->Draw();
}

void Part::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetEnemyParticleModel()->Draw();
}

void Part::DrawReflection()
{
	// ���ʈȉ��̏ꍇ�͕`�斳��
	if (m_Position.y < 0.0f) return;

	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Resource::GetEnemyParticleModel()->Draw();
}
