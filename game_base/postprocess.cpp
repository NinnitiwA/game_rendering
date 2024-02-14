#include "main.h"
#include "renderer.h"
#include "postprocess.h"
#include "input.h"
#include "resource.h"
#include <iostream>
#include <cmath>


void PostProcess::Init()
{
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

}


void PostProcess::Uninit()
{
	//Release��NULL�`�F�b�N������
	if (m_VertexBuffer != NULL)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}
}


void PostProcess::Update()
{

}


void PostProcess::DrawDepthOfField()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// ��ʊE�[�x��K�p����ꍇ
	if (GUI::depthOfField && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetDepthOfFieldVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetDepthOfFieldPS(), NULL, 0);

		// �e�N�X�`���ݒ�
		ID3D11ShaderResourceView* texture;
		if (GUI::textureType == 5) texture = Renderer::GetDirectionalLightingTexture();
		else if (GUI::textureType == 6) texture = Renderer::GetLocalLightingTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
		ID3D11ShaderResourceView* depthTexture = Renderer::GetDepthTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &depthTexture);
	}
	// ��ʊE�[�x��K�p���Ȃ��ꍇ
	else 
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

		// �e�N�X�`���ݒ�
		ID3D11ShaderResourceView* depthTexture = Renderer::GetTextureByNumber(GUI::textureType);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &depthTexture);
	}



	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �p�����[�^�ݒ�
	m_Param.depth = GUI::depthParameter;
	m_Param.texWidth = SCREEN_WIDTH;
	m_Param.texHeight = SCREEN_HEIGHT;
	Renderer::SetPostProcessParameter(m_Param);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void PostProcess::DrawFog()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// �t�H�O��K�p����ꍇ
	if (GUI::fog && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetFogVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetFogPS(), NULL, 0);
	
		// �e�N�X�`���ݒ�
		ID3D11ShaderResourceView* worldPositionTexture = Renderer::GetPositionTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &worldPositionTexture);
		ID3D11ShaderResourceView* normalTexture = Renderer::GetNormalTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &normalTexture);
		ID3D11ShaderResourceView* depthTexture = Renderer::GetDepthTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &depthTexture);
	
	}
	// �t�H�O��K�p���Ȃ��ꍇ
	else
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	}


	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* texture = Renderer::GetDepthOfFieldTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);


	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �p�����[�^�ݒ�
	FOG fog = GUI::fogParameter;
	Renderer::SetFog(fog);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void PostProcess::DrawLuminance()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// �u���[����K�p����ꍇ
	if (GUI::bloom && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetLuminanceVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetLuminancePS(), NULL, 0);
	}
	// �u���[����K�p���Ȃ��ꍇ
	else
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	}


	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �e�N�X�`���[�T�C�Y�ݒ�
	POSTPROCESSPARAMETER param;
	param.level = D3DXVECTOR4(1.0f, 1.0f, 1.0f, GUI::luminanceValue);
	param.texWidth = SCREEN_WIDTH / 2;
	param.texHeight = SCREEN_HEIGHT / 2;
	Renderer::SetPostProcessParameter(param);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* texture = Renderer::GetFogTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	ID3D11ShaderResourceView* materialTexture = Renderer::GetMaterialTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &materialTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void PostProcess::DrawBlooms(int num)
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// �u���[����K�p����ꍇ
	if (GUI::bloom && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetGaussianBlurVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetGaussianBlurPS(), NULL, 0);
	}
	// �u���[����K�p���Ȃ��ꍇ
	else
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	}



	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �e�N�X�`���[�T�C�Y�ݒ�
	POSTPROCESSPARAMETER param;
	param.texWidth = SCREEN_WIDTH / (num + 2);
	param.texHeight = SCREEN_HEIGHT / (num + 2);
	Renderer::SetPostProcessParameter(param);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* texture;
	if(num == 0)texture = Renderer::GetLuminanceTexture();
	else texture = Renderer::GetBloomsTexture(num - 1);
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}


void PostProcess::DrawBloom()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// �u���[����K�p����ꍇ
	if (GUI::bloom && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetBloomVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetBloomPS(), NULL, 0);
	}
	// �u���[����K�p���Ȃ��ꍇ
	else
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	}



	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* texture = Renderer::GetFogTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	ID3D11ShaderResourceView* lminanceTexture = Renderer::GetLuminanceTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &lminanceTexture);
	for (int i = 0; i < 4; i++)
	{
		ID3D11ShaderResourceView* bloomTexture = Renderer::GetBloomsTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(i + 2, 1, &bloomTexture);
	}

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void PostProcess::DrawAntiAliasing()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());


	// �A���`�G�C���A�V���O��K�p����ꍇ
	if (GUI::antiAliasing && GUI::textureType >= 5)
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetAntiAliasingFXAAVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetAntiAliasingFXAAPS(), NULL, 0);

		ID3D11ShaderResourceView* depthTexture = Renderer::GetDepthTexture();
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &depthTexture);
	}
	// �A���`�G�C���A�V���O��K�p���Ȃ��ꍇ
	else
	{
		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);
	}



	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �p�����[�^�ݒ�
	m_Param.color = (D3DXVECTOR4)GUI::PBRObjectColor;
	m_Param.level = D3DXVECTOR4(GUI::antiAliasingBias, GUI::UVOffset, GUI::drawEdgeFlag, 1.0f);
	m_Param.depth = GUI::depthParameter;
	m_Param.texWidth = SCREEN_WIDTH;
	m_Param.texHeight = SCREEN_HEIGHT;
	Renderer::SetPostProcessParameter(m_Param);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* texture = Renderer::GetBloomTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
