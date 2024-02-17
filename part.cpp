#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "manager.h"
#include "scene.h"
#include "random.h"
#include "camera.h"
#include "player.h"
#include "part.h"
#include "field.h"
#include "ground.h"
#include "invisibleBox.h"


Audio* Part::m_HealSE = NULL;

void Part::Init()
{
	// パラメータ設定
	float random = Random(0, 360);
	m_Rotation = D3DXVECTOR3(0.0f, random *  D3DX_PI / 180.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	m_RotateVector = D3DXVECTOR3(sinf(m_Rotation.y) / 10.0f, 0.55f + random / 3600.0f, cosf(m_Rotation.y) / 10.0f); // 各パーティクルごとにランダムで設定
	m_MaterialColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Frame = 0;


	// Audioのロード
	//if (!m_HealSE)
	//{
	//	m_HealSE = AddComponent<Audio>();
	//	m_HealSE->Load("asset\\audio\\se\\heal.wav");
	//}
}

void Part::Uninit()
{
	GameObject::Uninit();
}

void Part::Update()
{
	Scene* scene = Manager::GetScene();
	float radius = m_Scale.y;

	// 障害物との衝突判定
	float groundHeight = 0.0f; // 水面の高さ
	// 地面
	std::vector<Ground*> grounds = scene->GetGameObjects<Ground>();
	for (Ground* ground : grounds)
	{
		D3DXVECTOR3 position = ground->GetPosition();
		D3DXVECTOR3 scale = ground->GetScale();

		if (position.x - scale.x - radius < m_Position.x &&
			m_Position.x < position.x + scale.x + radius &&
			position.z - scale.z - radius < m_Position.z &&
			m_Position.z < position.z + scale.z + radius)
		{
			groundHeight = position.y + scale.y * 2.0f;
		}
	}
	// デバッグ用地面
	std::vector<Field*> fields = scene->GetGameObjects<Field>();
	for (Field* field : fields)
	{
		D3DXVECTOR3 position = field->GetPosition();
		D3DXVECTOR3 scale = field->GetScale();

		if (position.x - scale.x - radius < m_Position.x &&
			m_Position.x < position.x + scale.x + radius &&
			position.z - scale.z - radius < m_Position.z &&
			m_Position.z < position.z + scale.z + radius)
		{
			groundHeight = position.y + scale.y * 2.0f;
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

			if (position.x - scale.x - radius < m_Position.x &&
				m_Position.x < position.x + scale.x + radius &&
				position.z - scale.z - radius < m_Position.z &&
				m_Position.z < position.z + scale.z + radius)
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
		}
	}
	// 回転
	m_Rotation.x += 0.25f;
	// 回転方向に移動
	m_Position += m_RotateVector;
	// 重力移動
	m_RotateVector.y -= 0.0245f;
	if (m_Position.y <= groundHeight + m_Scale.y) m_RotateVector.y *= -m_CoefficientOfRestitution;


	m_Frame++;
	// 終了処理
	if (m_Frame >= 90)
	{
		switch (m_ParticleType)
		{
		case PARTICLE_ROCK:
			// 徐々に色を変化
			if (m_MaterialColor.a > 0.5f)
			{
				m_MaterialColor -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.01f);
			}
			// オブジェクト削除
			else SetDestroy();
			break;


		case PARTICLE_PLAYERDEATH:
			// 徐々に色を変化
			if (m_MaterialColor.a > 0.5f)
			{
				m_MaterialColor -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.01f);
			}
			// オブジェクト削除
			else SetDestroy();
			break;


		case PARTICLE_ENEMYDEATH:
			// プレイヤーの方向へ移動
			Player* player = scene->GetGameObject<Player>();
			D3DXVECTOR3 playerPos = player->GetPosition();
			D3DXVECTOR3 moveVector = playerPos - m_Position; // 移動方向ベクトル
			float length = D3DXVec3Length(&moveVector);      // プレイヤーとの距離
			D3DXVec3Normalize(&moveVector, &moveVector);
			m_Position -= m_RotateVector;                              // 回転移動分を無効にする
			m_Position += (moveVector / 3.0f) * (m_Frame / 90) * 2.5f; // プレイヤー方向の移動を加算する

			// プレイヤーとの当たり判定
			if (length <= PLAYER_RADIUS + radius + 1.2f)
			{
				// サウンド
				//m_HealSE->Play();

				// プレイヤー回復
				player->SetHealHP(0.05f);
				SetDestroy();
			}

			// オブジェクト強制削除
			if (m_Frame >= 2700) SetDestroy();
			break;
		}
	}
}


void Part::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferColoredObjectPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// テクスチャ設定
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// カラー設定
	POSTPROCESSPARAMETER param{};
	param.color = (D3DXVECTOR4)m_MaterialColor;
	Renderer::SetPostProcessParameter(param);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	if (m_ParticleType == PARTICLE_ENEMYDEATH) material.Shininess = 1.5f;
	material.Roughness = 0.5f;
	material.Metalic = 0.12f;

	// ポリゴン描写
	Resource::GetEnemyParticleModel()->Draw(material);
}


void Part::DrawShadowMapping()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetEnemyParticleModel()->Draw();
}

void Part::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ポリゴン描写
	Resource::GetEnemyParticleModel()->Draw();
}

void Part::DrawReflection()
{
	// 水面以下の場合は描画無し
	if (m_Position.y < 0.0f) return;

	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetColorVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetColorPS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Resource::GetEnemyParticleModel()->Draw();
}
