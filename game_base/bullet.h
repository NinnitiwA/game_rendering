#pragma once
#include "gameObject.h"

class Bullet : public GameObject
{
private:
	//class Audio* m_AttackSE;

	D3DXVECTOR3    m_Velocity; // 移動速度
	int            m_frame;    // オブジェクト削除用フレーム

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;

	void SetVelocity(D3DXVECTOR3 vel) { m_Velocity = vel; }
};