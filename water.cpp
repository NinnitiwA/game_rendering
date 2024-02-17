#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "water.h"
#include "random.h"


void Water::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Water::Uninit()
{
	GameObject::Uninit();
}


void Water::Update()
{
	if (!m_isIndependence)
	{
		Scene* scene = Manager::GetScene();
		Player* player = scene->GetGameObject<Player>();

		m_Position = player->GetPosition();
		m_Position.z += 20.0f;
		m_Position.y = 0.0f;
	}
}


void Water::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	Renderer::GetDeviceContext()->VSSetShader(Resource::GetReflectionVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetReflectionPS(), NULL, 0);


	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metalic = 0.02;
	material.Roughness = 0.3;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* reflectionTexture = Renderer::GetReflectionTexture();
	//�����_�����O�e�N�X�`����1�ԂɃZ�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &reflectionTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed(m_numIndex, 0, 0);

}

void Water::DrawZPrePass()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());

	Renderer::GetDeviceContext()->VSSetShader(Resource::GetReflectionVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetReflectionPS(), NULL, 0);


	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Metalic = 0.02;
	material.Roughness = 0.3;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* reflectionTexture = Renderer::GetReflectionTexture();
	//�����_�����O�e�N�X�`����1�ԂɃZ�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &reflectionTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed(m_numIndex, 0, 0);
}

void Water::CreateWave(void)
{
	// �g�̏��𐶐����萔�o�b�t�@�Ɋi�[
	WAVES waves;
	for (int i = 0; i < 20; i++)
	{
		WAVE& wave = waves.Wave[i];
		float randomRad = (float)(Random(0, 360) * D3DX_PI * 2 * 0.15f);
		wave.dir.x = sinf(randomRad);
		wave.dir.y = cosf(randomRad);
		wave.amplitude = 0.03f + powf(2.0f, (float)Random(0, 1) * 2.0f) * 0.045f;
		wave.waveLength = 1.0f + powf(2.f, 1.f + (float)Random(0, 2)) * 1.5f;
	}
	Renderer::SetWaves(waves);
}

void Water::CreateWater(bool Flag)
{
	// ���ʎq�ݒ�
	m_isIndependence = Flag;

	// �|���S������
	int yoko = m_Yoko;       // ���̃|���S���̕�����
	int tate = m_Tate;       // �c�̃|���S���̕�����
	float takasa = m_Takasa; // �����_���ō쐬���鍂���̍ő�l
	float sizeX = m_SizeX;   // �|���S���̉���
	float sizeZ = m_SizeZ;   // �|���S���̏c��

	// �K�v�Ȓ��_�����v�Z����
	m_numVertex = (yoko + 1) * (tate + 1);
	m_numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * m_numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int y = 0; y < (tate + 1); y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				int i = y * (yoko + 1) + x;

				float height = ((float)rand() / RAND_MAX) * takasa;
				// ���_���W�̐ݒ�
				pVtx[i].Position = D3DXVECTOR3(-(yoko / 2.0f) * sizeX + x * sizeX, 0.0f, (tate / 2.0f) * sizeZ - y * sizeZ);
				// UV�l�̐ݒ�
				pVtx[i].TexCoord = D3DXVECTOR2(x, y);

				D3DXVECTOR3 vUP(0.0f, 0.0f, 1.0f); // �A�b�v�x�N�g��
				// �@���̐ݒ�
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �ڃx�N�g���̐ݒ�
				D3DXVec3Cross(&pVtx[i].Tangent, &pVtx[i].Normal, &vUP);

				// �]�@���̐ݒ�
				D3DXVec3Cross(&pVtx[i].Binormal, &pVtx[i].Normal, &pVtx[i].Tangent);

				// ���_�J���[�̐ݒ�
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		int idx = 0;

		for (int y = 0; y < tate; y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				pIdx[idx] = (yoko + 1) + x + (yoko + 1) * y;
				idx++;
				pIdx[idx] = 0 + x + (yoko + 1) * y;
				idx++;
			}

			if (y < (tate - 1))
			{
				pIdx[idx] = yoko + (yoko + 1) * y;
				idx++;
				pIdx[idx] = (yoko + 1) * 2 + (yoko + 1) * y;
				idx++;
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);
	}
}

void Water::CreateWater(D3DXVECTOR3 Pos, int Yoko, int Tate, float Takasa, float SizeX, float SizeZ, bool Flag)
{
	// �p�����[�^�ݒ�
	m_Position = Pos;

	m_Yoko = Yoko;
	m_Tate = Tate;
	m_Takasa = Takasa;
	m_SizeX = SizeX;
	m_SizeZ = SizeZ;

	m_isIndependence = Flag;


	// �|���S������
	int yoko = m_Yoko;       // ���̃|���S���̕�����
	int tate = m_Tate;       // �c�̃|���S���̕�����
	float takasa = m_Takasa; // �����_���ō쐬���鍂���̍ő�l
	float sizeX = m_SizeX;   // �|���S���̉���
	float sizeZ = m_SizeZ;   // �|���S���̏c��

	// �K�v�Ȓ��_�����v�Z����
	m_numVertex = (yoko + 1) * (tate + 1);
	m_numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * m_numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		for (int y = 0; y < (tate + 1); y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				int i = y * (yoko + 1) + x;

				float height = ((float)rand() / RAND_MAX) * takasa;
				// ���_���W�̐ݒ�
				pVtx[i].Position = D3DXVECTOR3(-(yoko / 2.0f) * sizeX + x * sizeX, 0.0f, (tate / 2.0f) * sizeZ - y * sizeZ);
				// UV�l�̐ݒ�
				pVtx[i].TexCoord = D3DXVECTOR2(x, y);

				D3DXVECTOR3 vUP(0.0f, 0.0f, 1.0f);
				// �@���̐ݒ�
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �ڃx�N�g���̐ݒ�
				D3DXVec3Cross(&pVtx[i].Tangent, &pVtx[i].Normal, &vUP);

				// �]�@���̐ݒ�
				D3DXVec3Cross(&pVtx[i].Binormal, &pVtx[i].Normal, &pVtx[i].Tangent);

				// ���_�J���[�̐ݒ�
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		int idx = 0;

		for (int y = 0; y < tate; y++)
		{
			for (int x = 0; x < (yoko + 1); x++)
			{
				pIdx[idx] = (yoko + 1) + x + (yoko + 1) * y;
				idx++;
				pIdx[idx] = 0 + x + (yoko + 1) * y;
				idx++;
			}

			if (y < (tate - 1))
			{
				pIdx[idx] = yoko + (yoko + 1) * y;
				idx++;
				pIdx[idx] = (yoko + 1) * 2 + (yoko + 1) * y;
				idx++;
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);
	}
}
