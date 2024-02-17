#pragma once
#include "scene.h"

// �t�B�[���h�^�C�v
enum FIELDTYPE
{
	FIELD_GROUNDING, // �t�B�[���h���ڒn�i�������n�ʂ��琅�ʂ܂Łj
	FIELD_FLOATING,  // �t�B�[���h�����V�i������6.0f�ŌŒ�j
};

// �Q�[�����X�e�[�W����ъe�G���A�𓝊�����N���X
class Area
{
private:
	static FIELDTYPE m_FieldType;

public:
	// �t�B�[���h�^�C�v�ݒ�
	static void SetFieldType(FIELDTYPE FieldType) { m_FieldType = FieldType; }

	void InitEnvironment();
	void InitEnemy();
};