#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>


template<class Archive>
void serialize(Archive & archive, D3DXVECTOR3 &vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
}

struct Vector3Data {
	std::string name;
	std::vector<D3DXVECTOR3> VectorArray;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(name), CEREAL_NVP(VectorArray));
	}
};


void LoadData();
void SaveData();


// プレイヤー
// 移動中のアニメーション
static D3DXVECTOR3 g_PlayerWalkAnimPos[14] =
{
	D3DXVECTOR3(0.0f, -0.05f, 0.0f), // 跳ねる前
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),

	D3DXVECTOR3(0.0f, 0.35f, 0.0f), // 跳ねている時
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, 0.35f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.12f, 0.0f),
	D3DXVECTOR3(0.0f, 0.08f, 0.0f),
	D3DXVECTOR3(0.0f, 0.04f, 0.0f),
};
static D3DXVECTOR3 g_PlayerWalkAnimScale[14] =
{
	D3DXVECTOR3(1.05f, 0.95f, 1.05f), // 跳ねる前
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),

	D3DXVECTOR3(1.0f, 1.2f, 1.0f), // 跳ねている時
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
};


// ジャンプ中のアニメーション
static D3DXVECTOR3 g_PlayerJumpAnimPos[13] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // ジャンプ前
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),

	D3DXVECTOR3(0.0f, 0.2f, 0.0f), // ジャンプ中
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.25f, 0.0f),
	D3DXVECTOR3(0.0f, 0.25f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
};
static D3DXVECTOR3 g_PlayerJumpAnimScale[13] =
{
	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // ジャンプ前
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),

	D3DXVECTOR3(0.8f, 1.4f, 0.8f),    // ジャンプ中
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
};


// 攻撃中のアニメーション
static D3DXVECTOR3 g_PlayerPressAttackAnimPos[14] =
{
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.22f, 0.0f),
	D3DXVECTOR3(0.0f, -0.17f, 0.0f),
	D3DXVECTOR3(0.0f, -0.14f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.6f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
};
static D3DXVECTOR3 g_PlayerPressAttackAnimScale[14] =
{
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.75f, 1.6f, 0.75f),
	D3DXVECTOR3(0.7f, 1.7f, 0.7f),
	D3DXVECTOR3(2.8f, 0.7f, 2.8f),
	D3DXVECTOR3(3.2f, 0.65f, 3.2f),
	D3DXVECTOR3(3.3f, 0.65f, 3.3f),
	D3DXVECTOR3(3.2f, 0.65f, 3.2f),
	D3DXVECTOR3(2.8f, 0.68f, 2.8f),
	D3DXVECTOR3(2.5f, 0.72f, 2.5f),
	D3DXVECTOR3(2.3f, 0.77f, 2.3f),
	D3DXVECTOR3(2.0f, 0.83f, 2.0f),
	D3DXVECTOR3(1.6f, 0.88f, 1.6f),
	D3DXVECTOR3(1.2f, 0.95f, 1.2f),
	D3DXVECTOR3(1.05f, 1.0f, 1.05f),
};

