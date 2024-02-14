#pragma once
#include "gameObject.h"
#include "string.h"

// エフェクト種類
enum EffectType
{
	EFFECT_EXPLOSION, // 爆発
	EFFECT_IMPULSE,   // 衝撃波

	EFFECT_NON,
};

// エフェクト情報
struct EffectInfo
{
	char m_TextureName[256];  // テクスチャファイル名
	EffectType m_EffectType;  // エフェクトタイプ

	int m_DivideX;            // 縦の分割数
	int m_DivideY;            // 横の分割数
};


class Effect : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	ID3D11VertexShader*	m_DepthVertexShader{};
	ID3D11PixelShader*	m_DepthPixelShader{};

	EffectInfo* m_EffectInfo{};

	int m_Frame; // エフェクトアニメーションフレーム

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// エフェクト生成時に呼び出す
	void SetEffect(D3DXVECTOR3 pos, EffectType effectType);
};
