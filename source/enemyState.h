#pragma once
class Enemy;

// エネミーの状態を表す列挙体
enum EnemyStateName
{
	ENEMY_STATE_IDLE,         // 待機
	ENEMY_STATE_WALK,         // 移動
	ENEMY_STATE_READY,        // 攻撃準備
	ENEMY_STATE_ATTACK,       // 攻撃
	ENEMY_STATE_DAMAGED,      // 攻撃を受けている
	ENEMY_STATE_STONE,        // タックル攻撃

	ENEMY_STATE_NUM,
};

// エネミーステートの基底クラス
class EnemyState
{
protected:
	Enemy* m_Enemy{};
	EnemyStateName m_EnemyStateName{};
public:
	EnemyState(Enemy* enemy) : m_Enemy(enemy) {}
	~EnemyState() { delete[] this; }

	virtual void StateAction() = 0;
	virtual EnemyStateName GetStateName() = 0;
};