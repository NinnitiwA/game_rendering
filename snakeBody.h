#pragma once
#include "enemy.h"
#include "snakeHead.h"

class SnakeBody : public Enemy
{
private:
	D3DXVECTOR3 m_PreSnakePosition; // 直前蛇オブジェクトの座標
	int         m_Number;           // 胴体の順番
	SNAKEPARAMETER m_Param;

public:
	void SetPreSnakePosition(D3DXVECTOR3 Position) { m_PreSnakePosition = Position; }
	void SetSnakeBodyNumber(int num) { m_Number = num; }

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawShadowMapping();
	void DrawZPrePass();

	// 状況を見てエネミーの行動を決定する関数
	void EnemyActionDecider();
	// 直接攻撃された場合のダメージ処理
	void SetDamage(float);
	// 頭部から呼ばれた場合のダメージ処理
	void SetDamageFromHead(float);
	// スネークのステートを設定
	void SetSnakeState(SNAKE_STATE SnakeState) { m_Param.State = SnakeState; }
};

