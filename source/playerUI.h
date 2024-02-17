#pragma once
#include "UIObject.h"

struct PLAYERSTATUS
{
	D3DXVECTOR4 HP;     // x : 最大HP, y : 現在のHP, z : 減少分のHP, w : 
	D3DXVECTOR4 Status; // x : 最大スタミナ, y : 現在のスタミナ, z : 減少分のスタミナ, w : 現在のスタミナのかけら
};

// プレイヤーステータス用のUIクラス
class PlayerUI : public UIObject
{
private:
	ID3D11Buffer* m_PlayerUIVertexBuffer{};

	PLAYERSTATUS  m_PlayerStatus;
	static int    m_Count; // プレイヤーステータスの変動がない時間カウント
	static float  m_Alpha; // マテリアルのα値
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR4 GetPlayerStatus() { return m_PlayerStatus.Status; }
	void SetPlayerStatus(D3DXVECTOR4 ps) { m_PlayerStatus.Status += ps; }

	static void ResetStatusUICount(void)
	{
		m_Count = 0;
		m_Alpha = 1.0f;
	}
};