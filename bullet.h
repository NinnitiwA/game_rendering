#pragma once
#include "gameObject.h"

class Bullet : public GameObject
{
private:
	//class Audio* m_AttackSE;

	D3DXVECTOR3    m_Velocity; // �ړ����x
	int            m_frame;    // �I�u�W�F�N�g�폜�p�t���[��

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;

	void SetVelocity(D3DXVECTOR3 vel) { m_Velocity = vel; }
};