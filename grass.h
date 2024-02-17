#pragma once
#include "gameObject.h"

class Grass : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	int			  m_numVertex{};

	D3DXVECTOR4 m_GrassParameter;  // x : 揺れ値, y : 揺れの方向, z : 草の高さ
	float       m_GrassInterval;   // 草の間隔
	int         m_GrassNumber;     // 各草原オブジェクト固有の数

	static D3DXVECTOR4 m_Corner;   // 端に位置する草原オブジェクト生成時に利用する変数 x : Y+, y : Y-, z : X+, w : X-
	static int         m_GrassNum; // 草原オブジェクト生成時の草の行列数
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
	Scene    シーン
	Num      範囲内に生成する草の数
	X        生成する草原行数
	Y        生成する草原列数
	Position 生成する草原の中心位置
	Corner   生成時に端を考慮するかどうか  x : +Y方向 y : -Y方向 z : +X方向 w : -X方向
	Height   生成する草の高さ
	*/
	static void CreateGrass(Scene* Scene, int Num, int X, int Y, 
		D3DXVECTOR3 Position, D3DXVECTOR4 Corner, float Height);
};