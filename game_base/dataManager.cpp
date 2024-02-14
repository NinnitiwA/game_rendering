#include "main.h"
#include "dataManager.h"
#include "dataStorage.h"


void LoadData()
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


	// フィールドオブジェクト座標
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("rock", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetFieldDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("tree", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetFieldDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // 座標

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("groundgrass", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetFieldDataStorage()->GroundGrassPos.push_back(inputData.VectorArray[i]);
	}



//#ifdef _MSC_VER
//	system("pause");
//#endif
}


void SaveData()
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

		std::string filePath = "data/field/FieldObjectPos.json";

		std::ofstream ofs(filePath);
		{
			cereal::JSONOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp("rock", outputRockData));
			o_archive(cereal::make_nvp("tree", outputTreeData));
			o_archive(cereal::make_nvp("groundgrass", outputGroundGrassData));
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif
}