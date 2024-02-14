#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "resource.h"
#include "enemy.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "rock.h"
#include "ground.h"
#include "invisibleBox.h"
#include "enemyStateIdle.h"
#include "enemyStateDamaged.h"
#include "particle.h"
#include "random.h"


void Enemy::Init()
{
	m_EnemyState = new EnemyStateIdle(this);
	m_EnemyType = ENEMY_NORMAL;

	m_Position = D3DXVECTOR3(0.0f, 6.0f, 20.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_AttackRange = 6.0f;
	m_WalkRange = 30.0f;
}

void Enemy::Uninit()
{
	GameObject::Uninit();
}

void Enemy::Update()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 oldPosition = m_Position; // 前回の座標
	D3DXVECTOR3 oldRotation = m_Rotation; // 前回の向き
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// エネミーステート更新
	SetEnemyState();

	// 状況を見てエネミーの行動を決定する
	m_EnemyState->StateAction();


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
	// エネミー
	std::vector<GameObject*> enemies = scene->GetGameObjectWithTag(ENEMY);
	for (GameObject* enemy : enemies)
	{
		if (enemy->GetPosition() == m_Position && enemy->GetRotation() == m_Rotation) break; // 自身の時はスキップする

		D3DXVECTOR3 position = enemy->GetPosition();
		D3DXVECTOR3 scale = enemy->GetScale() + D3DXVECTOR3(1.0f, 0.0f, 1.0f); // 補正値を与える

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
	// プレイヤー
	Player* player = scene->GetGameObject<Player>();
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
	// 透明な壁
	std::vector<InvisibleBox*> invisibleBoxes = scene->GetGameObjects<InvisibleBox>();
	for (InvisibleBox* invisibleBox : invisibleBoxes)
	{
		// 衝突判定が有効で距離が一定以下の場合判定実行
		if (invisibleBox->GetInvisibleBoxParameter().Collision && invisibleBox->GetInvisibleBoxParameter().Length <= 20.0f)
		{
			D3DXVECTOR3 position = invisibleBox->GetPosition();
			D3DXVECTOR3 scale = invisibleBox->GetScale();

			if (position.x - scale.x - ENEMY_RADIUS < m_Position.x &&
				m_Position.x < position.x + scale.x + ENEMY_RADIUS &&
				position.z - scale.z - ENEMY_RADIUS < m_Position.z &&
				m_Position.z < position.z + scale.z + ENEMY_RADIUS)
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
	// 接地
	if (m_Position.y < groundHeight + ENEMY_RADIUS)
	{
		m_Position.y = groundHeight + ENEMY_RADIUS;
		m_Velocity.y = 0.0f;
	}

	// 死亡
	JudgeDeath();

	GameObject::Update();
}

void Enemy::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

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


	Resource::GetEnemyNormalModel()->Draw(material);
}


void Enemy::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	Resource::GetEnemyNormalModel()->Draw();
}

void Enemy::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetEnemyNormalModel()->Draw();
}

void Enemy::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetEnemyNormalModel()->Draw();
}





void Enemy::AttackCollisionDetection()
{
	Player* player = Manager::GetScene()->GetGameObject<Player>();

	D3DXVECTOR3 playerPos = player->GetPosition();
	D3DXVECTOR3 direction = (m_Position + m_AttackVector) - playerPos;
	float length = D3DXVec3Length(&direction);

	if (length <= PLAYER_RADIUS + ENEMY_RADIUS * 2.0f + ENEMY_ATTACK_ADDITIONAL_LENGTH)
	{
		// ダメージを与える
		player->SetDamage(1.0f);
		// ヒットバックベクトルを与える
		player->SetAttackedVector(-m_AttackVector);
	}
}



void Enemy::RotateToTarget(D3DXVECTOR3* targetRot)
{
	D3DXVECTOR3* mainRot = &m_Rotation;
	D3DXVECTOR3* vector = targetRot;
	vector->y = 0.0f;
	D3DXVec3Normalize(vector, vector);
	if (vector->z <= 0.0f && vector->x >= 0.0f) mainRot->y = acosf(vector->z);
	else if (vector->z < 0.0f && vector->x < 0.0f) mainRot->y = -acosf(vector->z);
	else mainRot->y = asinf(vector->x);

	float rot = mainRot->y * 180.0f / D3DX_PI;
	float subrot = vector->y * 180.0f / D3DX_PI;

	// 回転方向の正負を決める
	float length = rot - subrot;
	if (rot >= 0.0f)
	{
		if (abs(length) <= 180.0f && abs(length) > 0.0f)
		{
			if (abs(rot - subrot) <= 1.5f) rot = subrot;
			else rot -= 1.5f;
		}
		else
		{
			if (abs(rot - subrot) >= 1.5f) rot = subrot;
			else rot += 1.5f;
		}
	}
	else
	{
		if (abs(length) <= 180.0f && abs(length) > 0.0f)
		{
			if (abs(rot - subrot) <= 1.5f) rot = subrot;
			else rot += 1.5f;
		}
		else
		{
			if (abs(rot - subrot) >= 1.5f) rot = subrot;
			else rot -= 1.5f;
		}
	}

	mainRot->y = rot * D3DX_PI / 180.0f;
}

void Enemy::SetDamage(float damage)
{
	// 被ダメージ中はダメージ処理をしない
	if (m_EnemyState->GetStateName() == ENEMY_STATE_DAMAGED) return;

	// ダメージ処理
	m_HitPoint -= damage;
	SetNextEnemyState(new EnemyStateDamaged(this));
	// 攻撃ヒット時にカメラを揺らす
	Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
	camera->SetCameraOscillationMode(CO_LARGE_MODE);
}

void Enemy::JudgeDeath()
{
	// HPが0または落下した場合
	if (m_HitPoint <= 0 || m_Position.y < -15.0f)
	{
		Scene* scene = Manager::GetScene();
		// 死亡時の演出
		scene->AddGameObject<Particle>(1)->SetParticle(m_Position + D3DXVECTOR3(0.0f, 2.0f, 0.0f), PARTICLE_ENEMYDEATH);
		// 死亡処理
		SetDestroy();
	}
}

void Enemy::SetEnemyState()
{
	if (!m_NextEnemyState) return;

	m_EnemyState = m_NextEnemyState;
	m_NextEnemyState = nullptr;
}
