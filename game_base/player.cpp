#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "player.h"
#include "enemy.h"
#include "rock.h"
#include "tree.h"
#include "ground.h"
#include "field.h"
#include "camera.h"
#include "invisibleBox.h"
#include "textEvent.h"
#include "particle.h"
#include "gui.h"
#include "playerUI.h"
#include "input.h"
#include "dataStorage.h"

D3DXVECTOR3 Player::m_RespwanPosition = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
bool Player::m_CanControl = true;

void Player::Init()
{
	m_Position = D3DXVECTOR3( 0.0f, 6.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );


	// �p�����[�^�̏�����
	m_Param.HitPoint = PLAYER_MAX_HP;
	m_Param.AttackFrame = 0;
	m_Param.canAttack = false;
	m_Param.dashLevel = 0.0f;
	m_Param.isFindEnemy = false;
	m_Param.isDamaged = false;
	m_Param.DamageFrame = 0;

	// �A�j���[�V������񏉊���
	m_PlayerAnim.isAttack = false;
	m_PlayerAnim.AttackFrame = 0;
	m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;
	m_PlayerAnim.isGround = true;
	m_PlayerAnim.isJump = false;
	m_PlayerAnim.JumpFrame = 0;
	m_PlayerAnim.isWalk = false;
	m_PlayerAnim.AnimFrame = 0;
	m_PlayerAnim.isDodge = false;
	m_PlayerAnim.DodgeFrame = 0;
}

void Player::Uninit()
{
}


