#pragma once
#include "gameObject.h"


#define ENEMY_ATTACK_ADDITIONAL_LENGTH 1.2f // エネミ―攻撃時の補正距離

#define ENEMY_MAX_ATTACK_FRAME 32       // 攻撃アニメーション最大フレーム数
#define ENEMY_MAGIC_MAX_ATTACK_FRAME 54 // 魔法攻撃アニメーション最大フレーム数
#define ENEMY_MAX_WALK_FRAME 14         // 移動アニメーション最大フレーム数
#define ENEMY_MAX_DAMAGE_FRAME 20       // 被ダメージ状態最大フレーム数
#define ENEMY_MAX_STONE_FRAME 90        // 石化状態最大フレーム数

#define ENEMY_RADIUS 1.0f         // エネミーの半径


// Enemyの種類
enum ENEMY_TYPE
{
	ENEMY_NORMAL,
	ENEMY_MAGIC,
	ENEMY_STONE,
};


// すべてのEnemyの基本となるクラス
class Enemy : public GameObject
{
protected:
	bool        m_isTarget = false; // プレイヤーのターゲットにされているかどうかの識別子
	float       m_HitPoint = 5.0f;  // ヒットポイント

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_AttackVector{};   // 攻撃方向ベクトル

	ENEMY_TYPE        m_EnemyType{};      // エネミーの種類
	class EnemyState* m_EnemyState{};     // エネミーステート
	class EnemyState* m_NextEnemyState{}; // 次のエネミーステート

	float m_AttackRange{}; // 攻撃範囲
	float m_WalkRange{};   // 視認範囲
private:

public:
	Enemy()
	{
		SetTag(ENEMY);
		m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
	}


	void SetVelocity(D3DXVECTOR3 vec) { m_Velocity = vec; }
	void SetAttackVector(D3DXVECTOR3 attackVector) { m_AttackVector = attackVector; }
	D3DXVECTOR3 GetAttackVector() { return m_AttackVector; }
	EnemyState* GetEnemyState() { return m_EnemyState; }
	void SetNextEnemyState(EnemyState* state) { m_NextEnemyState = state; }
	void SetEnemyState();
	ENEMY_TYPE GetEnemyType() { return m_EnemyType; }
	// プレイヤーのターゲットになる
	void SetEnemyIsTarget(bool isTarget) { m_isTarget = isTarget; }
	// 攻撃時の当たり判定
	void AttackCollisionDetection();
	// ターゲットの方へ少しずつ回転する
	void RotateToTarget(D3DXVECTOR3* targetRot);
	// 攻撃範囲取得
	float GetAttackRange() { return m_AttackRange; }
	// 視認範囲取得
	float GetWalkRange() { return m_WalkRange; }

	// ダメージ処理
	virtual void SetDamage(float);
	// 死亡判定処理
	virtual void JudgeDeath();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};

enum SNAKE_STATE
{
	IDLE_STATE,
	WALK_STATE,
	READY_STATE,
	ATTACK_STATE,
	DAMAGED_STATE,
};

struct SNAKEPARAMETER
{
	SNAKE_STATE State;

	int DamageFrame;
	int AttackFrame;
	int WalkFrame;
	int ReadyFrame;
};
