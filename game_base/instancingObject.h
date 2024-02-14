#pragma once
#include "gameObject.h"


// �C���X�^���X���W
struct INSTANCE_POSITION_DATA
{
	D3DXVECTOR3 Position;
};

// �C���X�^���X��
struct INSTANCE_NUM_DATA
{
	int InstanceNum;
};

// �J�����O����
struct CULLING_DATA
{
	float        Rendering;
};

// �C���f�b�N�X�t���C���X�^���X���Ԑڕ`��̈���
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
	ID3D11Buffer* m_IndirectArgsBuffer{}; // Indirect�`��p�������

	// �C���X�^���X���W
	ID3D11Buffer* m_InstancePositionBuffer{};              // �C���X�^���X�̃��[���h���W�o�b�t�@

	// �J�����O����W
	ID3D11Buffer*              m_CullingWriteBuffer{};     // �������ݗp�J�����O�o�b�t�@
	ID3D11UnorderedAccessView* m_CullingResultBufferUAV{}; // �V�F�[�_�[�������ݗp�A�N�Z�X�r���[
	ID3D11Buffer*              m_CullingReadBuffer{};      // �ǂݍ��ݗp�J�����O�o�b�t�@

	// �C���X�^���X��
	ID3D11Buffer*              m_InstanceNumWriteBuffer{}; // �������ݗp�C���X�^���X���o�b�t�@
	ID3D11UnorderedAccessView* m_InstanceNumBufferUAV{};   // �V�F�[�_�[�������ݗp�A�N�Z�X�r���[
	ID3D11Buffer*              m_InstanceNumReadBuffer{};  // �ǂݍ��ݗp�C���X�^���X���o�b�t�@

	// �e��SRV
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