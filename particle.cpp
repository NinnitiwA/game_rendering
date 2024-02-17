#include "main.h"
#include "manager.h"
#include "scene.h"

#include "particle.h"
#include "part.h"

void Particle::SetParticle(D3DXVECTOR3 pos, ParticleType effectType)
{
	Scene* scene = Manager::GetScene();

	// 座標設定
	SetPosition(pos);

	// エフェクトパラメータ設定
	switch (effectType)
	{
	case PARTICLE_ROCK:
		for (int i = 0; i < 10; i++)
		{
			Part* part = scene->AddGameObject<Part>(1);
			part->SetPosition(pos);
			part->SetParticleType(PARTICLE_ROCK);
			part->SetCoefficientOfRestitution(0.6f);
			part->SetMaterialColor(D3DXCOLOR(0.800f, 0.515f, 0.146f, 1.0f));
		}
		break;
	case PARTICLE_ENEMYDEATH:
		for (int i = 0; i < 6; i++)
		{
			Part* part = scene->AddGameObject<Part>(1);
			part->SetPosition(pos);
			part->SetParticleType(PARTICLE_ENEMYDEATH);
			part->SetCoefficientOfRestitution(0.80f);
			part->SetMaterialColor(D3DXCOLOR(0.014f, 0.815f, 0.146f, 1.0f));
		}
		break;
	case PARTICLE_PLAYERDEATH:
		for (int i = 0; i < 6; i++)
		{
			Part* part = scene->AddGameObject<Part>(1);
			part->SetPosition(pos);
			part->SetParticleType(PARTICLE_PLAYERDEATH);
			part->SetCoefficientOfRestitution(1.0f);
			part->SetMaterialColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
		}
		break;
	case PARTICLE_NON:
		break;
	}
}