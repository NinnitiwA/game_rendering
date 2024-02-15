#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dataStorage.h"
#include "random.h"
#include "TheSkyIslandArea.h"
#include "area.h"

#include "eventManager.h"
#include "Ground.h"
#include "grass.h"
#include "enemy.h"
#include "enemyMagic.h"
#include "enemyStone.h"
#include "snakeHead.h"
#include "snakeBody.h"
#include "rock.h"
#include "tree.h"
#include "water.h"

D3DXVECTOR3 TheSkyIsland::m_AreaPosition = D3DXVECTOR3(95.0f, 8.0f, 370.0f);
//D3DXVECTOR3 TheSkyIsland::m_AreaPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

void TheSkyIsland::InitEnvironment()
{
	Scene* scene = Manager::GetScene();


	// システムオブジェクト
	EventManager::InitTheSkyIsland(m_AreaPosition);


	// 物理オブジェクト
	// 地面と草原
	int groundListNum = DataStorage::GetTheSkyIslandDataStorage()->GroundGrassPos.size();
	for (int i = 0; i < groundListNum; i++)
	{
		if (i == 1) Ground::SetGroundType(FLOATING_TYPE);
		Ground::CreateGround(scene, 1, 1, DataStorage::GetTheSkyIslandDataStorage()->GroundGrassPos[i] + m_AreaPosition);
		Grass::CreateGrass(scene, 120, 1, 1, DataStorage::GetTheSkyIslandDataStorage()->GroundGrassPos[i] + m_AreaPosition, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}
	Ground::SetGroundType(GROUNDING_TYPE);
	// 岩
	int rockListNum = DataStorage::GetTheSkyIslandDataStorage()->RockPos.size();
	for (int i = 0; i < rockListNum; i++)
	{
		Rock* rock = scene->AddGameObject<Rock>(1);
		rock->SetPosition(DataStorage::GetTheSkyIslandDataStorage()->RockPos[i] + m_AreaPosition);
		if (i >= 2 && i < 11)
		{
			float randScale = Random(0, 100) / 50.0f - 0.5f;
			rock->SetScale(D3DXVECTOR3(4.5f + randScale, 4.5f + randScale, 4.5f + randScale));
		}
	}
	// 木
	int treeListNum = DataStorage::GetTheSkyIslandDataStorage()->TreePos.size();
	for (int i = 0; i < treeListNum; i++)
	{
		scene->AddGameObject<Tree>(1)->SetPosition(DataStorage::GetTheSkyIslandDataStorage()->TreePos[i] + m_AreaPosition);
	}

}

void TheSkyIsland::InitEnemy()
{
	Scene* scene = Manager::GetScene();

	// エネミー
	int enemyListNum = DataStorage::GetTheSkyIslandDataStorage()->EnemyPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<Enemy>(1)->SetPosition(DataStorage::GetTheSkyIslandDataStorage()->EnemyPos[i] + m_AreaPosition);
	}
	enemyListNum = DataStorage::GetTheSkyIslandDataStorage()->EnemyMagicPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<EnemyMagic>(1)->SetPosition(DataStorage::GetTheSkyIslandDataStorage()->EnemyMagicPos[i] + m_AreaPosition);
	}
	enemyListNum = DataStorage::GetTheSkyIslandDataStorage()->EnemyStonePos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<EnemyStone>(1)->SetPosition(DataStorage::GetTheSkyIslandDataStorage()->EnemyStonePos[i] + m_AreaPosition);
	}

	// ボスエネミー
	SnakeHead* snake = scene->AddGameObject<SnakeHead>(1);
	snake->SetPosition(D3DXVECTOR3(-135.0f, 26.0f, 170.0f) + m_AreaPosition);
	snake->CreateSnakeBody();
}