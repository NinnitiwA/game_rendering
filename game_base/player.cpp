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


	// パラメータの初期化
	m_Param.HitPoint = PLAYER_MAX_HP;
	m_Param.AttackFrame = 0;
	m_Param.canAttack = false;
	m_Param.dashLevel = 0.0f;
	m_Param.isFindEnemy = false;
	m_Param.isDamaged = false;
	m_Param.DamageFrame = 0;

	// アニメーション情報初期化
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
	D3DXVECTOR3 oldPosition = m_Position; // 前回の座標
	D3DXVECTOR3 oldRotation = m_Rotation; // 前回の向き
	m_Velocity = D3DXVECTOR3(0.0f, m_Velocity.y, 0.0f);
	m_PlayerAnim.isWalk = false;
	m_CanControl = GUI::playerControllFlag;
	if (m_Param.HitPoint > PLAYER_MAX_HP) m_Param.HitPoint = PLAYER_MAX_HP; // 最大HPは超えない


	// プレイヤー操作不可能な場合
	if (!m_CanControl || scene->GetSceneName() == TITLE_SCENE)return;


	// 攻撃を受けている場合
	if (m_Param.isDamaged)
	{
		// 行動中止
		m_PlayerAnim.isAttack = false;
		m_PlayerAnim.AttackFrame = 0;
		m_PlayerAnim.isWalk = false;
		m_PlayerAnim.AnimFrame = 0;
		m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// ヒットストップ中
		if (m_Param.DamageFrame == 0)
		{
			// 攻撃ヒット時にカメラを揺らす
			camera->SetCameraOscillationMode(CO_LARGE_MODE);
		}
		// 終了処理
		else if (m_Param.DamageFrame >= PLAYER_MAX_DAMAGE_FRAME)
		{
			m_Param.DamageFrame = 0;
			m_Param.isDamaged = false;
		}
		m_Param.DamageFrame++;

		// ひるみ
		m_AttackedVector.y = 0.0f;
		D3DXVec3Normalize(&m_AttackedVector, &m_AttackedVector);
		m_Velocity = -m_AttackedVector * 0.2f * cosf(D3DX_PI * ((float)(m_Param.DamageFrame * 0.6) / (float)PLAYER_MAX_DAMAGE_FRAME));
	}
	// 攻撃を受けていない場合
	else
	{
		// 攻撃中の場合
		if (m_PlayerAnim.isAttack)
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
					m_Scale.y = 1.0f;
					m_PlayerAnim.isAttack = false;
					m_PlayerAnim.AttackFrame = 0;
					m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;
				}

				// 敵との衝突判定(一フレームのみ当たり判定を行う)
				if (m_PlayerAnim.AttackFrame == 3) AttackCollisionDetection();

				// サウンド
				//if (m_PlayerAnim.AttackFrame == 0) m_ChargeLightSE->Play();
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
					// ダッシュ攻撃ため時間をダッシュレベルに保存
					m_Param.dashLevel = m_PlayerAnim.AttackFrame;

					// サウンド
					//if (m_PlayerAnim.AttackFrame == 0)m_ChargeSE->Play();
				}
				// 攻撃開始
				if (Input::GetKeyExit('L'))
				{
					m_PlayerAnim.AttackFrame = 32;       // アニメーションフレームを攻撃直前のフレームに変更

					// サウンド
					//if (m_Param.dashLevel <= 20)m_ChargeLightSE->Play();
					//else m_ChargeHeavySE->Play();
				}
				// 攻撃中
				if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_PRE_FRAME)
				{
					// 攻撃アニメーション
					m_Position += D3DXVECTOR3(sinf(m_PlayerAnim.AttackRot), 0.0f, cosf(m_PlayerAnim.AttackRot)) // 向いている方向に対して座標を更新
						* DataStorage::GetPlayerDataStorage()->DashAttackAnimPos[m_PlayerAnim.AttackFrame].z
						* ((m_Param.dashLevel * 2.5) / PLAYER_MAX_DASH_ATTACK_PRE_FRAME);
					m_Scale = DataStorage::GetPlayerDataStorage()->DashAttackAnimScale[m_PlayerAnim.AttackFrame];

					// アニメーションフレーム更新
					m_PlayerAnim.AttackFrame++;
				}


				// 終了処理
				if (m_PlayerAnim.AttackFrame >= PLAYER_MAX_DASH_ATTACK_FRAME)
				{
					m_Scale.y = 1.0f;
					m_PlayerAnim.isAttack = false;
					m_PlayerAnim.AttackFrame = 0;
					m_PlayerAnim.PlayerAttack = PLAYER_NON_ATTACK;
					m_Param.dashLevel = 0.0f;
				}

				// 攻撃の衝突判定
				if (m_PlayerAnim.AttackFrame == 33 || m_PlayerAnim.AttackFrame == 37) // 二回当たり判定を行う
				{
					AttackCollisionDetection();
				}
			}

		}
		// 攻撃していない場合
		else
		{
			// 攻撃開始
			// プレス攻撃
			if (Input::GetKeyTrigger('K') && m_Param.canAttack)
			{
				m_PlayerAnim.isAttack = true;
				m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
				m_PlayerAnim.PlayerAttack = PLAYER_PRESS_ATTACK;

				// 攻撃間隔リセット
				m_Param.AttackFrame = 0;
				m_Param.canAttack = false;

				//m_ShotSE->Play();

				// ステータスの変動によりカウントリセット
				PlayerUI::ResetStatusUICount();
			}
			// ダッシュ攻撃
			if (Input::GetKeyTrigger('L') && m_Param.canAttack)
			{
				m_PlayerAnim.isAttack = true;
				m_PlayerAnim.AnimFrame = 0;       // アニメーションフレームを初期化
				m_PlayerAnim.PlayerAttack = PLAYER_DASH_ATTACK;

				// 攻撃方向とプレイヤー方向設定
				if (m_Target) m_PlayerAnim.AttackRot = m_Rotation.y = oldRotation.y = m_TargetRot;
				else m_PlayerAnim.AttackRot = m_Rotation.y;

				// 攻撃間隔リセット
				m_Param.AttackFrame = 0;
				m_Param.canAttack = false;

				// サウンド
				//m_ShotSE->Play();

				// ステータスの変動によりカウントリセット
				PlayerUI::ResetStatusUICount();
			}

			// 攻撃間隔フレーム更新
			if (m_Param.AttackFrame < PLAYER_ATTACK_INTERVAL)m_Param.AttackFrame++;
			else m_Param.canAttack = true;

			// ジャンプ処理
			{
				// ジャンプ中の場合
				if (m_PlayerAnim.isJump)
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

					// サウンド
					//if (m_PlayerAnim.JumpFrame == 6) m_MoveSE->Play();

					// 終了処理
					if (m_PlayerAnim.JumpFrame >= PLAYER_MAX_JUMP_FRAME)
					{
						m_PlayerAnim.isJump = false;
						m_PlayerAnim.JumpFrame = 0;
						m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					}
				}
				// ジャンプしていない場合(着地している場合)
				else
				{
					// ジャンプ開始
					if (Input::GetKeyTrigger(VK_SPACE) && m_PlayerAnim.isGround && !m_PlayerAnim.isJump)
					{
						m_PlayerAnim.isJump = true;
						m_PlayerAnim.JumpFrame = 0;       // アニメーションフレームを初期化
					}
				}
			}


			// 移動処理
			{
			// カメラとプレイヤーのベクトルから角度を計算
			D3DXVECTOR3 cameraDir = camera->GetPosition() - m_Position; // カメラからプレイヤーの方向
			float rot = atan2(cameraDir.x, cameraDir.z);                
			float cameraRot = -camera->GetRotation().y;                 // カメラの回転量

			D3DXVECTOR3 vecPower(0.0f, 0.0f, 0.0f);  // 進む移動量
			D3DXVECTOR3 vecForward(0.0f, rot, 0.0f); // 進む方向
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
			D3DXVec3Normalize(&vecPower, &vecPower); // 移動量の大きさを 1.0 にする
			m_Velocity += vecPower / 5;              // 移動量を加算

			// 斜め移動の場合
			if (Input::GetKeyPress('A') || Input::GetKeyPress('D') || Input::GetKeyPress('W') || Input::GetKeyPress('S'))
			{
				// 左前方または右前方の場合
				if (Input::GetKeyPress('A') && Input::GetKeyPress('W'))
				{
					vecForward.y = rot + D3DX_PI * 7 * 0.25f;
				}
				else if (Input::GetKeyPress('D') && Input::GetKeyPress('W'))
				{
					vecForward.y = rot + D3DX_PI * 1 * 0.25f;
				}
				// 左後方または右後方の場合
				else if (Input::GetKeyPress('A') && Input::GetKeyPress('S'))
				{
					vecForward.y = rot + D3DX_PI * 5 * 0.25f;
				}
				else if (Input::GetKeyPress('D') && Input::GetKeyPress('S'))
				{
					vecForward.y = rot + D3DX_PI * 3 * 0.25f;
				}
			}

			// 向きを計算
			vecForward.y += D3DX_PI * (1.0f - cameraRot);
			// 向きを反映
			if (!m_PlayerAnim.isWalk || (m_PlayerAnim.isAttack && m_PlayerAnim.PlayerAttack == PLAYER_DASH_ATTACK))
				m_Rotation = oldRotation;
			else m_Rotation = vecForward;


			// 移動中のアニメーション
			if ((m_PlayerAnim.isWalk || m_PlayerAnim.AnimFrame > 0) && !m_PlayerAnim.isJump)
			{
				m_Position += DataStorage::GetPlayerDataStorage()->WalkAnimPos[m_PlayerAnim.AnimFrame];
				m_Scale = DataStorage::GetPlayerDataStorage()->WalkAnimScale[m_PlayerAnim.AnimFrame];
				// アニメーションフレーム更新
				m_PlayerAnim.AnimFrame++;
				if (m_PlayerAnim.AnimFrame >= PLAYER_MAX_WALK_FRAME) m_PlayerAnim.AnimFrame = 0;
			}
			}

			// 回避処理
			{   // 回避中の場合
				if (m_PlayerAnim.isDodge)
				{
					// 回避のアニメーション
					m_Position += m_DodgeVector * 4.0f;
					m_PlayerAnim.DodgeFrame++;

					// サウンド
					//if (m_PlayerAnim.DodgeFrame == 1) m_DodgeSE->Play();

					// 終了処理
					if (m_PlayerAnim.DodgeFrame >= PLAYER_MAX_DODGE_FRAME)
					{
						m_PlayerAnim.isDodge = false;
						m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					}
				}
				// 回避開始
				else if (Input::GetKeyTrigger('E') && scene->GetGameObject<PlayerUI>()->GetPlayerStatus().y >= 1.0f)
				{
					m_PlayerAnim.isDodge = true;
					m_PlayerAnim.DodgeFrame = 0;       // アニメーションフレームを初期化
					scene->GetGameObject<PlayerUI>()->SetPlayerStatus(D3DXVECTOR4(0.0f, -1.0f, 1.0f, 0.0f));
					// 回避方向を設定
					m_Velocity.y = 0.0f;
					if (D3DXVec3Length(&m_Velocity) != 0) m_DodgeVector = m_Velocity;
					else m_DodgeVector = D3DXVECTOR3(sinf(m_Rotation.y), 0.0f, cosf(m_Rotation.y)) / 5; // プレイヤーの移動ベクトルが0の場合向いている方向へ回避

					// ステータスの変動によりカウントリセット
					PlayerUI::ResetStatusUICount();
				}
			}

			// ターゲットエネミー処理
			UpdateTargetEnemy();
		}
	}


	// 重力
	m_Velocity.y -= 0.015f;
	// 移動
	m_Position += m_Velocity;
	// 障害物との衝突判定
	float groundHeight;
	if (scene->GetSceneName() == WORFSPACE_SCENE) groundHeight = 0.0f;
	else groundHeight = -20.0f;

	// 地面
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
	// エネミー
	if (!m_PlayerAnim.isDodge) // 回避中は考慮しない
	{
		std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
		for (GameObject* enemy : enemies)
		{
			D3DXVECTOR3 position = enemy->GetPosition();
			D3DXVECTOR3 scale = enemy->GetScale() + D3DXVECTOR3(0.0f, 0.5f, 0.0f); // 頭上に乗らないように補正を掛ける

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
	// 岩
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
	// 透明な壁
	std::vector<InvisibleBox*> invisibleBoxs = scene->GetGameObjects<InvisibleBox>();
	for (InvisibleBox* invisibleBox : invisibleBoxs)
	{
		if (invisibleBox->GetInvisibleBoxParameter().Length <= 20.0f) // プレイヤーとの距離が一定以下の場合判定実行
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
					// イベント実行中
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
	// 接地
	if (m_Position.y < groundHeight + PLAYER_RADIUS )
	{
		m_Position.y = groundHeight + PLAYER_RADIUS;
		m_Velocity.y = 0.0f;
		m_PlayerAnim.isGround = true;
	}
	// 落下死
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

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// テクスチャ設定
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// マテリアル設定
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

	// マトリクス設定
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

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetPlayerModel()->Draw();
}

void Player::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetPlayerModel()->Draw();
}



void Player::UpdateTargetEnemy()
{
	Scene* scene = Manager::GetScene();

	if (m_Target)
	{
		// ターゲットに対するベクトルと角度を計算
		D3DXVECTOR3 targetPos = m_Target->GetPosition();
		m_TargetVector = m_Position - targetPos;
		m_TargetRot = atan2f(targetPos.x - m_Position.x, targetPos.z - m_Position.z);

		// 現在のターゲットとの距離が一定距離離れた場合ターゲットを外す
		if (D3DXVec3Length(&m_TargetVector) > 13.0f)
		{
			m_Target->SetEnemyIsTarget(false);
			m_Target = nullptr;
		}
	}

	// ターゲットの設定
	else
	{
		Enemy* newTarget = nullptr; // 新ターゲットとなるエネミー
		float targetLength = 10.0f; // エネミーとの距離

		std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
		for (GameObject* enemy : enemies)
		{
			D3DXVECTOR3 enemyPos = enemy->GetPosition();
			D3DXVECTOR3 direction = m_Position - enemyPos;
			float length = D3DXVec3Length(&direction);

			// 一定距離内かつ最も近いエネミーをターゲット候補として取得
			if (length < 10.0f && length < targetLength)
			{
				newTarget = (Enemy*)enemy;
				targetLength = length;
			}
		}

		// 新ターゲットとなるエネミーがいた場合ターゲットを設定
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

	// エネミーとの当たり判定
	std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
	for (GameObject* enemy : enemies)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPos;
		float length = D3DXVec3Length(&direction);

		if (length < 4.5f) ((Enemy*)enemy)->SetDamage(2.0f);
	}

	// 環境オブジェクトとの当たり判定
	// 岩
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
	// 木
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

// プレイヤーへのダメージ量をセット
void Player::SetDamage(float damage)
{
	// 被ダメージ中はダメージ処理をしない
	if (m_Param.isDamaged) return;
	// 回避中はダメージ処理をしない
	if (m_PlayerAnim.isDodge) return;

	// ダメージ処理
	m_Param.HitPoint -= damage;
	m_Param.isDamaged = true;
	// ステータスの変動によりカウントリセット
	PlayerUI::ResetStatusUICount();
}

void Player::SetRespawn()
{
	// 操作不可能
	m_CanControl = GUI::playerControllFlag = false;
	// 死亡テキストイベント生成
	Scene* scene = Manager::GetScene();
	TextEvent* textEvent = scene->AddGameObject<TextEvent>(3);
	textEvent->SetTextEvent(TEXTEVENT_DEATH);
	textEvent->SetEventStateReady();
	// 死亡パーティクルエフェクト
	scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 2.0f, 0.0f), PARTICLE_PLAYERDEATH);
}

void Player::SetHealHP(float heal)
{
	m_Param.HitPoint += heal;

	// ステータスの変動によりカウントリセット
	PlayerUI::ResetStatusUICount();
}

void Player::Respawn()
{
	// 座標・ステータスを初期化
	m_Position = m_RespwanPosition + D3DXVECTOR3(0.0f, 7.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
	m_Param.HitPoint = PLAYER_MAX_HP;

	// 操作可能
	m_CanControl = GUI::playerControllFlag = true;
}

