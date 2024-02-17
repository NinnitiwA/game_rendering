#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "dataStorage.h"
#include "scene.h"
#include "snakeBody.h"
#include "snakeHead.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "ground.h"
#include "ground.h"
#include "particle.h"
#include "model.h"

void SnakeBody::Init()
{

}

void SnakeBody::Uninit()
{
	GameObject::Uninit();
}

void SnakeBody::Update()
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

	GameObject::Update();
}

void SnakeBody::Draw()
{
	// ��^�[�Q�b�g���ɃA�E�g���C���`��
	if (m_isTarget) DrawOutline();

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
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
	Resource::GetSnakeBodyModel()->Draw(material);
}

void SnakeBody::DrawShadowMapping()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetSnakeBodyModel()->Draw();
}

void SnakeBody::DrawZPrePass()
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
	Resource::GetSnakeBodyModel()->Draw();
}

void SnakeBody::EnemyActionDecider()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 vecThisToPrePos = m_PreSnakePosition - m_Position;     // ���O�փI�u�W�F�N�g�Ɍ������x�N�g��
	float length = D3DXVec3Length(&vecThisToPrePos);

	// �ւ̐擪�I�u�W�F�N�g���擾
	SnakeHead* snakeHead = scene->GetGameObject<SnakeHead>();
	D3DXVECTOR3 vecThisToTacklePos = (m_PreSnakePosition * 1.0f + snakeHead->GetPosition() * 0.0f) - m_Position;

	// �U�����󂯂Ă���ꍇ
	if (m_Param.State == DAMAGED_STATE)
	{
		// �t���[���X�V
		m_Param.DamageFrame++;
		// �I������
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
		else SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame]);                                                    // �U������O
		m_Scale = DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimScale[m_Param.AttackFrame];
		m_Param.AttackFrame++;

		// �I������
		if (m_Param.AttackFrame >= SNAKE_MAX_ATTACK_FRAME)
		{
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			m_Param.State = WALK_STATE;
			m_Param.AttackFrame = 0;
			m_Param.WalkFrame = 0;
			m_Param.DamageFrame = 0;
		}

		// �U������O�ɍU�������x�N�g���𓾂�
		if (m_Param.AttackFrame == 6) D3DXVec3Normalize(&m_AttackVector, &vecThisToTacklePos);
		// �v���C���[�Ƃ̏Փ˔���
		if (m_Param.AttackFrame == 21) // �v���C���[����𒆂łȂ��ꍇ�Ɉ�t���[���̂ݓ����蔻����s��
		{
			AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// ���̃I�u�W�F�N�g�̏�Ԃ��ύX
		SnakeBody* nextSnakeBody = snakeHead->GetSnakeBody(m_Number + 1);
		if (m_Param.AttackFrame == 6 && nextSnakeBody && !nextSnakeBody->GetDestroy())
			nextSnakeBody->SetSnakeState(ATTACK_STATE);

		return;
	}


	// �U�����Ă��Ȃ��ꍇ
	{
		// ���O�I�u�W�F�N�g�Ƃ̋��������ȓ��̏ꍇ���s
		if (length >= 4.0f)
		{
			// �ҋ@��Ԃ��U��������ԂŃv���C���[�Ƌ��������ꂽ��
			if (m_Param.State == IDLE_STATE)
				m_Param.State = WALK_STATE;
		}
		else // �v���C���[�Ƃ̋���������Ă���Ƃ�
		{
			m_Param.State = IDLE_STATE;
		}

		// �ړ�����
		if ((m_Param.State == WALK_STATE) || m_Param.WalkFrame > 0)
		{
			D3DXVECTOR3 vector = vecThisToPrePos;
			float prePosLength = D3DXVec3Length(&vecThisToPrePos); // ���O�I�u�W�F�N�g�Ƃ̋���
			float gapMoveSpeed = (prePosLength / 4.0f) / 100.0f;
			vector.y = 0.0f; // Y���̈ړ��ʂ�0�ɂ���
			D3DXVec3Normalize(&vector, &vector);
			m_Velocity = (0.12f + snakeHead->GetMoveSpeed() + gapMoveSpeed) * vector;

			// �ړ��A�j���[�V����
			SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_Param.WalkFrame] * 1.2f);
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_Param.WalkFrame];
			m_Param.WalkFrame++;
			// �A�j���[�V�����t���[���J�E���g���Z�b�g
			if (m_Param.WalkFrame >= ENEMY_MAX_WALK_FRAME)
			{
				m_Param.WalkFrame = 0;
				m_Param.DamageFrame = 0;
			}
		}

	}
}

void SnakeBody::SetDamage(float damage)
{
	if (m_Param.DamageFrame != 0) return;

	// ���̂ւ̍U���̓_���[�W���قƂ�ǒʂ�Ȃ�
	m_HitPoint -= damage * 0.01f;
	// ���̂̏�Ԃ��_���[�W���ɍX�V
	m_Param.State = DAMAGED_STATE;

	// �U���q�b�g���ɃJ������h�炷
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}

void SnakeBody::SetDamageFromHead(float damage)
{
	if (m_Param.DamageFrame != 0) return;

	// �����̓_���[�W���ʂ�
	m_HitPoint -= damage;
	// ���̂̏�Ԃ��_���[�W���ɍX�V
	m_Param.State = DAMAGED_STATE;

	// �U���q�b�g���ɃJ������h�炷
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}
