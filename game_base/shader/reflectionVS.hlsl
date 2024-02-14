
#include "common.hlsl"


void main(in VS_IN In, out PS_REFLECT_IN Out)
{
	In.Position.w = 1.0f;

	// Gerstner波で頂点を移動
	float4 position = In.Position;
	position.xyz += CalcWaveOffset(position.xyz);

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(position, World);



	// WVP変換した頂点座標を出力
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(position, wvp);


	//UV値
	Out.TexCoord = In.TexCoord;

	//頂点の法線をワールド行列で変換する
	float4 worldNormal, normal;

	//Normal
	normal = float4(In.Normal.xyz, 0.0);
	//法線をワールド変換する
	worldNormal = mul(normal, World);
	//ワールド変換した法線を正規化する（拡大縮小の情報を削除する）
	Out.Normal = normalize(worldNormal);

	//Tangent
	normal = float4(In.Tangent.xyz, 0.0);
	//法線をワールド変換する
	worldNormal = mul(normal, World);
	//ワールド変換した法線を正規化する（拡大縮小の情報を削除する）
	Out.Tangent = normalize(worldNormal);

	//Binormal
	normal = float4(In.Binormal.xyz, 0.0);
	//法線をワールド変換する
	worldNormal = mul(normal, World);
	//ワールド変換した法線を正規化
	Out.Binormal = normalize(worldNormal);



	// 鏡面反射した座標を設定
	matrix reflectionProjectionWorld;
	reflectionProjectionWorld = mul(World, Camera.Reflection);
	reflectionProjectionWorld = mul(reflectionProjectionWorld, Projection);
	Out.ReflectPosition = mul(In.Position, reflectionProjectionWorld);
}
