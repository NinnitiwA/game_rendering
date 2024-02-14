#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "resource.h"
#include "enemyMagic.h"
#include "player.h"
#include "camera.h"
#include "field.h"
#include "rock.h"
#include "ground.h"
#include "enemyStateIdle.h"
#include "enemyStateDamaged.h"
#include "particle.h"
#include "random.h"


void EnemyMagic::Init()
{
	m_EnemyState = new EnemyStateIdle(this);
	m_EnemyType = ENEMY_MAGIC;

	m_Position = D3DXVECTOR3(0.0f, 6.0f, 20.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_AttackRange = 20.0f;
	m_WalkRange = 40.0f;
}

void EnemyMagic::Uninit()
{
	GameObject::Uninit();
}

void EnemyMagic::Update()
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
	float groundHeight = 0.0f;
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

void EnemyMagic::Draw()
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

	Resource::GetEnemyMagicModel()->Draw(material);
}


void EnemyMagic::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetShadowMappingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetShadowMappingPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	Resource::GetEnemyMagicModel()->Draw();
}

void EnemyMagic::DrawZPrePass()
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

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Resource::GetEnemyMagicModel()->Draw();
}

void EnemyMagic::DrawReflection()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描画
	Resource::GetEnemyMagicModel()->Draw();
}

