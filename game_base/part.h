#pragma once
#include "particle.h"

// �p�[�e�B�N�����\������I�u�W�F�N�g�N���X
class Part : public GameObject
{
private:
	float        m_CoefficientOfRestitution; // �����W��
	D3DXVECTOR3  m_RotateVector;             // ��]���Ȃ��瓮������

	ParticleType m_ParticleType;             // �p�[�e�B�N���^�C�v
	D3DXCOLOR    m_MaterialColor;            // �}�e���A���J���[

	int m_Frame;

	static class Audio* m_HealSE;

public:
	// �p�[�e�B�N���^�C�v�ݒ�
	void SetParticleType(ParticleType ParticleType) { m_ParticleType = ParticleType; }
	// �����W���ݒ�
	void SetCoefficientOfRestitution(float CoR) { m_CoefficientOfRestitution = CoR; }
	// �p�[�e�B�N���J���[�ݒ�
	void SetMaterialColor(D3DXCOLOR MaterialColor) { m_MaterialColor = MaterialColor; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
