#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "grass.h"
#include "random.h"

#define GRASS_RADIUS 0.35f // ���̃����_�����W�͈͔̔��a

int Grass::m_GrassNum = 0;
D3DXVECTOR4 Grass::m_Corner(0.0f, 0.0f, 0.0f, 0.0f);

void Grass::Init()
{
	// �p�����[�^������
	m_Scale = D3DXVECTOR3(20.0f, 4.0f, 20.0f);
	m_GrassParameter = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
	m_GrassInterval = 40.0f / m_GrassNum;
	m_GrassNumber = m_GrassNum * m_GrassNum;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_GrassNumber;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int y = 0 + (int)m_Corner.y; y < m_GrassNum - (int)m_Corner.x; y++)
		{
			for (int x = 0 + (int)m_Corner.w; x < m_GrassNum - (int)m_Corner.z; x++)
			{
				// ���_���W�̐ݒ�
				D3DXVECTOR3 pos;
				pos.x = -(m_GrassNum * m_GrassInterval / 2.0f) + m_GrassInterval * x;
				pos.y = 0.0f;
				pos.z = -(m_GrassNum * m_GrassInterval / 2.0f) + m_GrassInterval * y;
				pVtx[x + m_GrassNum * y].Position = pos;

				// �����_�����W�̐ݒ�
				float random = Random(0, 360);
				pos.x = GRASS_RADIUS * cosf((random * D3DX_PI / 180.0f));
				pos.y = sinf((Random(0, 360) * D3DX_PI / 360.0f));
				pos.z = GRASS_RADIUS * sinf((random * D3DX_PI / 180.0f));
				pVtx[x + m_GrassNum * y].Tangent = pos; // x : �����_�����W, y : �����̗h��, z : �����_�����W

				// �h��̕���
				pVtx[x + m_GrassNum * y].Binormal.y = 1.0f;

				// ���_�J���[�̐ݒ�
				pVtx[x + m_GrassNum * y].Diffuse = D3DXVECTOR4(0.0f, 0.8f, 0.0f, 1.0f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}
}


void Grass::Uninit()
{
	m_VertexBuffer->Release();
}


void Grass::Update()
{
	// �����̗h��ݒ� 
	if (m_GrassParameter.x >= D3DX_PI * 2) m_GrassParameter.x = 0.0f;
	m_GrassParameter.x += 0.02f;
}


void Grass::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetGrassVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetGrassVS(), NULL, 0);
	Renderer::GetDeviceContext()->GSSetShader(Resource::GetGrassGS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metalic = 0.028f;
	material.Roughness = 0.002f;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// Grass�p�����[�^�ݒ�
	Renderer::SetGrass(m_GrassParameter);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Resource::GetGrassTexture());

	// �e�N�X�`���ݒ�
	for (int i = 0; i < SHADOWMAP_NUM; i++)
	{
		ID3D11ShaderResourceView* texture = Renderer::GetShadowMapTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(1 + i, 1, &texture);
	}

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(m_GrassNumber, 0);

	// GS�ď�����
	Renderer::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}


void Grass::DrawZPrePass()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetGrassVertexLayout());

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetGrassVS(), NULL, 0);
	Renderer::GetDeviceContext()->GSSetShader(Resource::GetGrassGS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetGrassPS(), NULL, 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// Grass�p�����[�^�ݒ�
	Renderer::SetGrass(m_GrassParameter);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(m_GrassNumber, 0);

	// GS�ď�����
	Renderer::GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}

void Grass::CreateGrass(Scene* Scene, int Num, int X, int Y, D3DXVECTOR3 Position, D3DXVECTOR4 Corner, float Height)
{
	// ���̐���ݒ�
	Grass::SetGrassNum(Num);
	D3DXVECTOR4 corner = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 basePosition = D3DXVECTOR3((X - 1) / 2.0f, 0.0f, (Y - 1) / 2.0f);

	// ������
	for (int y = 0; y < Y; y++)
	{
		// �����̏㉺�[�̏ꍇ�̏�����ݒ�
		if (y == Y - 1 && Corner.x > 1.0f) corner.x = Corner.x;
		else corner.x = 0.0f;
		if (y == 0 && Corner.y > 1.0f) corner.y = Corner.y;
		else corner.y = 0.0f;

		for (int x = 0; x < X; x++)
		{
			// �����̍��E�[�̏ꍇ�̏�����ݒ�
			if (x == X - 1 && Corner.z > 1.0f) corner.z = Corner.z;
			else corner.z = 0.0f;
			if (x == 0 && Corner.w > 1.0f) corner.w = Corner.w;
			else corner.w = 0.0f;
			Grass::SetGrassCorner(corner);

			Grass* grass = Scene->AddGameObject<Grass>(1);
			grass->SetPosition(D3DXVECTOR3(Position.x - 40.0f * (X - 1) / 2.0f + x * 40.0f, Position.y, Position.z - 40.0f * (Y - 1) / 2.0f + y * 40.0f));
			grass->SetGrassHeight(Height);
		}
	}
}

