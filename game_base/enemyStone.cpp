#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "resource.h"
#include "enemyStone.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "rock.h"
#include "ground.h"
#include "enemyStateIdle.h"
#include "enemyStateStone.h"
#include "particle.h"
#include "random.h"


void EnemyStone::Init()
{
	m_EnemyState = new EnemyStateIdle(this);
	m_EnemyType = ENEMY_NORMAL;

	m_Position = D3DXVECTOR3(0.0f, 6.0f, 20.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_AttackRange = 6.0f;
	m_WalkRange = 30.0f;
}

void EnemyStone::Uninit()
{
	GameObject::Uninit();
}

void EnemyStone::Update()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 oldPosition = m_Position; // �O��̍��W
	D3DXVECTOR3 oldRotation = m_Rotation; // �O��̌���
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �G�l�~�[�X�e�[�g�X�V
	SetEnemyState();

	// �󋵂����ăG�l�~�[�̍s�������肷��
	m_EnemyState->StateAction();


	// �d��
	m_Velocity.y -= 0.15f;
	// �ړ�
	m_Position += m_Velocity;
	// ��Q���Ƃ̏Փ˔���
	float groundHeight = 0.0f;
	// �n��
	std::vector<Ground*> grounds = scene->GetGameObjects<Ground>();
	for (Ground* ground : grounds)
	{
		D3DXVECTOR3 position = ground->GetPosition();
		D3DXVECTOR3 scale = ground->GetScale();

		if (position.x - scale.x - PLAYER_RADIUS < m_Position.x &&
			m_Position.x < position.x + scale.x + PLAYER_RADIUS &&
			position.z - scale.z - PLAYER_RADIUS < m_Position.z &&
			m_Position.z < position.z + scale.z + PLAYER_RADIUS)
		{
			if (m_Position.y < position.y + scale.y * 2.0f)
			{
				if (groundHeight <= position.y + scale.y * 2.0f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
			}
			else if (groundHeight < position.y + scale.y * 2.0f)
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}
	// ��
	std::vector<Rock*> rocks = scene->GetGameObjects<Rock>();
	for (Rock* rock : rocks)
	{
		D3DXVECTOR3 position = rock->GetPosition();
		D3DXVECTOR3 scale = rock->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}
	}
	// �G�l�~�[
	std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
	for (GameObject* enemy : enemies)
	{
		if (enemy->GetPosition() == m_Position && enemy->GetRotation() == m_Rotation) break; // ���g�̎��̓X�L�b�v����

		D3DXVECTOR3 position = enemy->GetPosition();
		D3DXVECTOR3 scale = enemy->GetScale() + D3DXVECTOR3(1.0f, 0.0f, 1.0f); // �␳�l��^����

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x)
		{
			if (m_Position.y < position.y + scale.y)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}
	}
	// �v���C���[
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 position = player->GetPosition();
	D3DXVECTOR3 scale = player->GetScale() + D3DXVECTOR3(0.5f, 0.5f, 0.5f); // ����ɏ��Ȃ��悤�ɕ␳���|����
	D3DXVECTOR3 direction = m_Position - position;
	direction.y = 0.0f;
	float length = D3DXVec3Length(&direction);

	if (length < scale.x)
	{
		if (m_Position.y < position.y + scale.y)
		{
			m_Position.x = oldPosition.x;
			m_Position.z = oldPosition.z;
		}
		else
		{
			groundHeight = position.y + scale.y;
		}
	}
	// �ڒn
	if (m_Position.y < groundHeight + ENEMY_RADIUS)
	{
		m_Position.y = groundHeight + ENEMY_RADIUS;
		m_Velocity.y = 0.0f;
	}

	// ���S
	JudgeDeath();

	GameObject::Update();
}

void EnemyStone::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
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
	if (m_EnemyState->GetStateName() == ENEMY_STATE_STONE) material.Shininess = 2.5f;
	material.Roughness = 0.135f;
	material.Metalic = 0.26f;
	material.TextureEnable = true;

	// �|���S���`��
	Resource::GetEnemyStoneModel()->Draw(material);
}


void EnemyStone::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetEnemyStoneModel()->Draw();
}

void EnemyStone::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetEnemyStoneModel()->Draw();
}

void EnemyStone::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetEnemyStoneModel()->Draw();
}


void EnemyStone::SetDamage(float damage)
{
	// ��_���[�W���̓_���[�W���������Ȃ�
	if (m_EnemyState->GetStateName() == ENEMY_STATE_STONE) return;

	// �_���[�W����
	m_HitPoint -= damage;
	SetNextEnemyState(new EnemyStateStone(this));
	// �U���q�b�g���ɃJ������h�炷
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}
