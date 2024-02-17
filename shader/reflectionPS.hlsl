#include "common.hlsl"

Texture2D g_TextureReflection : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_REFLECT_IN In, out PS_DEFERRED_OUT Out)
{
	// 融合率
	float fusionRate = 0.45f;

	// 鏡面反射
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 waterColor;

	waterColor = float4(0.032, 0.422, 0.8, 1.0);

	reflectTexCoord.x = In.ReflectPosition.x / In.ReflectPosition.w / 2.05f + 0.5025f;
	reflectTexCoord.y = -In.ReflectPosition.y / In.ReflectPosition.w / 2.05f + 0.5025f;
	// テクスチャから色をサンプリングする
	reflectionColor = g_TextureReflection.Sample(g_SamplerState, reflectTexCoord);

	// デバッグ用
	//if (reflectTexCoord.x < 0.0 || reflectTexCoord.y < 0.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);
	//if (reflectTexCoord.x > 1.0 || reflectTexCoord.y > 1.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);

	// 合成
	float4 finalColor;
	finalColor = lerp(waterColor, reflectionColor, (1.0f - fusionRate));

	Out.Albedo = finalColor;
	Out.Position = In.Position;
	Out.Normal = float4(0.0, 1.0, 0.0, 1.0);
	Out.Material = float4(Material.Roughness, 1.0, Material.Metalic, 1.0);
}
