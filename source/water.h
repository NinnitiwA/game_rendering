#pragma once
#include "gameObject.h"

// ���ʃI�u�W�F�N�g�p�N���X
class Water : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	int							m_numVertex = 0;
	int							m_numIndex = 0;

	int   m_Yoko = 120;     	    // ���̃|���S���̕�����
	int   m_Tate = 120;		        // �c�̃|���S���̕�����
	float m_Takasa = 1.0f;	        // �����_���ō쐬���鍂���̍ő�l
	float m_SizeX = 10.0f;	        // �|���S���̉���
	float m_SizeZ = 10.0f;	        // �|���S���̏c��
	bool  m_isIndependence = false; // �v���C���[�̈ʒu�ɉ����č��W���ω����邩�̎��ʎq

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;

	// �萔�o�b�t�@�p�̔g��񐶐�
	static void CreateWave(void);
	// ���ʐ���
	void CreateWater(bool Flag);
	// ���ʐ����i���ʃT�C�Y�ݒ�\�j
	void CreateWater(D3DXVECTOR3 Pos, int Yoko, int Tate, float Takasa, float SizeX, float SizeZ, bool Flag);
};