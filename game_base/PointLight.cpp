#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "resource.h"
#include "PointLight.h"
#include "camera.h"
#include "input.h"

POINTLIGHTS g_PointLights;

void PointLight::Init()
{
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UseFlag = true;
	m_ModelFlag = false;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	// �|�C���g���C�g������
	for (int i = 0; i < 200; i++)
	{
		g_PointLights.PointLight[i].Position = D3DXVECTOR4((rand() % 100) / 10.0f - 5.0f, (rand() % 50) / 10.0f, (rand() % 100) / 10.0f - 5.0f, 0.0f);
		g_PointLights.PointLight[i].Attenuation = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		g_PointLights.PointLight[i].Color = D3DXCOLOR((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.0f);
	
		m_MoveVector[i] = D3DXVECTOR3((rand() % 100) / 500.0f - 0.1f, (rand() % 50) / 500.0f - 0.05f, (rand() % 100) / 500.0f - 0.1f);
	}
	g_PointLights.PointLight[0].Position = D3DXVECTOR4(1.0f, 2.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * 0.2f;
}

void PointLight::Uninit()
{
}


void PointLight::Update()
{
	for (int i = 0; i < GUI::pointLightNum; i++)
	{
		g_PointLights.PointLight[i].Position += D3DXVECTOR4(m_MoveVector[i], 0.0f);

		if (g_PointLights.PointLight[i].Position.x < -60.0f || g_PointLights.PointLight[i].Position.x > 60.0f)
			m_MoveVector[i].x *= -1;
		if (g_PointLights.PointLight[i].Position.y < 1.0f || g_PointLights.PointLight[i].Position.y > 9.0f)
			m_MoveVector[i].y *= -1;
		if (g_PointLights.PointLight[i].Position.z < -60.0f || g_PointLights.PointLight[i].Position.z > 60.0f)
			m_MoveVector[i].z *= -1;
	}



	// ���f���`��t���O
	m_ModelFlag = GUI::drawLocalLightFlag;
	// �������
	if (GUI::lightType == 0) m_UseFlag = true;
	else m_UseFlag = false;
}


void PointLight::Draw()
{
	if (!m_UseFlag || !m_ModelFlag) return;


	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);


	for (int i = 0; i < GUI::pointLightNum; i++)
	{
		m_Position = D3DXVECTOR3(g_PointLights.PointLight[i].Position.x, g_PointLights.PointLight[i].Position.y, g_PointLights.PointLight[i].Position.z);

		//�����ɃV�F�[�_�[�֘A�̕`�揀����ǉ�
		Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetDeferredGBufferVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetDeferredGBufferPS(), NULL, 0);

		// �}�g���N�X�ݒ�
		D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
		Renderer::SetWorldMatrix(&world);

		// ���f���`��
		Resource::GetSphereModel()->Draw();
	}
}


void PointLight::DrawZPrePass()
{
	if (!m_UseFlag) return;


	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world = Renderer::GetWorldMatrix(m_Scale, m_Rotation, m_Position);
	Renderer::SetWorldMatrix(&world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Resource::GetSphereModel()->Draw();
}

void PointLight::DrawLighting()
{
	if (!m_UseFlag) return;


	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// ���C�g�^�C�v�ݒ�
	LIGHT light;
	light.LightType = POINT_LIGHT;
	Renderer::SetLight(light);
	// �|�C���g���C�g�ݒ�
	g_PointLights.PointLightNum = GUI::pointLightNum;
	Renderer::SetPointLights(g_PointLights);


	//�����ɃV�F�[�_�[�֘A�̕`�揀����ǉ�
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetLocalLightingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetLocalLightingPS(), NULL, 0);


	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	    
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	    
	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* albedoTexture = Renderer::GetAlbedoColorTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &albedoTexture);
	ID3D11ShaderResourceView* positionTexture = Renderer::GetPositionTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &positionTexture);
	ID3D11ShaderResourceView* normalTexture = Renderer::GetNormalTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &normalTexture);
	ID3D11ShaderResourceView* shadowMaskTexture = Renderer::GetMaterialTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &shadowMaskTexture);
	ID3D11ShaderResourceView* directionalLightingTexture = Renderer::GetDirectionalLightingTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, &directionalLightingTexture);


	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}


