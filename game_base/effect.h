#pragma once
#include "gameObject.h"
#include "string.h"

// �G�t�F�N�g���
enum EffectType
{
	EFFECT_EXPLOSION, // ����
	EFFECT_IMPULSE,   // �Ռ��g

	EFFECT_NON,
};

// �G�t�F�N�g���
struct EffectInfo
{
	char m_TextureName[256];  // �e�N�X�`���t�@�C����
	EffectType m_EffectType;  // �G�t�F�N�g�^�C�v

	int m_DivideX;            // �c�̕�����
	int m_DivideY;            // ���̕�����
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

	int m_Frame; // �G�t�F�N�g�A�j���[�V�����t���[��

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �G�t�F�N�g�������ɌĂяo��
	void SetEffect(D3DXVECTOR3 pos, EffectType effectType);
};
