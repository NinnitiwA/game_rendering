#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "dataStorage.h"
#include "scene.h"
#include "sceneEvent.h"
#include "snakeHead.h"
#include "snakeBody.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "ground.h"
#include "invisibleBox.h"
#include "particle.h"
#include "model.h"

void SnakeHead::Init()
{
	// SnakeHead�ŗL�p�����[�^�̏�����
	m_MoveSpeed = 0.0f;
}

void SnakeHead::Uninit()
{
	GameObject::Uninit();
}

void SnakeHead::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 oldPosition = m_Position; // �O��̍��W
	D3DXVECTOR3 oldRotation = m_Rotation; // �O��̌���
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �󋵂����ăG�l�~�[�̍s�������肷��
	EnemyActionDecider();

	// �d��
	m_Velocity.y -= 0.15f;
	// �ړ�
	m_Position += m_Velocity;
	// ��Q���Ƃ̏Փ˔���
	float groundHeight = -20.0f;
	// �n��
	std::vector<Ground*> grounds = scene->GetGameObjects<Ground>();
	for (Ground* ground : grounds)
	{
		D3DXVECTOR3 position = ground->GetPosition();
		D3DXVECTOR3 scale = ground->GetScale();

		if (position.x - scale.x - ENEMY_RADIUS < m_Position.x &&
			m_Position.x < position.x + scale.x + ENEMY_RADIUS &&
			position.z - scale.z - ENEMY_RADIUS < m_Position.z &&
			m_Position.z < position.z + scale.z + ENEMY_RADIUS)
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
	// �f�o�b�O�p�n��
	std::vector<Field*> fields = scene->GetGameObjects<Field>();
	for (Field* field : fields)
	{
		groundHeight = 0.0f;
	}
	// �v���C���[
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
	if (m_Position.y < groundHeight + SNAKE_RADIUS)
	{
		m_Position.y = groundHeight + SNAKE_RADIUS;
		m_Velocity.y = 0.0f;
	}

	// ���S
	JudgeDeath();

	// �����փI�u�W�F�N�g�̐�
	int aliveSnakeNum = 0;

	// ���̃I�u�W�F�N�g�̍X�V����
	for (int i = 0; i <= SNAKE_BODY_NUM - 1; i++)
	{
		// ���O�փI�u�W�F�N�g�̍��W���i�[
		if (i == 0) m_SnakeBodyList[i]->SetPreSnakePosition(m_Position);
		else m_SnakeBodyList[i]->SetPreSnakePosition(m_SnakeBodyList[i - 1]->GetPosition());
		// ���Ԑ���
		m_SnakeBodyList[i]->SetSnakeBodyNumber(i);

		// �������J�E���g
		if (!m_SnakeBodyList[i]->GetDestroy()) aliveSnakeNum++;
	}

	// �ړ����x�X�V
	m_MoveSpeed = 0.04f * (SNAKE_BODY_NUM / (aliveSnakeNum + 1));


	GameObject::Update();
}

void SnakeHead::Draw()
{
	// ��^�[�Q�b�g���ɃA�E�g���C���`��
	if (m_isTarget) DrawOutline();

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(),  NULL, 0);
	if (m_Param.State == DAMAGED_STATE) Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);
	else Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

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
	material.Roughness = 0.5f;
	material.Metalic = 0.12f;
	material.TextureEnable = true;

	// �J���[�ݒ�
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(1.5f, 1.0f, 1.0f, 0.5f);
	Renderer::SetPostProcessParameter(param);

	// �|���S���`��
	Resource::GetSnakeHeadModel()->Draw(material);
}

void SnakeHead::DrawShadowMapping()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetSnakeHeadModel()->Draw();
}

void SnakeHead::DrawZPrePass()
{
	// ��^�[�Q�b�g���ɃA�E�g���C���`��
	if (m_isTarget) DrawOutline();

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	
	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetSnakeHeadModel()->Draw();
}


void SnakeHead::CreateSnakeBody(void)
{
	// ���̃I�u�W�F�N�g�ݒ�
	for (int i = 0; i <= SNAKE_BODY_NUM; i++)
	{
		// ���̃I�u�W�F�N�g�𐶐�
		Scene* scene = Manager::GetScene();
		SnakeBody* snakeBody = scene->AddGameObject<SnakeBody>(1);
		snakeBody->SetPosition(m_Position + D3DXVECTOR3(5.0f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM), 1.5f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM), 5.0f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM)));
		// �z��Ɋi�[
		m_SnakeBodyList[i] = snakeBody;
	}

	// �Ō���͋�̃I�u�W�F�N�g�ɂ��邽�ߎ��̂�6��
	m_SnakeBodyList[SNAKE_BODY_LAST_ALLAY_NUM + 1]->SetDestroy();
}