// 攻撃中のアニメーション
static D3DXVECTOR3 g_PlayerDashAttackAnimPos[40] =
{
	D3DXVECTOR3(0.0f,0.0f, -0.1f),  // 攻撃前
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),

	D3DXVECTOR3(0.0f, 0.0f,  0.095f), // 伸びる
	D3DXVECTOR3(0.0f, 0.0f,  0.14f),
	D3DXVECTOR3(0.0f, 0.0f,  0.185f),
	D3DXVECTOR3(0.0f, 0.0f,  0.23f),
	D3DXVECTOR3(0.0f, 0.0f,  0.245f),
	D3DXVECTOR3(0.0f, 0.0f,  0.29f),
	D3DXVECTOR3(0.0f, 0.0f,  0.335f),
	D3DXVECTOR3(0.0f, 0.0f,  0.38f),
	D3DXVECTOR3(0.0f, 0.0f,  0.425f),
	D3DXVECTOR3(0.0f, 0.0f,  0.47f),
	D3DXVECTOR3(0.0f, 0.0f,  0.515f),
	D3DXVECTOR3(0.0f, 0.0f,  0.56f),
	D3DXVECTOR3(0.0f, 0.0f,  0.605f),
	D3DXVECTOR3(0.0f, 0.0f,  0.65f),
	D3DXVECTOR3(0.0f, 0.0f,  0.695f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.15f),
	D3DXVECTOR3(0.0f, 0.0f,  0.05f),

	D3DXVECTOR3(0.0f, 0.0f,  0.6f), // 攻撃中
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.5f),
	D3DXVECTOR3(0.0f, 0.0f,  0.5f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f,  0.05f),
};
static D3DXVECTOR3 g_PlayerDashAttackAnimScale[40] =
{
	D3DXVECTOR3(1.02f, 1.0f, 1.02f), // 攻撃前
	D3DXVECTOR3(1.05f, 1.0f, 1.05f),
	D3DXVECTOR3(1.08f, 1.0f, 1.08f),
	D3DXVECTOR3(1.11f, 1.0f, 1.11f),
	D3DXVECTOR3(1.14f, 1.0f, 1.14f),
	D3DXVECTOR3(1.16f, 1.0f, 1.16f),
	D3DXVECTOR3(1.19f, 1.0f, 1.19f),
	D3DXVECTOR3(1.21f, 1.0f, 1.21f),
	D3DXVECTOR3(1.24f, 1.0f, 1.24f),
	D3DXVECTOR3(1.27f, 1.0f, 1.27f),
	D3DXVECTOR3(1.3f, 1.0f, 1.3f),
	D3DXVECTOR3(1.32f, 1.0f, 1.32f),
	D3DXVECTOR3(1.34f, 1.0f, 1.34f),
	D3DXVECTOR3(1.35f, 1.0f, 1.35f),

	D3DXVECTOR3(1.0f, 1.0f,  1.25f), // 伸びる
	D3DXVECTOR3(1.0f, 1.0f,  1.5f),
	D3DXVECTOR3(1.0f, 1.0f,  1.75f),
	D3DXVECTOR3(1.0f, 1.0f,  2.0f),
	D3DXVECTOR3(1.0f, 1.0f,  2.4f),
	D3DXVECTOR3(1.0f, 1.0f,  2.8f),
	D3DXVECTOR3(1.0f, 1.0f,  3.4f),
	D3DXVECTOR3(1.0f, 1.0f,  3.8f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),

	D3DXVECTOR3(1.2f, 1.2f,  0.8f), // 攻撃中
	D3DXVECTOR3(1.2f, 1.2f,  0.8f),
	D3DXVECTOR3(1.16f, 1.15f,  0.85f),
	D3DXVECTOR3(1.1f, 1.1f,  0.9f),
	D3DXVECTOR3(1.5f, 1.5f,  0.93f),
	D3DXVECTOR3(1.3f, 1.3f,  0.96f),
	D3DXVECTOR3(1.2f, 1.2f,  0.98f),
	D3DXVECTOR3(1.1f, 1.1f,  0.99f),
};


// エネミー
// 移動中のアニメーション
static D3DXVECTOR3 g_EnemyFieldWalkAnimPos[14] =
{
	D3DXVECTOR3(0.0f, -0.05f, 0.0f), // 跳ねる前
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),

	D3DXVECTOR3(0.0f, 0.35f, 0.0f),  // 跳ねている時
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, 0.35f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.12f, 0.0f),
	D3DXVECTOR3(0.0f, 0.08f, 0.0f),
	D3DXVECTOR3(0.0f, 0.04f, 0.0f),
};
static D3DXVECTOR3 g_EnemyFieldWalkAnimScale[14] =
{
	D3DXVECTOR3(1.05f, 0.95f, 1.05f), // 跳ねる前
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),

	D3DXVECTOR3(1.0f, 1.2f, 1.0f), // 跳ねている時
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
};

