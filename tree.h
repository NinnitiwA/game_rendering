#pragma once
#include "gameObject.h"

class Tree : public GameObject
{
private:
	D3DXVECTOR3 m_BaseScale; // 初期設定時のスケール

	bool m_isSway = false; // 他オブジェクトの衝突時の揺れ識別子
	int  m_SwayFrame = 0;  // 揺れフレーム
	int  m_TreeType;       // 木モデルの種類

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	void SetIsSway() { m_isSway = true; }
};