void Player::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 oldPosition = m_Position; // �O��̍��W
	D3DXVECTOR3 oldRotation = m_Rotation; // �O��̌���
	m_Velocity = D3DXVECTOR3(0.0f, m_Velocity.y, 0.0f);
	m_PlayerAnim.isWalk = false;
	m_CanControl = GUI::playerControllFlag;
	if (m_Param.HitPoint > PLAYER_MAX_HP) m_Param.HitPoint = PLAYER_MAX_HP; // �ő�HP�͒����Ȃ�


	// �v���C���[����s�\�ȏꍇ
	if (!m_CanControl || scene->GetSceneName() == TITLE_SCENE)return;


	// �U�����󂯂Ă���ꍇ
	if (m_Param.isDamaged)
	{
		// �s�����~
		m_PlayerAnim.isAttack = false;
		m_PlayerAnim.AttackFrame = 0;
		m_PlayerAnim.isWalk = false;
		m_PlayerAnim.AnimFrame = 0;
		m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// �q�b�g�X�g�b�v��
		if (m_Param.DamageFrame == 0)
		{
			// �U���q�b�g���ɃJ������h�炷
			camera->SetCameraOscillationMode(CO_LARGE_MODE);
		}
		// �I������
		else if (m_Param.DamageFrame >= PLAYER_MAX_DAMAGE_FRAME)
		{
			m_Param.DamageFrame = 0;
			m_Param.isDamaged = false;
		}
		m_Param.DamageFrame++;

		// �Ђ��
		m_AttackedVector.y = 0.0f;
		D3DXVec3Normalize(&m_AttackedVector, &m_AttackedVector);
		m_Velocity = -m_AttackedVector * 0.2f * cosf(D3DX_PI * ((float)(m_Param.DamageFrame * 0.6) / (float)PLAYER_MAX_DAMAGE_FRAME));
	}
	// �U�����󂯂Ă��Ȃ��ꍇ
	else
	{
		// �U�����̏ꍇ
		if (m_PlayerAnim.isAttack)
		{
			// �v���X�U��
			if (m_PlayerAnim.PlayerAttack == PLAYER_PRESS_ATTACK)
			{
				// �U���A�j���[�V����
				m_Position += DataStorage::GetPlayerDataStorage()->PressAttackAnimPos[m_PlayerAnim.AttackFrame];
				m_Scale = DataStorage::GetPlayerDataStorage()->PressAttackAnimScale[m_PlayerAnim.AttackFrame];

				// �A�j���[�V�����t���[���X�V
				m_PlayerAnim.AttackFrame++;
				// �I������
				if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_PRESS_ATTACK_FRAME)
				{
					m_Scale.y = 1.0f;
					m_PlayerAnim.isAttack = false;
					m_PlayerAnim.AttackFrame = 0;
					m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;
				}

				// �G�Ƃ̏Փ˔���(��t���[���̂ݓ����蔻����s��)
				if (m_PlayerAnim.AttackFrame == 3) AttackCollisionDetection();

				// �T�E���h
				//if (m_PlayerAnim.AttackFrame == 0) m_ChargeLightSE->Play();
			}
			// �_�b�V���U��
			else if (m_PlayerAnim.PlayerAttack == PLAYER_DASH_ATTACK)
			{
				// �U������
				if (m_PlayerAnim.AttackFrame < PLAYER_MAX_DASH_ATTACK_PRE_FRAME - 1)
				{
					// �U���A�j���[�V����
					m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // �����Ă�������ɑ΂��č��W���X�V
						* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z;
					m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

					// �A�j���[�V�����t���[���X�V
					m_PlayerAnim.AttackFrame++;
					// �_�b�V���U�����ߎ��Ԃ��_�b�V�����x���ɕۑ�
					m_Param.dashLevel = m_PlayerAnim.AttackFrame;

					// �T�E���h
					//if (m_PlayerAnim.AttackFrame == 0)m_ChargeSE->Play();
				}
				// �U���J�n
				if (Input::GetKeyExit('L'))
				{
					m_PlayerAnim.AttackFrame = 32;       // �A�j���[�V�����t���[�����U�����O�̃t���[���ɕύX

					// �T�E���h
					//if (m_Param.dashLevel <= 20)m_ChargeLightSE->Play();
					//else m_ChargeHeavySE->Play();
				}
				// �U����
				if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_PRE_FRAME)
				{
					// �U���A�j���[�V����
					m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // �����Ă�������ɑ΂��č��W���X�V
						* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z
						* ((m_Param.dashLevel * 2.5) / PLAYER_MAX_DASH_ATTACK_PRE_FRAME);
					m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

					// �A�j���[�V�����t���[���X�V
					m_PlayerAnim.AttackFrame++;
				}


				// �I������
				if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_FRAME)
				{
					m_Scale.y = 1.0f;
					m_PlayerAnim.isAttack = false;
					m_PlayerAnim.AttackFrame = 0;
					m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;
					m_Param.dashLevel = 0.0f;
				}

				// �U���̏Փ˔���
				if (m_PlayerAnim.AttackFrame == 33 || m_PlayerAnim.AttackFrame == 37) // ��񓖂��蔻����s��
				{
					AttackCollisionDetection();
				}
			}

		}
		// �U�����Ă��Ȃ��ꍇ
		else
		{
			// �U���J�n
			// �v���X�U��
			if (Input::GetKeyTrigger('K') && m_Param.canAttack)
			{
				m_PlayerAnim.isAttack = true;
				m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
				m_PlayerAnim.PlayerAttack = PLAYER_PRESS_ATTACK;

				// �U���Ԋu���Z�b�g
				m_Param.AttackFrame = 0;
				m_Param.canAttack = false;

				//m_ShotSE->Play();

				// �X�e�[�^�X�̕ϓ��ɂ��J�E���g���Z�b�g
				PlayerUI::ResetStatusUICount();
			}
			// �_�b�V���U��
			if (Input::GetKeyTrigger('L') && m_Param.canAttack)
			{
				m_PlayerAnim.isAttack = true;
				m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
				m_PlayerAnim.PlayerAttack = PLAYER_DASH_ATTACK;

				// �U�������ƃv���C���[�����ݒ�
				if (m_Target) m_PlayerAnim.AttackRot = m_Rotation.y = oldRotation.y = m_TargetRot;
				else m_PlayerAnim.AttackRot = m_Rotation.y;

				// �U���Ԋu���Z�b�g
				m_Param.AttackFrame = 0;
				m_Param.canAttack = false;

				// �T�E���h
				//m_ShotSE->Play();

				// �X�e�[�^�X�̕ϓ��ɂ��J�E���g���Z�b�g
				PlayerUI::ResetStatusUICount();
			}

			// �U���Ԋu�t���[���X�V
			if (m_Param.AttackFrame < PLAYER_ATTACK_INTERVAL)m_Param.AttackFrame++;
			else m_Param.canAttack = true;

			// �W�����v����
			{
				// �W�����v���̏ꍇ
				if (m_PlayerAnim.isJump)
				{
					// 4�t���[���ڂŃW�����v����
					if (m_PlayerAnim.JumpFrame == 5)
					{
						m_Velocity.y = 0.35f;
						m_PlayerAnim.isGround = false;
					}
					// �W�����v�̃A�j���[�V����
					else
					{
						m_Position += DataStorage::GetPlayerDataStorage()->JumpAnimPos[m_PlayerAnim.JumpFrame];
						m_Scale = DataStorage::GetPlayerDataStorage()->JumpAnimScale[m_PlayerAnim.JumpFrame];
					}
					// �A�j���[�V�����t���[���X�V
					m_PlayerAnim.JumpFrame++;

					// �T�E���h
					//if (m_PlayerAnim.JumpFrame == 6) m_MoveSE->Play();

					// �I������
					if (m_PlayerAnim.JumpFrame >= PLAYER_MAX_JUMP_FRAME)
					{
						m_PlayerAnim.isJump = false;
						m_PlayerAnim.JumpFrame = 0;
						m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					}
				}
				// �W�����v���Ă��Ȃ��ꍇ(���n���Ă���ꍇ)
				else
				{
					// �W�����v�J�n
					if (Input::GetKeyTrigger(VK_SPACE) && m_PlayerAnim.isGround && !m_PlayerAnim.isJump)
					{
						m_PlayerAnim.isJump = true;
						m_PlayerAnim.JumpFrame = 0;       // �A�j���[�V�����t���[����������
					}
				}
			}


			// �ړ�����
			{
			// �J�����ƃv���C���[�̃x�N�g������p�x���v�Z
			D3DXVECTOR3 cameraDir = camera->GetPosition() - m_Position; // �J��������v���C���[�̕���
			float rot = atan2(cameraDir.x, cameraDir.z);                
			float cameraRot = -camera->GetRotation().y;                 // �J�����̉�]��

			D3DXVECTOR3 vecPower(0.0f, 0.0f, 0.0f);  // �i�ވړ���
			D3DXVECTOR3 vecForward(0.0f, rot, 0.0f); // �i�ޕ���
			if (Input::GetKeyPress('A'))
			{
				vecPower.x += cos(rot);
				vecPower.z -= sin(rot);
				vecForward.y += D3DX_PI * 1.5f;
				m_PlayerAnim.isWalk = true;
			}
			if (Input::GetKeyPress('D'))
			{
				vecPower.x -= cos(rot);
				vecPower.z += sin(rot);
				vecForward.y += D3DX_PI * 0.5f;
				m_PlayerAnim.isWalk = true;
			}
			if (Input::GetKeyPress('W'))
			{
				vecPower.x -= sin(rot);
				vecPower.z -= cos(rot);
				vecForward.y += D3DX_PI * 0.0f;
				m_PlayerAnim.isWalk = true;
			}
			if (Input::GetKeyPress('S'))
			{
				vecPower.x += sin(rot);
				vecPower.z += cos(rot);
				vecForward.y += D3DX_PI * 1.0f;
				m_PlayerAnim.isWalk = true;
			}
			D3DXVec3Normalize(&vecPower, &vecPower); // �ړ��ʂ̑傫���� 1.0 �ɂ���
			m_Velocity += vecPower / 5;              // �ړ��ʂ����Z

			// �΂߈ړ��̏ꍇ
			if (Input::GetKeyPress('A') || Input::GetKeyPress('D') || Input::GetKeyPress('W') || Input::GetKeyPress('S'))
			{
				// ���O���܂��͉E�O���̏ꍇ
				if (Input::GetKeyPress('A') && Input::GetKeyPress('W'))
				{
					vecForward.y = rot + D3DX_PI * 7 * 0.25f;
				}
				else if (Input::GetKeyPress('D') && Input::GetKeyPress('W'))
				{
					vecForward.y = rot + D3DX_PI * 1 * 0.25f;
				}
				// ������܂��͉E����̏ꍇ
				else if (Input::GetKeyPress('A') && Input::GetKeyPress('S'))
				{
					vecForward.y = rot + D3DX_PI * 5 * 0.25f;
				}
				else if (Input::GetKeyPress('D') && Input::GetKeyPress('S'))
				{
					vecForward.y = rot + D3DX_PI * 3 * 0.25f;
				}
			}

			// �������v�Z
			vecForward.y += D3DX_PI * (1.0f - cameraRot);
			// �����𔽉f
			if (!m_PlayerAnim.isWalk || (m_PlayerAnim.isAttack && m_PlayerAnim.PlayerAttack == PLAYER_DASH_ATTACK))
				m_Rotation = oldRotation;
			else m_Rotation = vecForward;


			// �ړ����̃A�j���[�V����
			if ((m_PlayerAnim.isWalk || m_PlayerAnim.AnimFrame > 0) && !m_PlayerAnim.isJump)
			{
				m_Position += DataStorage::GetPlayerDataStorage()->WalkAnimPos[m_PlayerAnim.AnimFrame];
				m_Scale = DataStorage::GetPlayerDataStorage()->WalkAnimScale[m_PlayerAnim.AnimFrame];
				// �A�j���[�V�����t���[���X�V
				m_PlayerAnim.AnimFrame++;
				if (m_PlayerAnim.AnimFrame >= PLAYER_MAX_WALK_FRAME) m_PlayerAnim.AnimFrame = 0;
			}
			}

			// �������
			{   // ��𒆂̏ꍇ
				if (m_PlayerAnim.isDodge)
				{
					// ����̃A�j���[�V����
					m_Position += m_DodgeVector * 4.0f;
					m_PlayerAnim.DodgeFrame++;

					// �T�E���h
					//if (m_PlayerAnim.DodgeFrame == 1) m_DodgeSE->Play();

					// �I������
					if (m_PlayerAnim.DodgeFrame >= PLAYER_MAX_DODGE_FRAME)
					{
						m_PlayerAnim.isDodge = false;
						m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					}
				}
				// ����J�n
				else if (Input::GetKeyTrigger('E') && scene->GetGameObject<PlayerUI>()->GetPlayerStatus().y >= 1.0f)
				{
					m_PlayerAnim.isDodge = true;
					m_PlayerAnim.DodgeFrame = 0;       // �A�j���[�V�����t���[����������
					scene->GetGameObject<PlayerUI>()->SetPlayerStatus(D3DXVECTOR4(0.0f, -1.0f, 1.0f, 0.0f));
					// ��������ݒ�
					m_Velocity.y = 0.0f;
					if (D3DXVec3Length(&m_Velocity) != 0) m_DodgeVector = m_Velocity;
					else m_DodgeVector = D3DXVECTOR3(sinf(m_Rotation.y), 0.0f, cosf(m_Rotation.y)) / 5; // �v���C���[�̈ړ��x�N�g����0�̏ꍇ�����Ă�������։��

					// �X�e�[�^�X�̕ϓ��ɂ��J�E���g���Z�b�g
					PlayerUI::ResetStatusUICount();
				}
			}

			// �^�[�Q�b�g�G�l�~�[����
			UpdateTargetEnemy();
		}
	}


	// �d��
	m_Velocity.y -= 0.015f;
	// �ړ�
	m_Position += m_Velocity;
	// ��Q���Ƃ̏Փ˔���
	float groundHeight;
	if (scene->GetSceneName() == WORFSPACE_SCENE) groundHeight = 0.0f;
	else groundHeight = -20.0f;

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
	// �G�l�~�[
	if (!m_PlayerAnim.isDodge) // ��𒆂͍l�����Ȃ�
	{
		std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
		for (GameObject* enemy : enemies)
		{
			D3DXVECTOR3 position = enemy->GetPosition();
			D3DXVECTOR3 scale = enemy->GetScale() + D3DXVECTOR3(0.0f, 0.5f, 0.0f); // ����ɏ��Ȃ��悤�ɕ␳���|����

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
	// �����ȕ�
	std::vector<InvisibleBox*> invisibleBoxs = scene->GetGameObjects<InvisibleBox>();
	for (InvisibleBox* invisibleBox : invisibleBoxs)
	{
		if (invisibleBox->GetInvisibleBoxParameter().Length <= 20.0f) // �v���C���[�Ƃ̋��������ȉ��̏ꍇ������s
		{
			D3DXVECTOR3 position = invisibleBox->GetPosition();
			D3DXVECTOR3 scale = invisibleBox->GetScale();

			if (position.x - scale.x - PLAYER_RADIUS < m_Position.x &&
				m_Position.x < position.x + scale.x + PLAYER_RADIUS &&
				position.z - scale.z - PLAYER_RADIUS < m_Position.z &&
				m_Position.z < position.z + scale.z + PLAYER_RADIUS)
			{
				if (m_Position.y < position.y + scale.y * 2.0f && m_Position.y >= position.y)
				{
					// �C�x���g���s��
					invisibleBox->RunningEvent();
				}
				if (invisibleBox->GetInvisibleBoxParameter().Collision)
				{
					if (m_Position.y < position.y + scale.y * 2.0f)
					{
						m_Position.x = oldPosition.x;
						m_Position.z = oldPosition.z;
					}
					else if (groundHeight < position.y + scale.y * 2.0f)
					{
						groundHeight = position.y + scale.y * 2.0f;
					}
				}
			}
		}
	}
	// �ڒn
	if (m_Position.y < groundHeight + PLAYER_RADIUS )
	{
		m_Position.y = groundHeight + PLAYER_RADIUS;
		m_Velocity.y = 0.0f;
		m_PlayerAnim.isGround = true;
	}
	// ������
	if (m_Position.y < -15.0f)
	{
		SetRespawn();
	}

	GameObject::Update();
}

void Player::Draw()
{
	if (Manager::GetScene()->GetSceneName() == TITLE_SCENE) return;

	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPlayerPS(), NULL, 0);

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

	PLAYERPARAMETER param;
	param.Material = GUI::playerMaterial;
	Renderer::SetPlayerParameter(param);


	Resource::GetPlayerModel()->Draw(material);
}


