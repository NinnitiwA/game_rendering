#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "resource.h"
#include "player.h"
#include "animationObject.h"
#include "camera.h"
#include "gui.h"
#include "dataStorage.h"

void AnimationObject::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_isAnimation = false;
	m_EndAnimation = false;
	m_EndFrame = 0;
}

void AnimationObject::Uninit()
{
}


void AnimationObject::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 oldPosition = m_Position; // �O��̍��W
	D3DXVECTOR3 oldRotation = m_Rotation; // �O��̌���
	m_Velocity = D3DXVECTOR3(0.0f, m_Velocity.y, 0.0f);



	// �A�j���[�V�����I������
	if (m_EndAnimation)
	{
		if (m_EndFrame >= ANIM_END_FRAME && m_PlayerAnim.isGround) ResetAnimation();
		m_EndFrame++;
	}


	// �U�����̏ꍇ
	if (m_isAnimation && !m_EndAnimation)
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
				m_EndAnimation = true;
				GUI::playerAnimation.PressAttack = false;
			}

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
			}
			// �U���J�n
			if (m_PlayerAnim.AttackFrame == PLAYER_MAX_DASH_ATTACK_PRE_FRAME - 1)
			{
				m_PlayerAnim.AttackFrame = 32;       // �A�j���[�V�����t���[�����U�����O�̃t���[���ɕύX

			}
			// �U����
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_PRE_FRAME)
			{
				// �U���A�j���[�V����
				m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // �����Ă�������ɑ΂��č��W���X�V
					* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z
					* ((m_PlayerAnim.AttackFrame * 2.5) / PLAYER_MAX_DASH_ATTACK_PRE_FRAME);
				m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

				// �A�j���[�V�����t���[���X�V
				m_PlayerAnim.AttackFrame++;
			}
			// �I������
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.DashAttack = false;
			}
		}
		// ���@�r��
		else if (m_PlayerAnim.PlayerAttack == PLAYER_MAGIC_CHANT)
		{
			// �U���A�j���[�V����
			m_Position += DataStorage::GetPlayerDataStorage()->MagicAnimPos[m_PlayerAnim.AttackFrame];
			m_Scale = DataStorage::GetPlayerDataStorage()->MagicAnimScale[m_PlayerAnim.AttackFrame];

			// �A�j���[�V�����t���[���X�V
			m_PlayerAnim.AttackFrame++;
			// �I������
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_MAGIC_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.MagicChant = false;
			}

		}
		// ���@�U��
		else if (m_PlayerAnim.PlayerAttack == PLAYER_MAGIC_ATTACK)
		{
			D3DXVECTOR3 vecAttack(0.0f, 1.0f, -10.0f);
			D3DXVec3Normalize(&vecAttack, &vecAttack);

			// �U���A�j���[�V����
			m_Position += DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos[m_PlayerAnim.AttackFrame].z * vecAttack;
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale[m_PlayerAnim.AttackFrame];

			// �A�j���[�V�����t���[���X�V
			m_PlayerAnim.AttackFrame++;
			// �I������
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAGIC_MAX_ATTACK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.MagicAttack = false;
			}
		}
		// �W�����v��
		else if (GUI::playerAnimation.Jump)
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

			// �I������
			if (m_PlayerAnim.JumpFrame >= PLAYER_MAX_JUMP_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.Jump = false;
			}
		}
		// �ړ����̃A�j���[�V����
		else if ((m_PlayerAnim.isWalk || m_PlayerAnim.AnimFrame > 0) && !m_PlayerAnim.isJump && GUI::playerAnimation.Walk)
		{
			m_Position += DataStorage::GetPlayerDataStorage()->WalkAnimPos[m_PlayerAnim.AnimFrame];
			m_Scale = DataStorage::GetPlayerDataStorage()->WalkAnimScale[m_PlayerAnim.AnimFrame];
			// �A�j���[�V�����t���[���X�V
			m_PlayerAnim.AnimFrame++;

			// �I������
			if (m_PlayerAnim.AnimFrame >= PLAYER_MAX_WALK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.Walk = false;
			}
		}
	}
	// �U�����Ă��Ȃ��ꍇ
	else
	{
		// �U���J�n
		// �v���X�U��
		if (GUI::playerAnimation.PressAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
			m_PlayerAnim.PlayerAttack = PLAYER_PRESS_ATTACK;

			// �A�j���[�V������
			m_isAnimation = true;
		}
		// �_�b�V���U��
		else if (GUI::playerAnimation.DashAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
			m_PlayerAnim.PlayerAttack = PLAYER_DASH_ATTACK;

			// �U�������ƃv���C���[�����ݒ�
			if (m_Target) m_PlayerAnim.AttackRot = m_Rotation.y = oldRotation.y = m_TargetRot;
			else m_PlayerAnim.AttackRot = m_Rotation.y;

			// �A�j���[�V������
			m_isAnimation = true;
		}
		// ���@�r��
		else if (GUI::playerAnimation.MagicChant)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
			m_PlayerAnim.PlayerAttack = PLAYER_MAGIC_CHANT;

			// �A�j���[�V������
			m_isAnimation = true;
		}
		// ���@�U��
		else if (GUI::playerAnimation.MagicAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // �A�j���[�V�����t���[����������
			m_PlayerAnim.PlayerAttack = PLAYER_MAGIC_ATTACK;

			// �A�j���[�V������
			m_isAnimation = true;
		}

		// �W�����v�J�n
		else if (GUI::playerAnimation.Jump && m_PlayerAnim.isGround && !m_PlayerAnim.isJump)
		{
			m_PlayerAnim.isJump = true;
			m_PlayerAnim.JumpFrame = 0;       // �A�j���[�V�����t���[����������

			// �A�j���[�V������
			m_isAnimation = true;
		}

		// �ړ��J�n
		else if (GUI::playerAnimation.Walk && !m_PlayerAnim.isJump)
		{
			m_PlayerAnim.isWalk = true;

			// �A�j���[�V������
			m_isAnimation = true;
		}

	}


	// �d��
	m_Velocity.y -= 0.015f;
	// �ړ�
	m_Position += m_Velocity;
	// ��Q���Ƃ̏Փ˔���
	float groundHeight = .0f;

	// �ڒn
	if (m_Position.y < groundHeight + PLAYER_RADIUS)
	{
		m_Position.y = groundHeight + PLAYER_RADIUS;
		m_Velocity.y = 0.0f;
		m_PlayerAnim.isGround = true;
	}

	GameObject::Update();
}

void AnimationObject::DrawDebug()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

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

void AnimationObject::ResetAnimation()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �U��
	m_PlayerAnim.isAttack = false;
	m_PlayerAnim.AttackFrame = 0;
	m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;

	// �W�����v
	m_PlayerAnim.isJump = false;
	m_PlayerAnim.JumpFrame = 0;

	// �ړ�
	m_PlayerAnim.AnimFrame = 0;

	m_isAnimation = false;
	m_EndAnimation = false;
	m_EndFrame = 0;
}