// 攻撃中のアニメーション
static D3DXVECTOR3 g_EnemyFieldAttackAnimPos[32] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.0f), // 攻撃前
	D3DXVECTOR3(0.0f, 0.0f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.15f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),

	D3DXVECTOR3(0.0f, 0.0f, -0.9f), // 攻撃後
	D3DXVECTOR3(0.0f, 0.0f, -0.9f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
};
static D3DXVECTOR3 g_EnemyFieldAttackAnimScale[32] =
{
	D3DXVECTOR3(1.05f, 1.05f, 0.9f), // 攻撃前
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.15f, 1.15f, 0.75f),
	D3DXVECTOR3(1.15f, 1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),

	D3DXVECTOR3(0.7f,  0.7f, 2.0f), // 攻撃後
	D3DXVECTOR3(0.7f,  0.7f, 2.0f),
	D3DXVECTOR3(0.75f, 0.75f,2.0f),
	D3DXVECTOR3(0.8f,  0.8f, 2.0f),
	D3DXVECTOR3(0.85f, 0.85f,2.0f),
	D3DXVECTOR3(0.9f,  0.9f, 2.0f),
	D3DXVECTOR3(0.95f, 0.95f,2.0f),
	D3DXVECTOR3(0.95f, 0.95f,1.8f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.3f),
	D3DXVECTOR3(0.95f, 0.95f,1.15f),
};


// 魔法詠唱中のアニメーション
static D3DXVECTOR3 g_PlayerMagicAnimPos[86] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // 縮む
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),

	D3DXVECTOR3(0.0f, -0.2f, 0.0f),// 伸びる
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f), 
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),

	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // 縮む
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.45f, 0.0f),
	D3DXVECTOR3(0.0f, -0.45f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),

	D3DXVECTOR3(0.0f, -0.2f, 0.0f),// 伸びる
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.14f, 0.0f),
	D3DXVECTOR3(0.0f, 0.155f, 0.0f),
	D3DXVECTOR3(0.0f, 0.17f, 0.0f),
	D3DXVECTOR3(0.0f, 0.19f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(-0.1f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.05f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(0.0f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.1f),
	D3DXVECTOR3(-0.05f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.05f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(0.0f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.1f),
	D3DXVECTOR3(0.0f, 0.015f, 0.05f),
	D3DXVECTOR3(0.05f, 0.015f, 0.0f),
	D3DXVECTOR3(-0.1f, 0.015f, 0.05f),
	D3DXVECTOR3(0.0f, 0.015f, 0.0f),
	D3DXVECTOR3(0.05f, 0.015f, -0.05f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.025f, 0.0f),
	D3DXVECTOR3(0.0f, 0.025f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0125f, 0.0f),
};
static D3DXVECTOR3 g_PlayerMagicAnimScale[86] =
{
	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // 縮む
	D3DXVECTOR3(1.15f, 0.75f, 1.15f),
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.25f, 0.7f, 1.25f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.40f, 0.7f, 1.40f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),

	D3DXVECTOR3(1.45f, 0.65f, 1.45f), // 伸びる
	D3DXVECTOR3(1.25f, 0.75f, 1.25f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),    
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),

	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // 縮む
	D3DXVECTOR3(1.15f, 0.75f, 1.15f),
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.25f, 0.7f, 1.25f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.40f, 0.7f, 1.40f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.75f, 0.50f, 1.75f),
	D3DXVECTOR3(1.75f, 0.50f, 1.75f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),

	D3DXVECTOR3(1.45f, 0.65f, 1.45f), // 伸びる
	D3DXVECTOR3(1.35f, 0.70f, 1.35f),
	D3DXVECTOR3(1.2f, 0.80f, 1.2f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.7f, 1.6f, 0.7f),
	D3DXVECTOR3(0.775f, 1.75f, 0.775f),
	D3DXVECTOR3(0.75f, 1.85f, 0.75f),
	D3DXVECTOR3(0.75f, 1.95f, 0.75f),
	D3DXVECTOR3(0.75f, 2.05f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.8f, 1.85f, 0.8f),
	D3DXVECTOR3(0.8f, 1.65f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(0.95f, 1.1f, 0.95f),
	D3DXVECTOR3(0.95f, 1.05f, 0.95f),
};

