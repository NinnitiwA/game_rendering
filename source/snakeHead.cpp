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
	// SnakeHead固有パラメータの初期化
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
	D3DXVECTOR3 oldPosition = m_Position; // 前回の座標
	D3DXVECTOR3 oldRotation = m_Rotation; // 前回の向き
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 状況を見てエネミーの行動を決定する
	EnemyActionDecider();

	// 重力
	m_Velocity.y -= 0.15f;
	// 移動
	m_Position += m_Velocity;
	// 障害物との衝突判定
	float groundHeight = -20.0f;
	// 地面
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
	// デバッグ用地面
	std::vector<Field*> fields = scene->GetGameObjects<Field>();
	for (Field* field : fields)
	{
		groundHeight = 0.0f;
	}
	// プレイヤー
	D3DXVECTOR3 position = player->GetPosition();
	D3DXVECTOR3 scale = player->GetScale() + D3DXVECTOR3(0.5f, 0.5f, 0.5f); // 頭上に乗らないように補正を掛ける
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
	// 接地
	if (m_Position.y < groundHeight + SNAKE_RADIUS)
	{
		m_Position.y = groundHeight + SNAKE_RADIUS;
		m_Velocity.y = 0.0f;
	}

	// 死亡
	JudgeDeath();

	// 生存蛇オブジェクトの数
	int aliveSnakeNum = 0;

	// 胴体オブジェクトの更新処理
	for (int i = 0; i <= SNAKE_BODY_NUM - 1; i++)
	{
		// 直前蛇オブジェクトの座標を格納
		if (i == 0) m_SnakeBodyList[i]->SetPreSnakePosition(m_Position);
		else m_SnakeBodyList[i]->SetPreSnakePosition(m_SnakeBodyList[i - 1]->GetPosition());
		// 順番制御
		m_SnakeBodyList[i]->SetSnakeBodyNumber(i);

		// 生存数カウント
		if (!m_SnakeBodyList[i]->GetDestroy()) aliveSnakeNum++;
	}

	// 移動速度更新
	m_MoveSpeed = 0.04f * (SNAKE_BODY_NUM / (aliveSnakeNum + 1));


	GameObject::Update();
}

void SnakeHead::Draw()
{
	// 被ターゲット時にアウトライン描画
	if (m_isTarget) DrawOutline();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(),  NULL, 0);
	if (m_Param.State == DAMAGED_STATE) Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);
	else Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

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
	material.Roughness = 0.5f;
	material.Metalic = 0.12f;
	material.TextureEnable = true;

	// カラー設定
	POSTPROCESSPARAMETER param{};
	param.color = D3DXVECTOR4(1.5f, 1.0f, 1.0f, 0.5f);
	Renderer::SetPostProcessParameter(param);

	// ポリゴン描写
	Resource::GetSnakeHeadModel()->Draw(material);
}

void SnakeHead::DrawShadowMapping()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetSnakeHeadModel()->Draw();
}

void SnakeHead::DrawZPrePass()
{
	// 被ターゲット時にアウトライン描画
	if (m_isTarget) DrawOutline();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	
	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetSnakeHeadModel()->Draw();
}


void SnakeHead::CreateSnakeBody(void)
{
	// 胴体オブジェクト設定
	for (int i = 0; i <= SNAKE_BODY_NUM; i++)
	{
		// 胴体オブジェクトを生成
		Scene* scene = Manager::GetScene();
		SnakeBody* snakeBody = scene->AddGameObject<SnakeBody>(1);
		snakeBody->SetPosition(m_Position + D3DXVECTOR3(5.0f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM), 1.5f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM), 5.0f * ((float)i + 1.0f / (float)SNAKE_BODY_NUM)));
		// 配列に格納
		m_SnakeBodyList[i] = snakeBody;
	}

	// 最後尾は空のオブジェクトにするため実体は6体
	m_SnakeBodyList[SNAKE_BODY_LAST_ALLAY_NUM + 1]->SetDestroy();
}

