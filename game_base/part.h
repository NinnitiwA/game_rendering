#pragma once
#include "particle.h"

// パーティクルを構成するオブジェクトクラス
class Part : public GameObject
{
private:
	float        m_CoefficientOfRestitution; // 反発係数
	D3DXVECTOR3  m_RotateVector;             // 回転しながら動く方向

	ParticleType m_ParticleType;             // パーティクルタイプ
	D3DXCOLOR    m_MaterialColor;            // マテリアルカラー

	int m_Frame;

	static class Audio* m_HealSE;

public:
	// パーティクルタイプ設定
	void SetParticleType(ParticleType ParticleType) { m_ParticleType = ParticleType; }
	// 反発係数設定
	void SetCoefficientOfRestitution(float CoR) { m_CoefficientOfRestitution = CoR; }
	// パーティクルカラー設定
	void SetMaterialColor(D3DXCOLOR MaterialColor) { m_MaterialColor = MaterialColor; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;
};