void SnakeHead::EnemyActionDecider()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Position; // �G�l�~�[����v���C���[�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecEnemyToPlayer);

	// �U�����󂯂Ă���ꍇ
	if (m_Param.State == DAMAGED_STATE)
	{
		// �q�b�g�o�b�N����
		D3DXVECTOR3 vector = D3DXVECTOR3(vecEnemyToPlayer.x, 0.0f, vecEnemyToPlayer.z); // �q�b�g�o�b�N����
		D3DXVec3Normalize(&vector, &vector);
		m_Velocity = -vector * 0.25f + D3DXVECTOR3(0.0f, 0.7f * cosf(D3DX_PI * ((float)m_Param.DamageFrame * 2 / (float)ENEMY_MAX_DAMAGE_FRAME)), 0.0f);

		// �I������
		m_Param.DamageFrame++;
		if (m_Param.DamageFrame >= SNAKE_MAX_DAMAGE_FRAME)
		{
			m_Param.DamageFrame = 0;
			m_Param.State = IDLE_STATE;
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
		return;
	}

	// �U�����̏ꍇ
	if (m_Param.State == ATTACK_STATE)
	{
		// �U���A�j���[�V����
		if (m_Param.AttackFrame >= 20) SetPosition(m_Position - DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame].z * m_AttackVector); // �U�������v���C���[�֌������Ĉړ�����
		else SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame]);                           // �U������O
		m_Scale = DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimScale[m_Param.AttackFrame];
		m_Param.AttackFrame++;

		// �I������
		if (m_Param.AttackFrame >= SNAKE_MAX_ATTACK_FRAME)
		{
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			if (length < 6.0f) m_Param.State = READY_STATE;
			else m_Param.State = WALK_STATE;
			m_Param.AttackFrame = 0;
			m_Param.WalkFrame = 0;
		}

		// �U������O�ɍU��������ݒ�
		if (m_Param.AttackFrame == 6) D3DXVec3Normalize(&m_AttackVector, &vecEnemyToPlayer);
		// �v���C���[�Ƃ̏Փ˔���i����̂P�t���[���œ����蔻����s���j
		if (m_Param.AttackFrame == 20)
		{
			AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// ���̃I�u�W�F�N�g�̏�Ԃ��ύX
		if (m_Param.AttackFrame == 6 && m_SnakeBodyList[0] && !m_SnakeBodyList[0]->GetDestroy())
			m_SnakeBodyList[0]->SetSnakeState(ATTACK_STATE);

		return;
	}

	// �U�����Ă��Ȃ��ꍇ
	{
		// �U������
		if (m_Param.State == READY_STATE)
		{
			m_Param.ReadyFrame++;

			// �U���J�n
			if (m_Param.ReadyFrame >= 45)
			{
				m_Param.State = ATTACK_STATE;
				m_Param.AttackFrame = 0;
				m_Param.ReadyFrame = 0;
			}
		}
		// �v���C���[�Ƃ̋��������ȓ��̏ꍇ�U��������Ԃ�
		else if (length < 5.0f) m_Param.State = READY_STATE;

		// �v���C���[�Ƃ̋��������ȓ��̏ꍇ���s
		if (length < 30.0f)
		{
			// �v���C���[�̕���������
			RotateToTarget(&vecEnemyToPlayer);
			// �͈͓��Ƀv���C���[�������Ă���ꍇ
			if (length < 30.0f)
			{
				// �ҋ@��Ԃ��U��������ԂŃv���C���[�Ƌ��������ꂽ��
				if (m_Param.State == IDLE_STATE || length >= 20.0f)
					m_Param.State = WALK_STATE;
			}
		}
		else // �v���C���[�Ƃ̋���������Ă���Ƃ�
		{
			m_Param.State = IDLE_STATE;
		}

		// �ړ���
		if ((m_Param.State == WALK_STATE) || m_Param.WalkFrame > 0)
		{
			D3DXVECTOR3 vector = vecEnemyToPlayer;
			vector.y = 0.0f; // Y���̈ړ��ʂ͍l�����Ȃ�
			D3DXVec3Normalize(&vector, &vector);
			m_Velocity = (0.12 + m_MoveSpeed) * vector;

			// �ړ��A�j���[�V����
			SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_Param.WalkFrame] * 1.2f);
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_Param.WalkFrame];
			m_Param.WalkFrame++;
			// �A�j���[�V�����t���[���J�E���g���Z�b�g
			if (m_Param.WalkFrame >= ENEMY_MAX_WALK_FRAME) m_Param.WalkFrame = 0;
		}
	}
}

void SnakeHead::SetDamage(float damage)
{
	// �����̏�Ԃ��_���[�W���ɍX�V
	m_Param.State = DAMAGED_STATE;

	// �������̍Ō�����̃I�u�W�F�N�g��T��
	int aliveLastNumber = GetAliveSnakeBodyLastArrayNumber();

	// �������Ă��钆�ōŌ���̃I�u�W�F�N�g�Ń_���[�W����
	if (aliveLastNumber < 0)
	{
		m_HitPoint -= damage;
		// �U���q�b�g���ɃJ������h�炷
		Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
		camera->SetCameraOscillationMode(CO_LARGE_MODE);
	}
	else m_SnakeBodyList[aliveLastNumber]->SetDamageFromHead(damage);
}

void SnakeHead::JudgeDeath()
{
	// HP��0�܂��͗��������ꍇ
	if (m_HitPoint <= 0 || m_Position.y < -15.0f)
	{
		Scene* scene = Manager::GetScene();
		// ���S���̉��o
		scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 2.0f, 0.0f), PARTICLE_ENEMYDEATH);
		// ���S����
		SetDestroy();

		// �V�[���C�x���g����
		// ���U���g�V�[���ɐ���
		InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
		invisibleBox->SetPosition(D3DXVECTOR3(m_Position));
		invisibleBox->SetScale(D3DXVECTOR3(60.0f, 4.0f, 60.0f));
		SceneEvent* sceneEvent = scene->AddGameObject<SceneEvent>(3);
		sceneEvent->SetSceneEvent(SCENEEVENT_GAME_RESULT);
		invisibleBox->SetEvent(sceneEvent);
	}
}

int SnakeHead::GetAliveSnakeBodyLastArrayNumber()
{
	for (int i = 0; i <= SNAKE_BODY_NUM; i++)
	{
		if (m_SnakeBodyList[i]->GetDestroy()) return i - 1;
	}
	return SNAKE_BODY_LAST_ALLAY_NUM;
}
