#include "main.h"
#include "dataManager.h"
#include "dataStorage.h"


void LoadData()
{
	// �v���C���[
    // �ړ��A�j���[�V����
	{
		std::string filePath = "data/player/PlayerWalkAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) 
			DataStorage::GetPlayerDataStorage()->WalkAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/player/PlayerWalkAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->WalkAnimScale.push_back(inputData.VectorArray[i]);
	}

	// �W�����v�A�j���[�V����
	{
		std::string filePath = "data/player/PlayerJumpAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->JumpAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/player/PlayerJumpAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->JumpAnimScale.push_back(inputData.VectorArray[i]);
	}

	// �v���X�U���A�j���[�V����
	{
		std::string filePath = "data/player/PlayerPressAttackAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->PressAttackAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/player/PlayerPressAttackAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->PressAttackAnimScale.push_back(inputData.VectorArray[i]);
	}

	// �_�b�V���U���A�j���[�V����
	{
		std::string filePath = "data/player/PlayerDashAttackAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->DashAttackAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/player/PlayerDashAttackAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->DashAttackAnimScale.push_back(inputData.VectorArray[i]);
	}

	// ���@�r���A�j���[�V����
	{
		std::string filePath = "data/player/PlayerMagicAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->MagicAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/player/PlayerMagicAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetPlayerDataStorage()->MagicAnimScale.push_back(inputData.VectorArray[i]);
	}



	// �G�l�~�[
    // �ړ��A�j���[�V����
	{
		std::string filePath = "data/enemyField/EnemyFieldWalkAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->WalkAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/enemyField/EnemyFieldWalkAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->WalkAnimScale.push_back(inputData.VectorArray[i]);
	}

	// �U���A�j���[�V����
	{
		std::string filePath = "data/enemyField/EnemyFieldAttackAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->AttackAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/enemyField/EnemyFieldAttackAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->AttackAnimScale.push_back(inputData.VectorArray[i]);
	}

	// ���@�U���A�j���[�V����
	{
		std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/enemyField/EnemyFieldMagicAttackAnimScale.json"; // �X�P�[��

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("root", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetEnemyFieldDataStorage()->MagicAttackAnimScale.push_back(inputData.VectorArray[i]);
	}


	// �t�B�[���h�I�u�W�F�N�g���W
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("rock", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetFieldDataStorage()->RockPos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // ���W

		Vector3Data inputData;
		std::ifstream ifs(filePath);
		cereal::JSONInputArchive i_archive(ifs);
		i_archive(cereal::make_nvp("tree", inputData));

		int arraySize = inputData.VectorArray.size();
		for (int i = 0; i < arraySize; i++) DataStorage::GetFieldDataStorage()->TreePos.push_back(inputData.VectorArray[i]);
	}
	{
		std::string filePath = "data/field/FieldObjectPos.json"; // ���W

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
	// �v���C���[
	// �ړ��A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "PlayerWalkAnimPos"; // ���W
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
		outputData.name = "PlayerWalkAnimScale"; // �X�P�[��
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

	// �W�����v�A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "PlayerJumpAnimPos"; // ���W
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
		outputData.name = "PlayerJumpAnimScale"; // �X�P�[��
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

	// �v���X�U���A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "PlayerPressAttackAnimPos"; // ���W
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
		outputData.name = "PlayerPressAttackAnimScale"; // �X�P�[��
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

	// �_�b�V���U���A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "PlayerDashAttackAnimPos"; // ���W
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
		outputData.name = "PlayerDashAttackAnimScale"; // �X�P�[��
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

	// ���@�r���A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "PlayerMagicAnimPos"; // ���W
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
		outputData.name = "PlayerMagicAnimScale"; // �X�P�[��
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


	// �G�l�~�[
	// �ړ��A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "EnemyFieldWalkAnimPos"; // ���W
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
		outputData.name = "EnemyFieldWalkAnimScale"; // �X�P�[��
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

	// �U���A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "EnemyFieldAttackAnimPos"; // ���W
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
		outputData.name = "EnemyFieldAttackAnimScale"; // �X�P�[��
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

	// ���@�U���A�j���[�V����
	{
		Vector3Data outputData;
		outputData.name = "EnemyFieldMagicAttackAnimPos"; // ���W
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
		outputData.name = "EnemyFieldMagicAttackAnimScale"; // �X�P�[��
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

	// �t�B�[���h�I�u�W�F�N�g���W
	{
		Vector3Data outputRockData;
		outputRockData.name = "RockPosition"; // ���W
		int arraySize = sizeof(m_RockPositionList) / sizeof(m_RockPositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = m_RockPositionList[i];
			outputRockData.VectorArray.push_back(vec3);
		}

		Vector3Data outputTreeData;
		outputTreeData.name = "TreePosition"; // ���W
		arraySize = sizeof(m_TreePositionList) / sizeof(m_TreePositionList[0]);
		for (int i = 0; i < arraySize; i++) {
			D3DXVECTOR3 vec3 = m_TreePositionList[i];
			outputTreeData.VectorArray.push_back(vec3);
		}

		Vector3Data outputGroundGrassData;
		outputGroundGrassData.name = "GroundGrassPosition"; // ���W
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