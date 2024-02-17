#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dataStorage.h"
#include "random.h"
#include "TheCrystalFountainheadArea.h"
#include "area.h"
#include "eventManager.h"
#include "Ground.h"
#include "grass.h"
#include "enemy.h"
#include "enemyMagic.h"
#include "enemyStone.h"
#include "rock.h"
#include "tree.h"
#include "invisibleBox.h"
#include "water.h"

D3DXVECTOR3 TheCrystalFountainhead::m_AreaPosition = D3DXVECTOR3(200.0f, 0.0f, 150.0f);
//D3DXVECTOR3 TheCrystalFountainhead::m_AreaPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

void TheCrystalFountainhead::InitEnvironment()
{
	Scene* scene = Manager::GetScene();


	// �V�X�e���I�u�W�F�N�g
	EventManager::InitTheCrystalFountainhead(m_AreaPosition);


	// �����I�u�W�F�N�g
	// �n�ʂƑ���
	int groundListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->GroundGrassPos.size();
	for (int i = 0; i < groundListNum; i++)
	{
		Ground::CreateGround(scene, 1, 1, DataStorage::GetTheCrystalFountainheadDataStorage()->GroundGrassPos[i] + m_AreaPosition);
		Grass::CreateGrass(scene, 120, 1, 1, DataStorage::GetTheCrystalFountainheadDataStorage()->GroundGrassPos[i] + m_AreaPosition, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	}
	// ��
	int rockListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->RockPos.size();
	for (int i = 0; i < rockListNum; i++)
	{
		Rock* rock = scene->AddGameObject<Rock>(1);
		rock->SetPosition(DataStorage::GetTheCrystalFountainheadDataStorage()->RockPos[i] + m_AreaPosition);
		if (i >= 2 && i < 11)
		{
			float randScale = Random(0, 100) / 50.0f - 0.5f;
			rock->SetScale(D3DXVECTOR3(4.5f + randScale, 4.5f + randScale, 4.5f + randScale));
		}
	}
	// ��
	int treeListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->TreePos.size();
	for (int i = 0; i < treeListNum; i++)
	{
		scene->AddGameObject<Tree>(1)->SetPosition(DataStorage::GetTheCrystalFountainheadDataStorage()->TreePos[i] + m_AreaPosition);
	}
	// �����̏Փ˔���p�I�u�W�F�N�g
	InvisibleBox* invisibleBox = scene->AddGameObject<InvisibleBox>(1);
	invisibleBox->SetPosition(D3DXVECTOR3(40.0f, -2.5f, 140.0f) + m_AreaPosition);
	invisibleBox->SetScale(D3DXVECTOR3(60.0f, 1.0f, 80.0f));
	invisibleBox->SetCollision(true);

}

void TheCrystalFountainhead::InitEnemy()
{
	Scene* scene = Manager::GetScene();

	// �G�l�~�[
	int enemyListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<Enemy>(1)->SetPosition(DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyPos[i] + m_AreaPosition);
	}
	enemyListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyMagicPos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<EnemyMagic>(1)->SetPosition(DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyMagicPos[i] + m_AreaPosition);
	}
	enemyListNum = DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyStonePos.size();
	for (int i = 0; i < enemyListNum; i++)
	{
		scene->AddGameObject<EnemyStone>(1)->SetPosition(DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyStonePos[i] + m_AreaPosition);
	}
}