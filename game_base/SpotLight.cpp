#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "resource.h"
#include "SpotLight.h"
#include "camera.h"
#include "input.h"

SPOTLIGHT g_SpotLight;

void SpotLight::Init()
{
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UseFlag = false;
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


	// 頂点バッファ生成
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


	// ポイントライト初期化
	g_SpotLight.Position = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	g_SpotLight.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 50.0f);
	g_SpotLight.Attenuation = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 8.0f);
	g_SpotLight.Color = D3DXCOLOR((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.0f);

	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * 0.2f;
}

void SpotLight::Uninit()
{
}


void SpotLight::Update()
{
	if (Input::GetKeyPress('A'))
		g_SpotLight.Position.x -= 0.1f;


	if (Input::GetKeyPress('D'))
		g_SpotLight.Position.x += 0.1f;


	if (Input::GetKeyPress('W'))
		g_SpotLight.Position.z += 0.1f;


	if (Input::GetKeyPress('S'))
		g_SpotLight.Position.z -= 0.1f;


	if (Input::GetKeyPress('Q'))
		g_SpotLight.Position.y += 0.1f;


	if (Input::GetKeyPress('E'))
		g_SpotLight.Position.y -= 0.1f;



	// モデル描画フラグ
	m_ModelFlag = GUI::drawFlag;
	// 光源種類
	if (GUI::lightType == 0) m_UseFlag = false;
	else m_UseFlag = true;
}


void SpotLight::Draw()
{
	if (!m_UseFlag || !m_ModelFlag) return;


	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);


	m_Position = D3DXVECTOR3(g_SpotLight.Position);

	//ここにシェーダー関連の描画準備を追加
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetLightObjectGBufferVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetLightObjectGBufferPS(),NULL, 0);

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	Resource::GetSphereModel()->Draw();
}

void SpotLight::DrawZPrePass()
{
	if (!m_UseFlag) return;


	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Resource::GetSphereModel()->Draw();
}

void SpotLight::DrawLighting()
{
	if (!m_UseFlag) return;


	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// ライトタイプ設定
	LIGHT light;
	light.LightType = SPOT_LIGHT;
	Renderer::SetLight(light);
	// スポットライト設定
	Renderer::SetSpotLight(g_SpotLight);


	//ここにシェーダー関連の描画準備を追加
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetLocalLightingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetLocalLightingPS(), NULL, 0);


	// マトリクス設定
	Renderer::SetWorldViewProjection2D();


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
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


	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}


