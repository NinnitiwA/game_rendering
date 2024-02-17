#pragma once


// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};





class Model
{
protected:
	bool m_UseModelTexture = true;
private:

	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	SUBSET*	m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );
	void CalcTangentAndBinormal(D3DXVECTOR3* p0, D3DXVECTOR2* uv0, D3DXVECTOR3* p1, D3DXVECTOR2* uv1, D3DXVECTOR3* p2, D3DXVECTOR2* uv2, D3DXVECTOR3* outTangent, D3DXVECTOR3* outBinormal);

public:
	void Draw();
	void Draw(MATERIAL material);
	void DrawInstancedIndirect(int instanceNum, ID3D11Buffer* indirectBuffer);


	void Load( const char *FileName );
	void Load( const char* FileName, bool use);
	void Unload();

};