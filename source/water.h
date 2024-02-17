#pragma once
#include "gameObject.h"

// 水面オブジェクト用クラス
class Water : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	int							m_numVertex = 0;
	int							m_numIndex = 0;

	int   m_Yoko = 120;     	    // 横のポリゴンの分割数
	int   m_Tate = 120;		        // 縦のポリゴンの分割数
	float m_Takasa = 1.0f;	        // ランダムで作成する高さの最大値
	float m_SizeX = 10.0f;	        // ポリゴンの横幅
	float m_SizeZ = 10.0f;	        // ポリゴンの縦幅
	bool  m_isIndependence = false; // プレイヤーの位置に応じて座標が変化するかの識別子

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;

	// 定数バッファ用の波情報生成
	static void CreateWave(void);
	// 水面生成
	void CreateWater(bool Flag);
	// 水面生成（水面サイズ設定可能）
	void CreateWater(D3DXVECTOR3 Pos, int Yoko, int Tate, float Takasa, float SizeX, float SizeZ, bool Flag);
};