#pragma once
#include "enemy.h"

#define SNAKE_MAX_ATTACK_FRAME 34   // 攻撃時最大フレーム数
#define SNAKE_MAX_DAMAGE_FRAME 20   // 被ダメージ状態フレーム数
#define SNAKE_RADIUS 1.0f           // スネークオブジェクトの半径
#define SNAKE_BODY_NUM 6            // 胴体の数（最後尾は空のオブジェクトを入れるため実体は6体）
#define SNAKE_BODY_LAST_ALLAY_NUM 5 // 胴体配列の最後尾番号（6体）

class SnakeHead : public Enemy
{
private:
	class SnakeBody* m_SnakeBodyList[SNAKE_BODY_NUM + 1]; // 胴体オブジェクトの配列
	SNAKEPARAMETER m_Param;

	float        m_MoveSpeed; // 移動速度

public:
	// 胴体配列を取得
	SnakeBody* GetSnakeBody(int num) { return m_SnakeBodyList[num]; }
	// 移動速度を取得
	float GetMoveSpeed(void) { return m_MoveSpeed; }

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadowMapping();
	void DrawZPrePass();

	// 胴体オブジェクト生成
	void CreateSnakeBody(void);
	// 状況を見たエネミーの行動決定関数
	void EnemyActionDecider();
	// ダメージ処理
	void SetDamage(float);
	// 死亡判定処理
	void JudgeDeath();
	// 生存中の最後尾オブジェクトの配列番号を取得
	int GetAliveSnakeBodyLastArrayNumber();
	// スネークのステートを設定
	void SetSnakeState(SNAKE_STATE SnakeState) { m_Param.State = SnakeState; }
};