// 魔法攻撃中のアニメーション
static D3DXVECTOR3 g_EnemyMagicAttackAnimPos[54] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.0f), // 攻撃前
	D3DXVECTOR3(0.0f, 0.0f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.15f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // 攻撃一回目
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.2f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // 攻撃二回目
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.2f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // 攻撃三回目
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
};
static D3DXVECTOR3 g_EnemyMagicAttackAnimScale[54] =
{
	D3DXVECTOR3(1.05f, 1.05f, 0.9f), // 攻撃前
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.15f, 1.15f, 0.75f),
	D3DXVECTOR3(1.15f, 1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // 攻撃一回目
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // 攻撃二回目
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // 攻撃三回目
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
};


// ステージに配置する岩の座標リスト
static D3DXVECTOR3 m_RockPositionList[] =
{
	D3DXVECTOR3(-3.0f, 2.0f, -10.0f),
	D3DXVECTOR3(50.0f, 4.0f, 12.0f),

	D3DXVECTOR3(118.0f, 8.0f, 64.0f), // 壁岩
	D3DXVECTOR3(123.0f, 8.0f, 68.0f),
	D3DXVECTOR3(120.0f, 8.0f, 72.0f),
	D3DXVECTOR3(128.0f, 8.0f, 76.0f),
	D3DXVECTOR3(119.5f, 8.0f, 80.0f),
	D3DXVECTOR3(122.0f, 8.0f, 84.0f),
	D3DXVECTOR3(120.0f, 8.0f, 88.0f),
	D3DXVECTOR3(116.5f, 8.0f, 92.0f),
	D3DXVECTOR3(120.0f, 8.0f, 95.0f),

	D3DXVECTOR3(168.0f, 8.0f, 150.0f),
};

