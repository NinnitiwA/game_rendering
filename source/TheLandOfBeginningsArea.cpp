#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dataStorage.h"
#include "random.h"
#include "TheLandOfBeginningsArea.h"
#include "area.h"
#include "eventManager.h"
#include "Ground.h"
#include "grass.h"
#include "enemy.h"
#include "enemyMagic.h"
#include "rock.h"
#include "tree.h"
#include "arrow.h"
#include "invisibleBox.h"
#include "water.h"

D3DXVECTOR3 TheLandOfBeginnings::m_AreaPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

void TheLandOfBeginnings::InitEnvironment()
{
	Scene* scene = Manager::GetScene();

	// システムオブジェクト
	EventManager::InitTheLandOfBeginnings(m_AreaPosition + m_AreaPosition);


	// 環境オブジェクト
	// 地面と草原
	int groundListNum = DataStorage::GetTheLandOfBeginningsDataStorage()->GroundGrassPos.size();
	for (int i = 0; i < groundListNum; i++)
	{
		Ground::CreateGround(scene, 1, 1, DataStorage::GetTheLandOfBeginningsDataStorage()->GroundGrassPos[i] + m_AreaPosition);
		Grass::CreateGrass(scene, 120, 1, 1, DataStorage::GetTheLandOfBeginningsDataStorage()->GroundGrassPos[i] + m_AreaPosition, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}
	// 岩
	int rockListNum = DataStorage::GetTheLandOfBeginningsDataStorage()->RockPos.size();
	for (int i = 0; i < rockListNum; i++)
	{
		Rock* rock = scene->AddGameObject<Rock>(1);
		rock->SetPosition(DataStorage::GetTheLandOfBeginningsDataStorage()->RockPos[i] + m_AreaPosition);
		if (i >= 2 && i < 11)
		{
			float randScale = Random(0, 100) / 50.0f - 0.5f;
			rock->SetScale(D3DXVECTOR3(4.5f + randScale, 4.5f + randScale, 4.5f + randScale));
		}
	}
	// 木
	int treeListNum = DataStorage::GetTheLandOfBeginningsDataStorage()->TreePos.size();
	for (int i = 0; i < treeListNum; i++)
	{
		scene->AddGameObject<Tree>(1)->SetPosition(DataStorage::GetTheLandOfBeginningsDataStorage()->TreePos[i] + m_AreaPosition);
	}
	// 矢印看板
	Arrow* arrow = scene->AddGameObject<Arrow>(1);
	arrow->SetPosition(D3DXVECTOR3(-17.0f, 9.0f, 25.0f) + m_AreaPosition);
	arrow->SetRotation(D3DXVECTOR3(0.0f, 210.0f * D3DX_PI / 180.0f, 0.0f));

}

void TheLandOfBeginnings::InitEnemy()
{
	Scene* scene = Manager::GetScene();

	// エネミー
	int enemyListNum = DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<Enemy>(1)->SetPosition(DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyPos[i] + m_AreaPosition);
	}
	enemyListNum = DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyMagicPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<EnemyMagic>(1)->SetPosition(DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyMagicPos[i] + m_AreaPosition);
	}
}