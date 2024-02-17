#pragma once
#include "gameObject.h"

// �����x�[�X�����_�����O�I�u�W�F�N�g�f�o�b�O�p�I�u�W�F�N�g
class PBRObject : public GameObject
{
private:
	float       m_Roughness;
	float       m_Metalic;

public:
	void Init() override {};
	void Init(D3DXVECTOR3 pos, float roughness, float metalic);
	void Uninit() override;
	void Update() override;
	void Draw() override; // PBR���ؗp
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
};