// ステージに配置する木の座標リスト
static D3DXVECTOR3 m_TreePositionList[] =
{
	D3DXVECTOR3(-17.0f, 3.0f, -16.0f),
	D3DXVECTOR3(-5.0f, 3.0f, 18.0f),
	D3DXVECTOR3(38.0f, 3.0f, -10.0f),
	D3DXVECTOR3(28.0f, 6.0f, 58.0f),
	D3DXVECTOR3(68.0f, 6.0f, 38.0f),
	D3DXVECTOR3(78.0f, 7.5f, 98.0f),
	D3DXVECTOR3(179.0f, 7.5f, 64.0f),
	D3DXVECTOR3(142.0f, 7.5f, 124.0f),

	D3DXVECTOR3(142.0f, 7.5f, 198.0f), // ここから最期の道
	D3DXVECTOR3(176.0f, 7.5f, 187.0f),
	D3DXVECTOR3(144.0f, 7.5f, 177.0f),
	D3DXVECTOR3(177.0f, 7.5f, 173.0f),

	D3DXVECTOR3(143.0f, 7.5f, 225.0f),
	D3DXVECTOR3(177.0f, 7.5f, 225.0f),

	D3DXVECTOR3(145.0f, 7.5f, 229.0f),
	D3DXVECTOR3(175.0f, 7.5f, 229.0f),

	D3DXVECTOR3(144.0f, 7.5f, 233.0f),
	D3DXVECTOR3(176.0f, 7.5f, 233.0f),
	D3DXVECTOR3(170.0f, 7.5f, 235.0f),

	D3DXVECTOR3(147.0f, 7.5f, 237.0f),
	D3DXVECTOR3(173.0f, 7.5f, 237.0f),

	D3DXVECTOR3(145.0f, 7.5f, 241.0f),
	D3DXVECTOR3(175.0f, 7.5f, 241.0f),

	D3DXVECTOR3(146.0f, 7.5f, 245.0f),
	D3DXVECTOR3(174.0f, 7.5f, 245.0f),

	D3DXVECTOR3(149.0f, 7.5f, 249.0f),
	D3DXVECTOR3(171.0f, 7.5f, 249.0f),
	D3DXVECTOR3(144.0f, 7.5f, 250.0f),

	D3DXVECTOR3(147.0f, 7.5f, 253.0f),
	D3DXVECTOR3(173.0f, 7.5f, 253.0f),

	D3DXVECTOR3(148.0f, 7.5f, 257.0f),
	D3DXVECTOR3(172.0f, 7.5f, 257.0f),
	D3DXVECTOR3(142.0f, 7.5f, 257.0f),
	D3DXVECTOR3(178.0f, 7.5f, 257.0f),

	D3DXVECTOR3(150.0f, 7.5f, 261.0f),
	D3DXVECTOR3(170.0f, 7.5f, 261.0f),

	D3DXVECTOR3(151.0f, 7.5f, 265.0f),
	D3DXVECTOR3(169.0f, 7.5f, 265.0f),

	D3DXVECTOR3(149.0f, 7.5f, 269.0f),
	D3DXVECTOR3(171.0f, 7.5f, 269.0f),
	D3DXVECTOR3(143.0f, 7.5f, 269.0f),
	D3DXVECTOR3(177.0f, 7.5f, 269.0f),

	D3DXVECTOR3(150.0f, 7.5f, 273.0f),
	D3DXVECTOR3(170.0f, 7.5f, 273.0f),
	D3DXVECTOR3(145.0f, 7.5f, 273.0f),
	D3DXVECTOR3(175.0f, 7.5f, 273.0f),

	D3DXVECTOR3(153.0f, 7.5f, 277.0f),
	D3DXVECTOR3(167.0f, 7.5f, 277.0f),

	D3DXVECTOR3(152.0f, 7.5f, 281.0f),
	D3DXVECTOR3(168.0f, 7.5f, 281.0f),
	D3DXVECTOR3(144.0f, 7.5f, 281.0f),
	D3DXVECTOR3(176.0f, 7.5f, 281.0f),

	D3DXVECTOR3(150.0f, 7.5f, 285.0f),
	D3DXVECTOR3(170.0f, 7.5f, 285.0f),
	D3DXVECTOR3(142.0f, 7.5f, 285.0f),
	D3DXVECTOR3(178.0f, 7.5f, 285.0f),

	D3DXVECTOR3(153.0f, 7.5f, 289.0f),
	D3DXVECTOR3(167.0f, 7.5f, 289.0f),

	D3DXVECTOR3(154.0f, 7.5f, 293.0f),
	D3DXVECTOR3(166.0f, 7.5f, 293.0f),
	D3DXVECTOR3(145.0f, 7.5f, 293.0f),
	D3DXVECTOR3(175.0f, 7.5f, 293.0f),

	D3DXVECTOR3(152.0f, 7.5f, 297.0f),
	D3DXVECTOR3(168.0f, 7.5f, 297.0f),
	D3DXVECTOR3(143.0f, 7.5f, 297.0f),
	D3DXVECTOR3(177.0f, 7.5f, 297.0f),

	D3DXVECTOR3(155.0f, 7.5f, 301.0f),
	D3DXVECTOR3(165.0f, 7.5f, 301.0f),
	D3DXVECTOR3(145.0f, 7.5f, 302.0f),
	D3DXVECTOR3(175.0f, 7.5f, 302.0f),
	D3DXVECTOR3(150.0f, 7.5f, 302.0f),
	D3DXVECTOR3(170.0f, 7.5f, 302.0f),
	D3DXVECTOR3(152.0f, 7.5f, 303.0f),
	D3DXVECTOR3(168.0f, 7.5f, 303.0f),
	D3DXVECTOR3(156.0f, 7.5f, 303.0f),
	D3DXVECTOR3(164.0f, 7.5f, 303.0f),
	D3DXVECTOR3(160.0f, 7.5f, 305.0f),

	D3DXVECTOR3(143.0f, 7.5f, 308.0f),
	D3DXVECTOR3(148.0f, 7.5f, 308.0f),
	D3DXVECTOR3(152.0f, 7.5f, 308.0f),
	D3DXVECTOR3(157.0f, 7.5f, 308.0f),
	D3DXVECTOR3(162.0f, 7.5f, 308.0f),
	D3DXVECTOR3(172.0f, 7.5f, 308.0f),
	D3DXVECTOR3(177.0f, 7.5f, 308.0f),

	D3DXVECTOR3(142.0f, 7.5f, 311.0f),
	D3DXVECTOR3(146.0f, 7.5f, 311.0f),
	D3DXVECTOR3(155.0f, 7.5f, 311.0f),
	D3DXVECTOR3(160.0f, 7.5f, 311.0f),
	D3DXVECTOR3(165.0f, 7.5f, 311.0f),
	D3DXVECTOR3(170.0f, 7.5f, 311.0f),
	D3DXVECTOR3(175.0f, 7.5f, 311.0f),
	D3DXVECTOR3(178.0f, 7.5f, 311.0f),

	D3DXVECTOR3(143.0f, 7.5f, 315.0f),
	D3DXVECTOR3(148.0f, 7.5f, 315.0f),
	D3DXVECTOR3(152.0f, 7.5f, 315.0f),
	D3DXVECTOR3(157.0f, 7.5f, 315.0f),
	D3DXVECTOR3(162.0f, 7.5f, 315.0f),
	D3DXVECTOR3(167.0f, 7.5f, 315.0f),
	D3DXVECTOR3(172.0f, 7.5f, 315.0f),

	D3DXVECTOR3(142.0f, 7.5f, 320.0f),
	D3DXVECTOR3(146.0f, 7.5f, 320.0f),
	D3DXVECTOR3(150.0f, 7.5f, 320.0f),
	D3DXVECTOR3(155.0f, 7.5f, 320.0f),
	D3DXVECTOR3(165.0f, 7.5f, 320.0f),
	D3DXVECTOR3(170.0f, 7.5f, 320.0f),
	D3DXVECTOR3(175.0f, 7.5f, 320.0f),
	D3DXVECTOR3(178.0f, 7.5f, 320.0f),

	D3DXVECTOR3(148.0f, 7.5f, 326.0f),
	D3DXVECTOR3(152.0f, 7.5f, 326.0f),
	D3DXVECTOR3(157.0f, 7.5f, 326.0f),
	D3DXVECTOR3(162.0f, 7.5f, 326.0f),
	D3DXVECTOR3(167.0f, 7.5f, 326.0f),
	D3DXVECTOR3(172.0f, 7.5f, 326.0f),
	D3DXVECTOR3(177.0f, 7.5f, 326.0f),

	D3DXVECTOR3(142.0f, 7.5f, 332.0f),
	D3DXVECTOR3(150.0f, 7.5f, 332.0f),
	D3DXVECTOR3(155.0f, 7.5f, 332.0f),
	D3DXVECTOR3(160.0f, 7.5f, 332.0f),
	D3DXVECTOR3(165.0f, 7.5f, 332.0f),
	D3DXVECTOR3(175.0f, 7.5f, 332.0f),
	D3DXVECTOR3(178.0f, 7.5f, 332.0f),
};

// ステージに配置する地面と草原の座標リスト
static D3DXVECTOR3 m_GroundGrassPositionList[] =
{
	D3DXVECTOR3(0.0f, 3.0f, 0.0f),
	D3DXVECTOR3(40.0f, 3.0f, 0.0f),
	D3DXVECTOR3(40.0f, 6.0f, 40.0f),
	D3DXVECTOR3(80.0f, 6.0f, 40.0f),
	D3DXVECTOR3(80.0f, 7.5f, 80.0f),
	D3DXVECTOR3(120.0f, 7.5f, 80.0f),
	D3DXVECTOR3(160.0f, 7.5f, 80.0f),
	D3DXVECTOR3(160.0f, 7.5f, 120.0f),
	D3DXVECTOR3(160.0f, 7.5f, 160.0f),
	D3DXVECTOR3(160.0f, 7.5f, 200.0f),
	D3DXVECTOR3(160.0f, 7.5f, 240.0f),
	D3DXVECTOR3(160.0f, 7.5f, 280.0f),
	D3DXVECTOR3(160.0f, 7.5f, 320.0f),
};