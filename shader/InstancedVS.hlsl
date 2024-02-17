#include "common.hlsl"

// 座標
struct InstancedParam {
	float3 Position;
};
// インスタンス数
struct InstanceNum {
	int InstanceNum;
};

StructuredBuffer<InstancedParam> WorldPositionArgs : register(t0);

StructuredBuffer<InstancedParam> gCullingResultBuffer : register(t1); // カリング結果を格納するバッファ
StructuredBuffer<InstanceNum> gInstanceNumBuffer : register(t2);      // インスタンス数を格納するバッファ


void main(in VS_IN In, out PS_IN Out)
{
	// インスタンスの座標取得
	uint index = In.InstanceId;

	In.Position.xyz += WorldPositionArgs[index].Position;

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);


	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
}

