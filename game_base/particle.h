#pragma once
#include "gameObject.h"

// パーティクルの種類
enum ParticleType
{
	PARTICLE_ROCK,        // 岩
	PARTICLE_ENEMYDEATH,  // エネミー死亡
	PARTICLE_PLAYERDEATH, // プレイヤー死亡

	PARTICLE_NON,
};

// パーティクル生成クラス
class Particle : public GameObject
{
public:
	// エフェクト生成時に呼び出す
	void SetParticle(D3DXVECTOR3 pos, ParticleType effectType);
};
