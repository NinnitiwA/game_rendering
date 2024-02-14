#pragma once
#include "gameObject.h"

// �n�ʐ������ɉe������n�ʂ̃^�C�v
enum GROUND_TYPE
{
	GROUNDING_TYPE, // �ڒn
	FLOATING_TYPE,  // ���V
};

class Ground : public GameObject
{
private:
	static GROUND_TYPE m_GroundType;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	// �n�ʐ���
	static void CreateGround(Scene* Scene, int X, int Z, D3DXVECTOR3 Position);
	// �n�ʂ̃^�C�v�ݒ�
	static void SetGroundType(GROUND_TYPE GroundType) { m_GroundType = GroundType; }
};