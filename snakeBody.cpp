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

	GameObject::Update();
}

void SnakeBody::Draw()
{
	// 被ターゲット時にアウトライン描画
	if (m_isTarget) DrawOutline();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
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
	Resource::GetSnakeBodyModel()->Draw(material);
}

void SnakeBody::DrawShadowMapping()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetSnakeBodyModel()->Draw();
}

void SnakeBody::DrawZPrePass()
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
	Resource::GetSnakeBodyModel()->Draw();
}

void SnakeBody::EnemyActionDecider()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXVECTOR3 vecThisToPrePos = m_PreSnakePosition - m_Position;     // 直前蛇オブジェクトに向かうベクトル
	float length = D3DXVec3Length(&vecThisToPrePos);

	// 蛇の先頭オブジェクトを取得
	SnakeHead* snakeHead = scene->GetGameObject<SnakeHead>();
	D3DXVECTOR3 vecThisToTacklePos = (m_PreSnakePosition * 1.0f + snakeHead->GetPosition() * 0.0f) - m_Position;

	// 攻撃を受けている場合
	if (m_Param.State == DAMAGED_STATE)
	{
		// フレーム更新
		m_Param.DamageFrame++;
		// 終了処理
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
		else SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos[m_Param.AttackFrame]);                                                    // 攻撃判定前
		m_Scale = DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimScale[m_Param.AttackFrame];
		m_Param.AttackFrame++;

		// 終了処理
		if (m_Param.AttackFrame >= SNAKE_MAX_ATTACK_FRAME)
		{
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			m_Param.State = WALK_STATE;
			m_Param.AttackFrame = 0;
			m_Param.WalkFrame = 0;
			m_Param.DamageFrame = 0;
		}

		// 攻撃判定前に攻撃方向ベクトルを得る
		if (m_Param.AttackFrame == 6) D3DXVec3Normalize(&m_AttackVector, &vecThisToTacklePos);
		// プレイヤーとの衝突判定
		if (m_Param.AttackFrame == 21) // プレイヤーが回避中でない場合に一フレームのみ当たり判定を行う
		{
			AttackCollisionDetection();
			Resource::GetEnemyAttackSEAudio()->Play();
		}

		// 胴体オブジェクトの状態も変更
		SnakeBody* nextSnakeBody = snakeHead->GetSnakeBody(m_Number + 1);
		if (m_Param.AttackFrame == 6 && nextSnakeBody && !nextSnakeBody->GetDestroy())
			nextSnakeBody->SetSnakeState(ATTACK_STATE);

		return;
	}


	// 攻撃していない場合
	{
		// 直前オブジェクトとの距離が一定以内の場合実行
		if (length >= 4.0f)
		{
			// 待機状態か攻撃準備状態でプレイヤーと距離が離れた時
			if (m_Param.State == IDLE_STATE)
				m_Param.State = WALK_STATE;
		}
		else // プレイヤーとの距離が離れているとき
		{
			m_Param.State = IDLE_STATE;
		}

		// 移動処理
		if ((m_Param.State == WALK_STATE) || m_Param.WalkFrame > 0)
		{
			D3DXVECTOR3 vector = vecThisToPrePos;
			float prePosLength = D3DXVec3Length(&vecThisToPrePos); // 直前オブジェクトとの距離
			float gapMoveSpeed = (prePosLength / 4.0f) / 100.0f;
			vector.y = 0.0f; // Y軸の移動量は0にする
			D3DXVec3Normalize(&vector, &vector);
			m_Velocity = (0.12f + snakeHead->GetMoveSpeed() + gapMoveSpeed) * vector;

			// 移動アニメーション
			SetPosition(m_Position + DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos[m_Param.WalkFrame] * 1.2f);
			m_Scale = DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale[m_Param.WalkFrame];
			m_Param.WalkFrame++;
			// アニメーションフレームカウントリセット
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

	// 胴体への攻撃はダメージがほとんど通らない
	m_HitPoint -= damage * 0.01f;
	// 胴体の状態を被ダメージ中に更新
	m_Param.State = DAMAGED_STATE;

	// 攻撃ヒット時にカメラを揺らす
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}

void SnakeBody::SetDamageFromHead(float damage)
{
	if (m_Param.DamageFrame != 0) return;

	// 頭部はダメージが通る
	m_HitPoint -= damage;
	// 胴体の状態を被ダメージ中に更新
	m_Param.State = DAMAGED_STATE;

	// 攻撃ヒット時にカメラを揺らす
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}
