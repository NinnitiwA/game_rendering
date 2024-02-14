
#include "main.h"
#include "renderer.h"
#include "polygon.h"
#include "input.h"
#include "resource.h"
#include <iostream>
#include <cmath>

enum DEBUG
{
	COLOR,
	POSITION,
	NORMAL,
	LIGHTVIEWDEPTH,
	SHADOWMASK,
	DIRECTIONALLIGHT,
	LOCALLIGHT
};

void Polygon2D::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );

	vertex[1].Position = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2( 1.0f, 0.0f );

	vertex[2].Position = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, 1.0f );

	vertex[3].Position = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2( 1.0f, 1.0f );


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );

}


void Polygon2D::Uninit()
{
}


void Polygon2D::Update()
{
}


void Polygon2D::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetUnlitTextureVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetUnlitTexturePS(), NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	Renderer::SetMaterial( material );


	// デバッグ用
	DEBUG debug = (DEBUG)(GUI::textureType);
	{
		ID3D11ShaderResourceView* texture;
		//レンダリングテクスチャを取得
		switch (debug)
		{
		case COLOR:
			texture = Renderer::GetAlbedoColorTexture();
			break;
		case POSITION:
			texture = Renderer::GetPositionTexture();
			break;
		case NORMAL:
			texture = Renderer::GetNormalTexture();
			break;
		case LIGHTVIEWDEPTH:
			texture = Renderer::GetShadowMapTexture(0);
			break;
		case SHADOWMASK:
			texture = Renderer::GetMaterialTexture();
			break;
		case DIRECTIONALLIGHT:
		case LOCALLIGHT:
			texture = Renderer::GetAntiAliasingTexture();
			break;
		}
		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw( 4, 0 );

}


void Polygon2D::DrawDirectionalLighting()
{
	//ここにシェーダー関連の描画準備を追加
	Renderer::GetDeviceContext()->IASetInputLayout(Resource::GetVertexLayout());
	Renderer::GetDeviceContext()->VSSetShader(Resource::GetDirectionalLightingVS(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(Resource::GetDirectionalLightingPS(), NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);


	// テクスチャ設定
	ID3D11ShaderResourceView* albedoTexture = Renderer::GetAlbedoColorTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &albedoTexture);
	ID3D11ShaderResourceView* positionTexture = Renderer::GetPositionTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &positionTexture);
	ID3D11ShaderResourceView* normalTexture = Renderer::GetNormalTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &normalTexture);
	ID3D11ShaderResourceView* shadowMaskTexture = Renderer::GetMaterialTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &shadowMaskTexture);
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, Resource::GetEnvironmentMapTexture());


	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