void SnakeHead::EnemyActionDecider()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 vecEnemyToPlayer = player->GetPosition() - m_Position; // エネミーからプレイヤーに向かうベクトル
	float length = D3DXVec3Length(&vecEnemyToPlayer);

	// 攻撃を受けている場合
	if (m_Param.State == DAMAGED_STATE)
	{
		// ヒットバック処理
		D3DXVECTOR3 vector = D3DXVECTOR3(vecEnemyToPlayer.x, 0.0f, vecEnemyToPlayer.z); // ヒットバック方向
		D3DXVec3Normalize(&vector, &vector);
		m_Velocity = -vector * 0.25f + D3DXVECTOR3(0.0f, 0.7f * cosf(D3DX_PI * ((float)m_Param.DamageFrame * 2 / (float)ENEMY_MAX_DAMAGE_FRAME)), 0.0f);

		// 終了処理
		m_Param.DamageFrame++;
		if (m_Param.DamageFrame >= SNAKE_MAX_DAMAGE_FRAME)
		{
			m_Param.DamageFrame = 0;
			m_Param.State = IDLE_STATE;
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
		return;
	}

	// 攻撃中の場合
	if (m_Param.State == ATTACK_STATE)
	{
		// 攻撃アニメーション
		if (m_Param.AttackFrame >= 20) SetPosition(m_Position - DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame].z * m_AttackVector); // 攻撃判定後プレイヤーへ向かって移動する
		else SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame]);                           // 攻撃判定前
		m_Scale = DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimScale[m_Param.AttackFrame];
		m_Param.AttackFrame++;

		// 終了処理
		if (m_Param.AttackFrame >= SNAKE_MAX_ATTACK_FRAME)
		{
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			if (length < 6.0f) m_Param.State = READY_STATE;
			else m_Param.State = WALK_STATE;
			m_Param.AttackFrame = 0;
			m_Param.WalkFrame = 0;
		}

		// 攻撃判定前に攻撃方向を設定
		if (m_Param.AttackFrame == 6) D3DXVec3Normalize(&m_AttackVector, &vecEnemyToPlayer);
		// プレイヤーとの衝突判定（特定の１フレームで当たり判定を行う）
		if (m_Param.AttackFrame == 20)
		{
			AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// 胴体オブジェクトの状態も変更
		if (m_Param.AttackFrame == 6 && m_SnakeBodyList[0] && !m_SnakeBodyList[0]->GetDestroy())
			m_SnakeBodyList[0]->SetSnakeState(ATTACK_STATE);

		return;
	}

	// 攻撃していない場合
	{
		// 攻撃準備
		if (m_Param.State == READY_STATE)
		{
			m_Param.ReadyFrame++;

			// 攻撃開始
			if (m_Param.ReadyFrame >= 45)
			{
				m_Param.State = ATTACK_STATE;
				m_Param.AttackFrame = 0;
				m_Param.ReadyFrame = 0;
			}
		}
		// プレイヤーとの距離が一定以内の場合攻撃準備状態へ
		else if (length < 5.0f) m_Param.State = READY_STATE;

		// プレイヤーとの距離が一定以内の場合実行
		if (length < 30.0f)
		{
			// プレイヤーの方向を向く
			RotateToTarget(&vecEnemyToPlayer);
			// 範囲内にプレイヤーが入っている場合
			if (length < 30.0f)
			{
				// 待機状態か攻撃準備状態でプレイヤーと距離が離れた時
				if (m_Param.State == IDLE_STATE || length >= 20.0f)
					m_Param.State = WALK_STATE;
			}
		}
		else // プレイヤーとの距離が離れているとき
		{
			m_Param.State = IDLE_STATE;
		}

		// 移動中
		if ((m_Param.State == WALK_STATE) || m_Param.WalkFrame > 0)
		{
			D3DXVECTOR3 vector = vecEnemyToPlayer;
			vector.y = 0.0f; // Y軸の移動量は考慮しない
			D3DXVec3Normalize(&vector, &vector);
			m_Velocity = (0.12 + m_MoveSpeed) * vector;

			// 移動アニメーション
			SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_Param.WalkFrame] * 1.2f);
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_Param.WalkFrame];
			m_Param.WalkFrame++;
			// アニメーションフレームカウントリセット
			if (m_Param.WalkFrame >= ENEMY_MAX_WALK_FRAME) m_Param.WalkFrame = 0;
		}
	}
}

void SnakeHead::SetDamage(float damage)
{
	// 頭部の状態を被ダメージ中に更新
	m_Param.State = DAMAGED_STATE;

	// 生存中の最後尾胴体オブジェクトを探す
	int aliveLastNumber = GetAliveSnakeBodyLastArrayNumber();

	// 生存している中で最後尾のオブジェクトでダメージ処理
	if (aliveLastNumber < 0)
	{
		m_HitPoint -= damage;
		// 攻撃ヒット時にカメラを揺らす
		Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
		camera->SetCameraOscillationMode(CO_LARGE_MODE);
	}
	else m_SnakeBodyList[aliveLastNumber]->SetDamageFromHead(damage);
}

void SnakeHead::JudgeDeath()
{
	// HPが0または落下した場合
	if (m_HitPoint <= 0 || m_Position.y < -15.0f)
	{
		Scene* scene = Manager::GetScene();
		// 死亡時の演出
		scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 2.0f, 0.0f), PARTICLE_ENEMYDEATH);
		// 死亡処理
		SetDestroy();

		// シーンイベント生成
		// リザルトシーンに推移
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
