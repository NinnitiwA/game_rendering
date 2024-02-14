#include "main.h"
#include "renderer.h"
#include "resource.h"
#include "instancingObject.h"


void InstancingObject::InitInstancePositionData()
{
	// Indirect -------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC indirectArgsBufferDesc = {};
	indirectArgsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indirectArgsBufferDesc.ByteWidth = sizeof(D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS);
	indirectArgsBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	indirectArgsBufferDesc.CPUAccessFlags = 0;
	indirectArgsBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

	Renderer::GetDevice()->CreateBuffer(&indirectArgsBufferDesc, nullptr, &m_IndirectArgsBuffer);

	D3D11_DRAW_INDEXED_INSTANCED_INDIRECT_ARGS indirectArgs = {};
	indirectArgs.IndexCountPerInstance = 1;  // インスタンスごとのインデックス数
	indirectArgs.InstanceCount = 1;          // カリングされたオブジェクトの数
	indirectArgs.StartIndexLocation = 1;     // インデックスの開始位置
	indirectArgs.BaseVertexLocation = 1;     // ベース頂点の位置
	indirectArgs.StartInstanceLocation = 0;  // インスタンスの開始位置

	Renderer::GetDeviceContext()->UpdateSubresource(m_IndirectArgsBuffer, 0, nullptr, &indirectArgs, 0, 0);


}

void InstancingObject::UninitInstanceData()
{
	m_IndirectArgsBuffer->Release();
	m_InstancePositionBuffer->Release();
	m_InstancePositionShaderResourceView->Release();
	m_InstanceNumReadBuffer->Release();
	m_InstanceNumWriteBuffer->Release();
	m_InstanceNumBufferUAV->Release();
	m_InstanceNumShaderResourceView->Release();
	m_CullingReadBuffer->Release();
	m_CullingWriteBuffer->Release();
	m_CullingResultBufferUAV->Release();
	m_CullingShaderResourceView->Release();
}
