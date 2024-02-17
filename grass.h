#pragma once
#include "gameObject.h"

class Grass : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	int			  m_numVertex{};

	D3DXVECTOR4 m_GrassParameter;  // x : �h��l, y : �h��̕���, z : ���̍���
	float       m_GrassInterval;   // ���̊Ԋu
	int         m_GrassNumber;     // �e�����I�u�W�F�N�g�ŗL�̐�

	static D3DXVECTOR4 m_Corner;   // �[�Ɉʒu���鑐���I�u�W�F�N�g�������ɗ��p����ϐ� x : Y+, y : Y-, z : X+, w : X-
	static int         m_GrassNum; // �����I�u�W�F�N�g�������̑��̍s��
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;

	static void SetGrassCorner(D3DXVECTOR4 Corner) { m_Corner = Corner; }
	static void SetGrassNum(int Num) { Grass::m_GrassNum = Num; }
	void SetGrassHeight(float height) { m_GrassParameter.z = height; }


	/*
	Scene    �V�[��
	Num      �͈͓��ɐ������鑐�̐�
	X        �������鑐���s��
	Y        �������鑐����
	Position �������鑐���̒��S�ʒu
	Corner   �������ɒ[���l�����邩�ǂ���  x : +Y���� y : -Y���� z : +X���� w : -X����
	Height   �������鑐�̍���
	*/
	static void CreateGrass(Scene* Scene, int Num, int X, int Y, 
		D3DXVECTOR3 Position, D3DXVECTOR4 Corner, float Height);
};