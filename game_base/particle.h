#pragma once
#include "gameObject.h"

// �p�[�e�B�N���̎��
enum ParticleType
{
	PARTICLE_ROCK,        // ��
	PARTICLE_ENEMYDEATH,  // �G�l�~�[���S
	PARTICLE_PLAYERDEATH, // �v���C���[���S

	PARTICLE_NON,
};

// �p�[�e�B�N�������N���X
class Particle : public GameObject
{
public:
	// �G�t�F�N�g�������ɌĂяo��
	void SetParticle(D3DXVECTOR3 pos, ParticleType effectType);
};
