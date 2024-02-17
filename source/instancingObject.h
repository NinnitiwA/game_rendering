#pragma once
#include "gameObject.h"


// インスタンス座標
struct INSTANCE_POSITION_DATA
{
	D3DXVECTOR3 Position;
};

// インスタンス数
struct INSTANCE_NUM_DATA
{
	int InstanceNum;
};

// カリング結果
struct CULLING_DATA
{
	float        Rendering;
};

// インデックス付きインスタンス化間接描画の引数
struct D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS {
	UINT IndexCountPerInstance;
	UINT InstanceCount;
	UINT StartIndexLocation;
	INT  BaseVertexLocation;
	UINT StartInstanceLocation;
};



class InstancingObject : public GameObject
{
protected:
	ID3D11Buffer* m_IndirectArgsBuffer{}; // Indirect描画用引数情報

	// インスタンス座標
	ID3D11Buffer* m_InstancePositionBuffer{};              // インスタンスのワールド座標バッファ

	// カリング後座標
	ID3D11Buffer*              m_CullingWriteBuffer{};     // 書き込み用カリングバッファ
	ID3D11UnorderedAccessView* m_CullingResultBufferUAV{}; // シェーダー書き込み用アクセスビュー
	ID3D11Buffer*              m_CullingReadBuffer{};      // 読み込み用カリングバッファ

	// インスタンス数
	ID3D11Buffer*              m_InstanceNumWriteBuffer{}; // 書き込み用インスタンス数バッファ
	ID3D11UnorderedAccessView* m_InstanceNumBufferUAV{};   // シェーダー書き込み用アクセスビュー
	ID3D11Buffer*              m_InstanceNumReadBuffer{};  // 読み込み用インスタンス数バッファ

	// 各種SRV
	ID3D11ShaderResourceView*  m_InstancePositionShaderResourceView{};
	ID3D11ShaderResourceView*  m_CullingShaderResourceView{};
	ID3D11ShaderResourceView*  m_InstanceNumShaderResourceView{};

	int m_ThreadNum = 0;
	int m_InstanceNum = 0;
private:

public:
	virtual void Init() {};
	virtual void Uninit() {};

	void InitInstancePositionData();
	void UninitInstanceData();
};