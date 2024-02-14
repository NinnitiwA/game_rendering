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
	D3DXVECTOR3 oldPosition = m_Position; // 前回の座標
	D3DXVECTOR3 oldRotation = m_Rotation; // 前回の向き
	m_Velocity = D3DXVECTOR3(0.0f, m_Velocity.y, 0.0f);



	// アニメーション終了処理
	if (m_EndAnimation)
	{
		if (m_EndFrame >= ANIM_END_FRAME && m_PlayerAnim.isGround) ResetAnimation();
		m_EndFrame++;
	}


	// 攻撃中の場合
	if (m_isAnimation && !m_EndAnimation)
	{
		// プレス攻撃
		if (m_PlayerAnim.PlayerAttack == PLAYER_PRESS_ATTACK)
		{
			// 攻撃アニメーション
			m_Position += DataStorage::GetPlayerDataStorage()->PressAttackAnimPos[m_PlayerAnim.AttackFrame];
			m_Scale = DataStorage::GetPlayerDataStorage()->PressAttackAnimScale[m_PlayerAnim.AttackFrame];

			// アニメーションフレーム更新
			m_PlayerAnim.AttackFrame++;
			// 終了処理
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_PRESS_ATTACK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.PressAttack = false;
			}

		}
		// ダッシュ攻撃
		else if (m_PlayerAnim.PlayerAttack == PLAYER_DASH_ATTACK)
		{
			// 攻撃準備
			if (m_PlayerAnim.AttackFrame < PLAYER_MAX_DASH_ATTACK_PRE_FRAME - 1)
			{
				// 攻撃アニメーション
				m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // 向いている方向に対して座標を更新
					* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z;
				m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

				// アニメーションフレーム更新
				m_PlayerAnim.AttackFrame++;
			}
			// 攻撃開始
			if (m_PlayerAnim.AttackFrame == PLAYER_MAX_DASH_ATTACK_PRE_FRAME - 1)
			{
				m_PlayerAnim.AttackFrame = 32;       // アニメーションフレームを攻撃直前のフレームに変更

			}
			// 攻撃中
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_PRE_FRAME)
			{
				// 攻撃アニメーション
				m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // 向いている方向に対して座標を更新
					* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z
					* ((m_PlayerAnim.AttackFrame * 2.5) / PLAYER_MAX_DASH_ATTACK_PRE_FRAME);
				m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

				// アニメーションフレーム更新
				m_PlayerAnim.AttackFrame++;
			}
			// 終了処理
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.DashAttack = false;
			}
		}
		// 魔法詠唱
		else if (m_PlayerAnim.PlayerAttack == PLAYER_MAGIC_CHANT)
		{
			// 攻撃アニメーション
			m_Position += DataStorage::GetPlayerDataStorage()->MagicAnimPos[m_PlayerAnim.AttackFrame];
			m_Scale = DataStorage::GetPlayerDataStorage()->MagicAnimScale[m_PlayerAnim.AttackFrame];

			// アニメーションフレーム更新
			m_PlayerAnim.AttackFrame++;
			// 終了処理
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_MAGIC_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.MagicChant = false;
			}

		}
		// 魔法攻撃
		else if (m_PlayerAnim.PlayerAttack == PLAYER_MAGIC_ATTACK)
		{
			D3DXVECTOR3 vecAttack(0.0f, 1.0f, -10.0f);
			D3DXVec3Normalize(&vecAttack, &vecAttack);

			// 攻撃アニメーション
			m_Position += DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos[m_PlayerAnim.AttackFrame].z * vecAttack;
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale[m_PlayerAnim.AttackFrame];

			// アニメーションフレーム更新
			m_PlayerAnim.AttackFrame++;
			// 終了処理
			if (m_PlayerAnim.AttackFrame >= PLAYER_MAGIC_MAX_ATTACK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.MagicAttack = false;
			}
		}
		// ジャンプ中
		else if (GUI::playerAnimation.Jump)
		{
		    // 4フレーム目でジャンプする
			if (m_PlayerAnim.JumpFrame == 5)
			{
				m_Velocity.y = 0.35f;
				m_PlayerAnim.isGround = false;
			}
			// ジャンプのアニメーション
			else
			{
				m_Position += DataStorage::GetPlayerDataStorage()->JumpAnimPos[m_PlayerAnim.JumpFrame];
				m_Scale = DataStorage::GetPlayerDataStorage()->JumpAnimScale[m_PlayerAnim.JumpFrame];
			}
			// アニメーションフレーム更新
			m_PlayerAnim.JumpFrame++;

			// 終了処理
			if (m_PlayerAnim.JumpFrame >= PLAYER_MAX_JUMP_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.Jump = false;
			}
		}
		// 移動中のアニメーション
		else if ((m_PlayerAnim.isWalk || m_PlayerAnim.AnimFrame > 0) && !m_PlayerAnim.isJump && GUI::playerAnimation.Walk)
		{
			m_Position += DataStorage::GetPlayerDataStorage()->WalkAnimPos[m_PlayerAnim.AnimFrame];
			m_Scale = DataStorage::GetPlayerDataStorage()->WalkAnimScale[m_PlayerAnim.AnimFrame];
			// アニメーションフレーム更新
			m_PlayerAnim.AnimFrame++;

			// 終了処理
			if (m_PlayerAnim.AnimFrame >= PLAYER_MAX_WALK_FRAME)
			{
				m_EndAnimation = true;
				GUI::playerAnimation.Walk = false;
			}
		}
	}
	// 攻撃していない場合
	else
	{
		// 攻撃開始
		// プレス攻撃
		if (GUI::playerAnimation.PressAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
			m_PlayerAnim.PlayerAttack = PLAYER_PRESS_ATTACK;

			// アニメーション中
			m_isAnimation = true;
		}
		// ダッシュ攻撃
		else if (GUI::playerAnimation.DashAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
			m_PlayerAnim.PlayerAttack = PLAYER_DASH_ATTACK;

			// 攻撃方向とプレイヤー方向設定
			if (m_Target) m_PlayerAnim.AttackRot = m_Rotation.y = oldRotation.y = m_TargetRot;
			else m_PlayerAnim.AttackRot = m_Rotation.y;

			// アニメーション中
			m_isAnimation = true;
		}
		// 魔法詠唱
		else if (GUI::playerAnimation.MagicChant)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
			m_PlayerAnim.PlayerAttack = PLAYER_MAGIC_CHANT;

			// アニメーション中
			m_isAnimation = true;
		}
		// 魔法攻撃
		else if (GUI::playerAnimation.MagicAttack)
		{
			m_PlayerAnim.isAttack = true;
			m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
			m_PlayerAnim.PlayerAttack = PLAYER_MAGIC_ATTACK;

			// アニメーション中
			m_isAnimation = true;
		}

		// ジャンプ開始
		else if (GUI::playerAnimation.Jump && m_PlayerAnim.isGround && !m_PlayerAnim.isJump)
		{
			m_PlayerAnim.isJump = true;
			m_PlayerAnim.JumpFrame = 0;       // アニメーションフレームを初期化

			// アニメーション中
			m_isAnimation = true;
		}

		// 移動開始
		else if (GUI::playerAnimation.Walk && !m_PlayerAnim.isJump)
		{
			m_PlayerAnim.isWalk = true;

			// アニメーション中
			m_isAnimation = true;
		}

	}


	// 重力
	m_Velocity.y -= 0.015f;
	// 移動
	m_Position += m_Velocity;
	// 障害物との衝突判定
	float groundHeight = .0f;

	// 接地
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

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetRedGradationTexture());

	// マテリアル設定
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

	// 攻撃
	m_PlayerAnim.isAttack = false;
	m_PlayerAnim.AttackFrame = 0;
	m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;

	// ジャンプ
	m_PlayerAnim.isJump = false;
	m_PlayerAnim.JumpFrame = 0;

	// 移動
	m_PlayerAnim.AnimFrame = 0;

	m_isAnimation = false;
	m_EndAnimation = false;
	m_EndFrame = 0;
}


