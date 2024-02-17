#include "common.hlsl"

Texture2D g_AlbedoTexture : register(t0);
Texture2D g_PositionTexture : register(t1);
Texture2D g_NormalTexture : register(t2);
Texture2D g_MaterialTexture  : register(t3);
Texture2D g_DirectionalLightingTexture  : register(t4);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outColor : SV_TARGET0)
{
	float2 uv = In.WorldPosition.xy / float2(Param.TexWidth, Param.TexHeight);
	
    // Gバッファから情報を取得
	float3 albedo = g_AlbedoTexture.Sample(g_SamplerState, uv).xyz;
	float4 worldPosition = g_PositionTexture.Sample(g_SamplerState, uv);
    float4 normal = g_NormalTexture.Sample(g_SamplerState, uv);
	float4 pbr = g_MaterialTexture.Sample(g_SamplerState, In.TexCoord);

	float4 diffuse = g_DirectionalLightingTexture.Sample(g_SamplerState, uv);

	float roughness = pbr.x;
	float metalic = pbr.z;
	float3 eyev = normalize(Camera.Position.xyz - worldPosition.xyz);
	float3 refv = normalize(reflect(eyev, normal.xyz));

	// ポイントライト************************************************************
	if (Light.LightType == 1)
	{
		for (int i = 0; i < PointLightNum; i++)
		{
			// 光源座標
			float3 pointLightPos = mul(PointLight[i].Position.xyz, World);

			// 環境光
			float4 ambient = 1.0f;

			// 拡散反射
			float3 lightDir = pointLightPos - worldPosition.xyz;
			float distance = length(lightDir);
			lightDir = normalize(lightDir);


			// 影響率
			float affect = 1.0f - (1.0f / 3.0f) * distance;
			if (affect < 0.0f) affect = 0.0f;

			affect = pow(affect, 3.0f);


			if (distance < 3.0f)
			{
				// 鏡面反射
				lightDir = saturate(lightDir);
				float NL = saturate(dot(normal, lightDir));
				float3 refv = normalize(2 * NL * normal - lightDir);
				float3 eyev = normalize(worldPosition.xyz - Camera.Position.xyz);
				float4 specular = pow(saturate(dot(refv, eyev)), 4) * 1.0f;
				// フォンモデル最終色　３つの項の合計
				float4 color = ambient + diffuse + specular;
				color *= PointLight[i].Color;

				diffuse = color * affect + diffuse * (1.0f - affect);
				diffuse.a = 1.0f;
			}

		}
	}


	// スポットライト************************************************************
	else if (Light.LightType == 2)
	{
		// 光源座標と方向
		float3 spotLightPos = mul(SpotLight.Position.xyz, World);
		float3 spotLightDir = normalize(SpotLight.Direction.xyz);

		// スポットライトのカットオフ角度と外向きのスポットライト半径
		float spotLightCosCutoff = 0.95f;
		float spotLightOuterRadius = tan(30.0f * 0.5);

		// 環境光
		float4 ambient = 1.0f;

		// 拡散反射
		float3 lightDir = spotLightPos - worldPosition.xyz;
		float distance = length(lightDir);
		lightDir = normalize(lightDir);

		// スポットライトの影響範囲内にあるかどうかを確認
		float spotEffect = saturate((dot(-lightDir, spotLightDir) - spotLightCosCutoff) / (1.0 - spotLightCosCutoff));

		{ 
			// 鏡面反射
			lightDir = saturate(lightDir);
			float NL = saturate(dot(normal, lightDir));
			float3 refv = normalize(2 * NL * normal - lightDir);
			float3 eyev = normalize(worldPosition.xyz - Camera.Position.xyz);
			float4 specular = pow(saturate(dot(refv, eyev)), 4) * 1.0f;

			// フォンモデル最終色　３つの項の合計
			float4 color = ambient + diffuse + specular;
			color *= SpotLight.Color;

			diffuse = color * spotEffect + diffuse * (1.0f - spotEffect);
			diffuse.a = 1.0f;
		}
	}

	// 最終出力
	outColor = float4(diffuse.rgb, 1.0f);
}
