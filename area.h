#pragma once
#include "scene.h"

// フィールドタイプ
enum FIELDTYPE
{
	FIELD_GROUNDING, // フィールドが接地（高さが地面から水面まで）
	FIELD_FLOATING,  // フィールドが浮遊（高さが6.0fで固定）
};

// ゲーム内ステージおよび各エリアを統括するクラス
class Area
{
private:
	static FIELDTYPE m_FieldType;

public:
	// フィールドタイプ設定
	static void SetFieldType(FIELDTYPE FieldType) { m_FieldType = FieldType; }

	void InitEnvironment();
	void InitEnemy();
};