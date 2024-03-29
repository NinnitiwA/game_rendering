#include "common.hlsl"

Texture2D g_AlbedoTexture : register(t0);
Texture2D g_PositionTexture : register(t1);
Texture2D g_NormalTexture : register(t2);
Texture2D g_MaterialTexture  : register(t3);
Texture2D g_EnvMapTexture  : register(t4);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outColor : SV_TARGET0)
{
	// GBufferζθeνTvO
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


	// Β«}bvζΎ
	float2 envTexCoord;
	envTexCoord = -refv * 0.3f + 0.5f;
	float4 envColor = g_EnvMapTexture.SampleBias(g_SamplerState, envTexCoord, 0.0f);

	// Β«υ
	float3 ambient = float3(0.1, 0.1, 0.1);


	// gU½Λ---------------------------------------------diffuse	
	// tl½ΛπlΆ΅½gU½ΛπvZ
	float fresnelDiffuse = CalcDisneyDiffuse(normal.xyz, -Light.Direction.xyz, eyev, roughness, metalic);

	// ΕIIΘgU½ΛπvZ
	float3 diffuse = albedo.rgb * saturate(dot(normal, -Light.Direction.xyz)) * (1.0 - fresnelDiffuse) / PI;


	// ΎΚ½Λ---------------------------------------------specular
	// ΎΚ½Λ¦πvZ
	float3 specular = CalcCookTorranceSpecular(-Light.Direction.xyz, eyev, normal.xyz, albedo.rgb, roughness, metalic);

	// gU½ΛυΖΎΚ½Λυπ¬
	float k = pow((1.0 - roughness), 5.0) * pow((metalic), 2.0);
	float3 lig = diffuse * (1.0 - metalic) + specular * metalic + ambient + envColor * k;

	outColor = float4(lig, 1.0f);

	

	// VhEμ¬---------------------------------------------
	outColor.xyz *= shadow;
}
