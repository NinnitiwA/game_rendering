#pragma once
#include "gameObject.h"

class Tree : public GameObject
{
private:
	D3DXVECTOR3 m_BaseScale; // �����ݒ莞�̃X�P�[��

	bool m_isSway = false; // ���I�u�W�F�N�g�̏Փˎ��̗h�ꎯ�ʎq
	int  m_SwayFrame = 0;  // �h��t���[��
	int  m_TreeType;       // �؃��f���̎��

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawShadowMapping() override;
	void DrawZPrePass() override;
	void DrawReflection() override;

	void SetIsSway() { m_isSway = true; }
};