void Player::DrawShadowMapping()
{
	if (Manager::GetScene()->GetSceneName() == TITLE_SCENE) return;
	
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	Resource::GetPlayerModel()->Draw();
}

void Player::DrawZPrePass()
{
	if (Manager::GetScene()->GetSceneName() == TITLE_SCENE) return;

	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetPlayerModel()->Draw();
}

void Player::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �|���S���`��
	Resource::GetPlayerModel()->Draw();
}



void Player::UpdateTargetEnemy()
{
	Scene* scene = Manager::GetScene();

	if (m_Target)
	{
		// �^�[�Q�b�g�ɑ΂���x�N�g���Ɗp�x���v�Z
		D3DXVECTOR3 targetPos = m_Target->GetPosition();
		m_TargetVector = m_Position - targetPos;
		m_TargetRot = atan2f(targetPos.x - m_Position.x, targetPos.z - m_Position.z);

		// ���݂̃^�[�Q�b�g�Ƃ̋�������苗�����ꂽ�ꍇ�^�[�Q�b�g���O��
		if (D3DXVec3Length(&m_TargetVector) > 13.0f)
		{
			m_Target->SetEnemyIsTarget(false);
			m_Target = nullptr;
		}
	}

	// �^�[�Q�b�g�̐ݒ�
	else
	{
		Enemy* newTarget = nullptr; // �V�^�[�Q�b�g�ƂȂ�G�l�~�[
		float targetLength = 10.0f; // �G�l�~�[�Ƃ̋���

		std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
		for (GameObject* enemy : enemies)
		{
			D3DXVECTOR3 enemyPos = enemy->GetPosition();
			D3DXVECTOR3 direction = m_Position - enemyPos;
			float length = D3DXVec3Length(&direction);

			// ��苗�������ł��߂��G�l�~�[���^�[�Q�b�g���Ƃ��Ď擾
			if (length < 10.0f && length < targetLength)
			{
				newTarget = (Enemy*)enemy;
				targetLength = length;
			}
		}

		// �V�^�[�Q�b�g�ƂȂ�G�l�~�[�������ꍇ�^�[�Q�b�g��ݒ�
		if (newTarget)
		{
			m_Target = newTarget;
			m_Target->SetEnemyIsTarget(true);
		}
	}
}

void Player::AttackCollisionDetection()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	// �G�l�~�[�Ƃ̓����蔻��
	std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
	for (GameObject* enemy : enemies)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPos;
		float length = D3DXVec3Length(&direction);

		if (length < 4.5f) ((Enemy*)enemy)->SetDamage(2.0f);
	}

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	// ��
	std::vector<Rock*> rocks = scene->GetGameObjects<Rock>();
	for (Rock* rock : rocks)
	{
		D3DXVECTOR3 position = rock->GetPosition();
		D3DXVECTOR3 scale = rock->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x * 1.6f)
		{
			rock->SetDamage(2.0f);

			camera->SetCameraOscillationMode(CO_MEDIUM_MODE);
		}
	}
	// ��
	std::vector<Tree*> trees = scene->GetGameObjects<Tree>();
	for (Tree* tree : trees)
	{
		D3DXVECTOR3 position = tree->GetPosition();
		D3DXVECTOR3 scale = tree->GetScale();

		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < 3.5f)
		{
			tree->SetIsSway();
			camera->SetCameraOscillationMode(CO_SMALL_MODE);
		}
	}
}

// �v���C���[�ւ̃_���[�W�ʂ��Z�b�g
void Player::SetDamage(float damage)
{
	// ��_���[�W���̓_���[�W���������Ȃ�
	if (m_Param.isDamaged) return;
	// ��𒆂̓_���[�W���������Ȃ�
	if (m_PlayerAnim.isDodge) return;

	// �_���[�W����
	m_Param.HitPoint -= damage;
	m_Param.isDamaged = true;
	// �X�e�[�^�X�̕ϓ��ɂ��J�E���g���Z�b�g
	PlayerUI::ResetStatusUICount();
}

void Player::SetRespawn()
{
	// ����s�\
	m_CanControl = GUI::playerControllFlag = false;
	// ���S�e�L�X�g�C�x���g����
	Scene* scene = Manager::GetScene();
	TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
	textEvent->SetTextEvent(TEXTEVENT_DEATH);
	textEvent->SetEventStateReady();
	// ���S�p�[�e�B�N���G�t�F�N�g
	scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 2.0f, 0.0f), PARTICLE_PLAYERDEATH);
}

void Player::SetHealHP(float heal)
{
	m_Param.HitPoint += heal;

	// �X�e�[�^�X�̕ϓ��ɂ��J�E���g���Z�b�g
	PlayerUI::ResetStatusUICount();
}

void Player::Respawn()
{
	// ���W�E�X�e�[�^�X��������
	m_Position = m_RespwanPosition + D3DXVECTOR3(0.0f, 7.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
	m_Param.HitPoint = PLAYER_MAX_HP;

	// ����\
	m_CanControl = GUI::playerControllFlag = true;
}

