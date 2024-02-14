#include "common.hlsl"

Texture2D g_AlbedoTexture : register(t0);
Texture2D g_PositionTexture : register(t1);
Texture2D g_NormalTexture : register(t2);
Texture2D g_MaterialTexture  : register(t3);
Texture2D g_EnvMapTexture  : register(t4);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outColor : SV_TARGET0)
{
	// GBufferより各種サンプリング
	float4 albedo = g_AlbedoTexture.Sample(g_SamplerState, In.TexCoord);
	float4 worldPosition = g_PositionTexture.Sample(g_SamplerState, In.TexCoord);
	float4 normal = g_NormalTexture.Sample(g_SamplerState, In.TexCoord);
	float4 pbr = g_MaterialTexture.Sample(g_SamplerState, In.TexCoord);

	float roughness = saturate(pbr.x + 0.0001);
	float isPBR = pbr.w;
	float metalic = pbr.z;
	float shadow = pbr.y;
	float3 eyev = normalize(Camera.Position.xyz - worldPosition.xyz);
	float3 refv = normalize(reflect(eyev, normal.xyz));


	// 環境マップ取得
	float2 envTexCoord;
	envTexCoord = -refv * 0.3f + 0.5f;
	float4 envColor = g_EnvMapTexture.SampleBias(g_SamplerState, envTexCoord, 0.0f);

	// 環境光
	float3 ambient = float3(0.1, 0.1, 0.1);


	// 拡散反射---------------------------------------------diffuse	
	// フレネル反射を考慮した拡散反射を計算
	float fresnelDiffuse = CalcDisneyDiffuse(normal.xyz, -Light.Direction.xyz, eyev, roughness, metalic);

	// 最終的な拡散反射を計算
	float3 diffuse = albedo.rgb * saturate(dot(normal, -Light.Direction.xyz)) * (1.0 - fresnelDiffuse) / PI;


	// 鏡面反射---------------------------------------------specular
	// 鏡面反射率を計算
	float3 specular = CalcCookTorranceSpecular(-Light.Direction.xyz, eyev, normal.xyz, albedo.rgb, roughness, metalic);

	// 拡散反射光と鏡面反射光を合成
	float k = pow((1.0 - roughness), 5.0) * pow((metalic), 2.0);
	float3 lig = diffuse * (1.0 - metalic) + specular * metalic + ambient + envColor * k;

	outColor = float4(lig, 1.0f);

	

	// シャドウ作成---------------------------------------------
	outColor.xyz *= shadow;
}
