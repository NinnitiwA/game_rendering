#pragma once
#include "gameObject.h"


#define PLAYER_RADIUS 1.0f        // プレイヤーの半径
#define PLAYER_MAX_HP 5.0f        // プレイヤーの最大HP

#define PLAYER_ATTACK_INTERVAL 40           // プレイヤーの攻撃間隔
#define PLAYER_MAX_MAGIC_FRAME 86			// 魔法詠唱最大フレーム数
#define PLAYER_MAGIC_MAX_ATTACK_FRAME 54    // 魔法攻撃アニメーション最大フレーム数
#define PLAYER_MAX_PRESS_ATTACK_FRAME 14    // プレス攻撃アニメーション最大フレーム数
#define PLAYER_MAX_DASH_ATTACK_FRAME 40		// ダッシュ攻撃アニメーション最大フレーム数
#define PLAYER_MAX_DASH_ATTACK_PRE_FRAME 32	// ダッシュ攻撃準備アニメーション最大フレーム数
#define PLAYER_MAX_JUMP_FRAME 13			// ジャンプアニメーション最大フレーム数
#define PLAYER_MAX_WALK_FRAME 14			// 移動アニメーション最大フレーム数
#define PLAYER_MAX_DODGE_FRAME 8			// 回避アニメーション最大フレーム数
#define PLAYER_MAX_DAMAGE_FRAME 12			// 被ダメージ状態最大フレーム数

enum PlayerState
{
	PLAYER_STATE_GROUND, // 接地中
	PLAYER_STARE_JUMP,   // ジャンプ中
	PLAYER_STATE_WALK,   // 移動中
	PLAYER_STATE_DODGE,  // 回避中

	PLAYER_STATE_PRESSATTACK,  // プレス攻撃中
	PLAYER_STATE_DASHATTACK,   // ダッシュ攻撃中

	PLAYER_STATE_NUM,
};

// プレイヤー攻撃種類
enum PlayerAttack
{
	// フィールド
	PLAYER_PRESS_ATTACK,
	PLAYER_DASH_ATTACK,

	// 戦闘
	PLAYER_MAGIC_CHANT,
	PLAYER_MAGIC_ATTACK,


	PLAYER_NON_ATTACK,
};

// プレイヤーパラメータ構造体
struct PlayerParameter
{
	float HitPoint;     // ヒットポイント

	int  AttackFrame;   // 攻撃間隔用フレーム
	bool canAttack;     // 攻撃可能識別子

	float dashLevel;    // 攻撃時のダッシュのレベル

	bool  isFindEnemy;  // ターゲット捕捉用識別子

	bool  isDamaged;    // 攻撃を受けているかどうか
	int   DamageFrame;  // 攻撃を受けた後の無敵時間
};

// プレイヤーアニメーション構造体
struct PlayerAnim
{
	bool isAttack;      // 攻撃中かどうか
	int  AttackFrame;   // 攻撃フレーム
	float AttackRot;    // 攻撃方向
	PlayerAttack PlayerAttack; // 攻撃の種類

	bool isGround;      // 接地しているかどうか
	bool isJump;        // ジャンプ中かどうか
	int  JumpFrame;     // ジャンプ中のアニメーションフレーム

	bool isWalk;        // 移動中かどうか  
	int  AnimFrame;     // 移動中のアニメーションフレーム

	bool isDodge;       // 回避中かどうか
	int  DodgeFrame;    // 回避中のアニメーションフレーム
};



class Player : public GameObject 
{
private:
	D3DXVECTOR3 m_Velocity{};         // 移動ベクトル
	D3DXVECTOR3 m_AttackedVector{};   // 攻撃を受けた方向ベクトル
	D3DXVECTOR3 m_DodgeVector{};      // 回避方向のベクトル;
									  
	PlayerParameter m_Param{};        // プレイヤーパラメータ
	PlayerAnim      m_PlayerAnim{};   // プレイヤーアニメーション

	class Enemy*    m_Target{};       // ロックオン中のターゲットエネミー
	D3DXVECTOR3     m_TargetVector{}; // ターゲット方向のベクトル
	float           m_TargetRot{};    // プレイヤーとターゲットの角度

	static D3DXVECTOR3 m_RespwanPosition; // リスポーン座標
	static bool        m_CanControl;      // プレイヤー操作フラグ

public:
	// リスポーン座標設定
	void SetRespawnPosition(D3DXVECTOR3 position) { m_RespwanPosition = position; }

	// 攻撃を受けた方向ベクトルをセット
	void SetAttackedVector(D3DXVECTOR3 vec)
	{
		D3DXVec3Normalize(&vec, &vec); // 正規化
		m_AttackedVector += vec;
		D3DXVec3Normalize(&m_AttackedVector, &m_AttackedVector); // 加算したベクトルを正規化
	}

	// 回避中識別子を取得
	bool GetPlayerIsDodge() { return m_PlayerAnim.isDodge; }
	// プレイヤーパラメータを取得
	PlayerParameter GetPlayerParameter() { return m_Param; }
	// 操作可能用識別子の設定
	void SetCanControl(bool use) { m_CanControl = use; }
	// 操作可能用識別子の取得
	bool GetCanControl() { return m_CanControl; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	// ターゲット更新処理
	void UpdateTargetEnemy();
	// 攻撃時の当たり判定
	void AttackCollisionDetection();
	// ダメージ処理
	void SetDamage(float damage);
	// プレイヤーの回復量処理
	void SetHealHP(float heal);
	// リスポーン処理設定
	void SetRespawn();
	// リスポーン処理
	void Respawn();
};



