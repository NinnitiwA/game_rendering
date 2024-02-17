#include "main.h"
#include "dataManager.h"
#include "dataStorage.h"


void LoadData()
{
	// アニメーション
	{
		// プレイヤー
		// 移動アニメーション
		{
			std::string filePath = "data/player/PlayerWalkAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++)
				DataStorage::GetPlayerDataStorage()->WalkAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/player/PlayerWalkAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->WalkAnimScale.push_back(inputData.VectorArray[i]);
		}

		// ジャンプアニメーション
		{
			std::string filePath = "data/player/PlayerJumpAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->JumpAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/player/PlayerJumpAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->JumpAnimScale.push_back(inputData.VectorArray[i]);
		}

		// プレス攻撃アニメーション
		{
			std::string filePath = "data/player/PlayerPressAttackAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->PressAttackAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/player/PlayerPressAttackAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->PressAttackAnimScale.push_back(inputData.VectorArray[i]);
		}

		// ダッシュ攻撃アニメーション
		{
			std::string filePath = "data/player/PlayerDashAttackAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->DashAttackAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/player/PlayerDashAttackAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->DashAttackAnimScale.push_back(inputData.VectorArray[i]);
		}

		// 魔法詠唱アニメーション
		{
			std::string filePath = "data/player/PlayerMagicAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->MagicAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/player/PlayerMagicAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->MagicAnimScale.push_back(inputData.VectorArray[i]);
		}



		// エネミー
		// 移動アニメーション
		{
			std::string filePath = "data/enemyField/EnemyFieldWalkAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/enemyField/EnemyFieldWalkAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale.push_back(inputData.VectorArray[i]);
		}

		// 攻撃アニメーション
		{
			std::string filePath = "data/enemyField/EnemyFieldAttackAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/enemyField/EnemyFieldAttackAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->AttackAnimScale.push_back(inputData.VectorArray[i]);
		}

		// 魔法攻撃アニメーション
		{
			std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale.push_back(inputData.VectorArray[i]);
		}

		// スネーク攻撃アニメーション
		{
			std::string filePath = "data/snake/SnakeAttackAnimPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/snake/SnakeAttackAnimScale.json"; // スケール

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("root", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->SnakeAttackAnimScale.push_back(inputData.VectorArray[i]);
		}
	}


	// チュートリアル
	{
		// チュートリアルオブジェクト座標
		{
			std::string filePath = "data/field/TutorialObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("rock", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTutorialDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TutorialObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("tree", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTutorialDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TutorialObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("groundgrass", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTutorialDataStorage()->GroundGrassPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TutorialObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("arrowPos", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTutorialDataStorage()->ArrowPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TutorialObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("arrowRot", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTutorialDataStorage()->ArrowRot.push_back(inputData.VectorArray[i]);
		}
	}


	// はじまりの大地
	{
		// はじまりの大地オブジェクト座標
		{
			std::string filePath = "data/field/TheLandOfBeginningsObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("rock", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTheLandOfBeginningsDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TheLandOfBeginningsObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("tree", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTheLandOfBeginningsDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TheLandOfBeginningsObjectPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("groundgrass", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTheLandOfBeginningsDataStorage()->GroundGrassPos.push_back(inputData.VectorArray[i]);
		}

		// はじまりの大地エネミー座標
		{
			std::string filePath = "data/field/TheLandOfBeginningsEnemyPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("enemy", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyPos.push_back(inputData.VectorArray[i]);
		}
		{
			std::string filePath = "data/field/TheLandOfBeginningsEnemyPos.json"; // 座標

			Vector3Data inputData;
			std::ifstream ifs(filePath);
			cereal::JSONInputArchive i_archive(ifs);
			i_archive(cereal::make_nvp("enemyMagic", inputData));

			int arraySize = inputData.VectorArray.size();
			for (int i = 0; i < arraySize; i++) DataStorage::GetTheLandOfBeginningsDataStorage()->EnemyMagicPos.push_back(inputData.VectorArray[i]);
		}
	}

	// 玲瓏水源オブジェクト座標
	{
		std::string filePath = "data/field/TheCrystalFountainheadObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("rock", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheCrystalFountainheadObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("tree", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheCrystalFountainheadObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("groundgrass", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->GroundGrassPos.push_back(inputData.VectorArray[i]);
	}

	// 玲瓏水源エネミー座標
	{
		std::string filePath = "data/field/TheCrystalFountainheadEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemy", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheCrystalFountainheadEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemyMagic", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyMagicPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheCrystalFountainheadEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemyStone", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheCrystalFountainheadDataStorage()->EnemyStonePos.push_back(inputData.VectorArray[i]);
	}

	// 空島オブジェクト座標
	{
		std::string filePath = "data/field/TheSkyIslandObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("rock", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheSkyIslandObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("tree", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheSkyIslandObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("groundgrass", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->GroundGrassPos.push_back(inputData.VectorArray[i]);
	}

	// 空島エネミー座標
	{
		std::string filePath = "data/field/TheSkyIslandEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemy", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->EnemyPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheSkyIslandEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemyMagic", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->EnemyMagicPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/TheSkyIslandEnemyPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("enemyStone", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetTheSkyIslandDataStorage()->EnemyStonePos.push_back(inputData.VectorArray[i]);
	}


//#ifdef _MSC_VER
//	system("pause");
//#endif
}


void SaveData()
{
	// アニメーション
	{
		// プレイヤー
		// 移動アニメーション
		{
			Vector3Data outputData;
			outputData.name = "PlayerWalkAnimPos"; // 座標
			int arraySize = sizeof(g_PlayerWalkAnimPos) / sizeof(g_PlayerWalkAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerWalkAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerWalkAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "PlayerWalkAnimScale"; // スケール
			int arraySize = sizeof(g_PlayerWalkAnimScale) / sizeof(g_PlayerWalkAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerWalkAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerWalkAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// ジャンプアニメーション
		{
			Vector3Data outputData;
			outputData.name = "PlayerJumpAnimPos"; // 座標
			int arraySize = sizeof(g_PlayerJumpAnimPos) / sizeof(g_PlayerJumpAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerJumpAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerJumpAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "PlayerJumpAnimScale"; // スケール
			int arraySize = sizeof(g_PlayerJumpAnimScale) / sizeof(g_PlayerJumpAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerJumpAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerJumpAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// プレス攻撃アニメーション
		{
			Vector3Data outputData;
			outputData.name = "PlayerPressAttackAnimPos"; // 座標
			int arraySize = sizeof(g_PlayerPressAttackAnimPos) / sizeof(g_PlayerPressAttackAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerPressAttackAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerPressAttackAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "PlayerPressAttackAnimScale"; // スケール
			int arraySize = sizeof(g_PlayerPressAttackAnimScale) / sizeof(g_PlayerPressAttackAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerPressAttackAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerPressAttackAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// ダッシュ攻撃アニメーション
		{
			Vector3Data outputData;
			outputData.name = "PlayerDashAttackAnimPos"; // 座標
			int arraySize = sizeof(g_PlayerDashAttackAnimPos) / sizeof(g_PlayerDashAttackAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerDashAttackAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerDashAttackAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "PlayerDashAttackAnimScale"; // スケール
			int arraySize = sizeof(g_PlayerDashAttackAnimScale) / sizeof(g_PlayerDashAttackAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerDashAttackAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerDashAttackAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// 魔法詠唱アニメーション
		{
			Vector3Data outputData;
			outputData.name = "PlayerMagicAnimPos"; // 座標
			int arraySize = sizeof(g_PlayerMagicAnimPos) / sizeof(g_PlayerMagicAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerMagicAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerMagicAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "PlayerMagicAnimScale"; // スケール
			int arraySize = sizeof(g_PlayerMagicAnimScale) / sizeof(g_PlayerMagicAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_PlayerMagicAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/player/PlayerMagicAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}


		// エネミー
		// 移動アニメーション
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldWalkAnimPos"; // 座標
			int arraySize = sizeof(g_EnemyFieldWalkAnimPos) / sizeof(g_EnemyFieldWalkAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyFieldWalkAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldWalkAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldWalkAnimScale"; // スケール
			int arraySize = sizeof(g_EnemyFieldWalkAnimScale) / sizeof(g_EnemyFieldWalkAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyFieldWalkAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldWalkAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// 攻撃アニメーション
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldAttackAnimPos"; // 座標
			int arraySize = sizeof(g_EnemyFieldAttackAnimPos) / sizeof(g_EnemyFieldAttackAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyFieldAttackAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldAttackAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldAttackAnimScale"; // スケール
			int arraySize = sizeof(g_EnemyFieldAttackAnimScale) / sizeof(g_EnemyFieldAttackAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyFieldAttackAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldAttackAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// 魔法攻撃アニメーション
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldMagicAttackAnimPos"; // 座標
			int arraySize = sizeof(g_EnemyMagicAttackAnimPos) / sizeof(g_EnemyMagicAttackAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyMagicAttackAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "EnemyFieldMagicAttackAnimScale"; // スケール
			int arraySize = sizeof(g_EnemyMagicAttackAnimScale) / sizeof(g_EnemyMagicAttackAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_EnemyMagicAttackAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}

		// スネーク攻撃アニメーション
		{
			Vector3Data outputData;
			outputData.name = "SnakeAttackAnimPos"; // 座標
			int arraySize = sizeof(g_SnakeAttackAnimPos) / sizeof(g_SnakeAttackAnimPos[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_SnakeAttackAnimPos[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/snake/SnakeAttackAnimPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
		{
			Vector3Data outputData;
			outputData.name = "SnakeAttackAnimScale"; // スケール
			int arraySize = sizeof(g_SnakeAttackAnimScale) / sizeof(g_SnakeAttackAnimScale[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_SnakeAttackAnimScale[i];
				outputData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/snake/SnakeAttackAnimScale.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("root", outputData));
			}
		}
	}


	// チュートリアル
	{
		// フィールドオブジェクト座標
		{
			Vector3Data outputRockData;
			outputRockData.name = "RockPosition"; // 座標
			int arraySize = sizeof(m_RockPositionList) / sizeof(m_RockPositionList[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = m_RockPositionList[i];
				outputRockData.VectorArray.push_back(vec3);
			}

			Vector3Data outputTreeData;
			outputTreeData.name = "TreePosition"; // 座標
			arraySize = sizeof(m_TreePositionList) / sizeof(m_TreePositionList[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = m_TreePositionList[i];
				outputTreeData.VectorArray.push_back(vec3);
			}

			Vector3Data outputGroundGrassData;
			outputGroundGrassData.name = "GroundGrassPosition"; // 座標
			arraySize = sizeof(m_GroundGrassPositionList) / sizeof(m_GroundGrassPositionList[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = m_GroundGrassPositionList[i];
				outputGroundGrassData.VectorArray.push_back(vec3);
			}

			Vector3Data outputArrowPosData;
			outputArrowPosData.name = "ArrowPosition"; // 座標
			arraySize = sizeof(g_TutorialArrowPositionList) / sizeof(g_TutorialArrowPositionList[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_TutorialArrowPositionList[i];
				outputArrowPosData.VectorArray.push_back(vec3);
			}

			Vector3Data outputArrowRotData;
			outputArrowRotData.name = "ArrowRotation"; // 角度
			arraySize = sizeof(g_TutorialArrowRotationList) / sizeof(g_TutorialArrowRotationList[0]);
			for (int i = 0; i < arraySize; i++) {
				D3DXVECTOR3 vec3 = g_TutorialArrowRotationList[i];
				outputArrowRotData.VectorArray.push_back(vec3);
			}

			std::string filePath = "data/field/TutorialObjectPos.json";

			std::ofstream ofs(filePath);
			{
				cereal::JSONOutputArchive o_archive(ofs);
				o_archive(cereal::make_nvp("rock", outputRockData));
				o_archive(cereal::make_nvp("tree", outputTreeData));
				o_archive(cereal::make_nvp("groundgrass", outputGroundGrassData));
				o_archive(cereal::make_nvp("arrowPos", outputArrowPosData));
				o_archive(cereal::make_nvp("arrowRot", outputArrowRotData));
			}
		}
	}

	// はじまりの大地オブジェクト座標
	{
		Vector3Data outputRockData;
		outputRockData.name = "RockPosition"; // 座標
		int arraySize = sizeof(TLOB_RockPositionList) / sizeof(TLOB_RockPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TLOB_RockPositionList[i];
			outputRockData.VectorArray.push_back(vec3);
		}

		Vector3Data outputTreeData;
		outputTreeData.name = "TreePosition"; // 座標
		arraySize = sizeof(TLOB_TreePositionList) / sizeof(TLOB_TreePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TLOB_TreePositionList[i];
			outputTreeData.VectorArray.push_back(vec3);
		}

		Vector3Data outputGroundGrassData;
		outputGroundGrassData.name = "GroundGrassPosition"; // 座標
		arraySize = sizeof(TLOB_GroundGrassPositionList) / sizeof(TLOB_GroundGrassPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TLOB_GroundGrassPositionList[i];
			outputGroundGrassData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheLandOfBeginningsObjectPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("rock", outputRockData));
			o_archive(cereal::make_nvp("tree", outputTreeData));
			o_archive(cereal::make_nvp("groundgrass", outputGroundGrassData));
		}
	}

	// はじまりの大地エネミー座標
	{
		Vector3Data outputEnemyData;
		outputEnemyData.name = "EnemyPosition"; // 座標
		int arraySize = sizeof(TLOB_EnemyPositionList) / sizeof(TLOB_EnemyPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TLOB_EnemyPositionList[i];
			outputEnemyData.VectorArray.push_back(vec3);
		}

		Vector3Data outputEnemyMagicData;
		outputEnemyMagicData.name = "EnemyMagicPosition"; // 座標
		arraySize = sizeof(TLOB_EnemyMagicPositionList) / sizeof(TLOB_EnemyMagicPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TLOB_EnemyMagicPositionList[i];
			outputEnemyMagicData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheLandOfBeginningsEnemyPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("enemy", outputEnemyData));
			o_archive(cereal::make_nvp("enemyMagic", outputEnemyMagicData));
		}
	}

	// 玲瓏水源オブジェクト座標
	{
		Vector3Data outputRockData;
		outputRockData.name = "RockPosition"; // 座標
		int arraySize = sizeof(TCF_RockPositionList) / sizeof(TCF_RockPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_RockPositionList[i];
			outputRockData.VectorArray.push_back(vec3);
		}

		Vector3Data outputTreeData;
		outputTreeData.name = "TreePosition"; // 座標
		arraySize = sizeof(TCF_TreePositionList) / sizeof(TCF_TreePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_TreePositionList[i];
			outputTreeData.VectorArray.push_back(vec3);
		}

		Vector3Data outputGroundGrassData;
		outputGroundGrassData.name = "GroundGrassPosition"; // 座標
		arraySize = sizeof(TCF_GroundGrassPositionList) / sizeof(TCF_GroundGrassPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_GroundGrassPositionList[i];
			outputGroundGrassData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheCrystalFountainheadObjectPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("rock", outputRockData));
			o_archive(cereal::make_nvp("tree", outputTreeData));
			o_archive(cereal::make_nvp("groundgrass", outputGroundGrassData));
		}
	}

	// 玲瓏水源エネミー座標
	{
		Vector3Data outputEnemyData;
		outputEnemyData.name = "EnemyPosition"; // 座標
		int arraySize = sizeof(TCF_EnemyPositionList) / sizeof(TCF_EnemyPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_EnemyPositionList[i];
			outputEnemyData.VectorArray.push_back(vec3);
		}

		Vector3Data outputEnemyMagicData;
		outputEnemyMagicData.name = "EnemyMagicPosition"; // 座標
		arraySize = sizeof(TCF_EnemyMagicPositionList) / sizeof(TCF_EnemyMagicPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_EnemyMagicPositionList[i];
			outputEnemyMagicData.VectorArray.push_back(vec3);
		}

		Vector3Data outputEnemyStoneData;
		outputEnemyStoneData.name = "EnemyStonePosition"; // 座標
		arraySize = sizeof(TCF_EnemyStonePositionList) / sizeof(TCF_EnemyStonePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TCF_EnemyStonePositionList[i];
			outputEnemyStoneData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheCrystalFountainheadEnemyPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("enemy", outputEnemyData));
			o_archive(cereal::make_nvp("enemyMagic", outputEnemyMagicData));
			o_archive(cereal::make_nvp("enemyStone", outputEnemyStoneData));
		}
	}

	// 空島オブジェクト座標
	{
		Vector3Data outputRockData;
		outputRockData.name = "RockPosition"; // 座標
		int arraySize = sizeof(TSI_RockPositionList) / sizeof(TSI_RockPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_RockPositionList[i];
			outputRockData.VectorArray.push_back(vec3);
		}

		Vector3Data outputTreeData;
		outputTreeData.name = "TreePosition"; // 座標
		arraySize = sizeof(TSI_TreePositionList) / sizeof(TSI_TreePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_TreePositionList[i];
			outputTreeData.VectorArray.push_back(vec3);
		}

		Vector3Data outputGroundGrassData;
		outputGroundGrassData.name = "GroundGrassPosition"; // 座標
		arraySize = sizeof(TSI_GroundGrassPositionList) / sizeof(TSI_GroundGrassPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_GroundGrassPositionList[i];
			outputGroundGrassData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheSkyIslandObjectPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("rock", outputRockData));
			o_archive(cereal::make_nvp("tree", outputTreeData));
			o_archive(cereal::make_nvp("groundgrass", outputGroundGrassData));
		}
	}

	// 空島エネミー座標
	{
		Vector3Data outputEnemyData;
		outputEnemyData.name = "EnemyPosition"; // 座標
		int arraySize = sizeof(TSI_EnemyPositionList) / sizeof(TSI_EnemyPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_EnemyPositionList[i];
			outputEnemyData.VectorArray.push_back(vec3);
		}

		Vector3Data outputEnemyMagicData;
		outputEnemyMagicData.name = "EnemyMagicPosition"; // 座標
		arraySize = sizeof(TSI_EnemyMagicPositionList) / sizeof(TSI_EnemyMagicPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_EnemyMagicPositionList[i];
			outputEnemyMagicData.VectorArray.push_back(vec3);
		}

		Vector3Data outputEnemyStoneData;
		outputEnemyStoneData.name = "EnemyStonePosition"; // 座標
		arraySize = sizeof(TSI_EnemyStonePositionList) / sizeof(TSI_EnemyStonePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = TSI_EnemyStonePositionList[i];
			outputEnemyStoneData.VectorArray.push_back(vec3);
		}

		std::string filePath = "data/field/TheSkyIslandEnemyPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("enemy", outputEnemyData));
			o_archive(cereal::make_nvp("enemyMagic", outputEnemyMagicData));
			o_archive(cereal::make_nvp("enemyStone", outputEnemyStoneData));
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif
}