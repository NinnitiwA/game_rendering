#include"common.hlsl"

Texture2D g_Texture: register(t0);
Texture2D g_WorldPositionTexture: register(t1);
Texture2D g_NormalTexture: register(t2);
Texture2D g_DepthTexture: register(t3);
SamplerState g_SamplerState: register(s0);

void main(in PS_IN In, out float4 outDiffuse: SV_Target)
{
	float4 albedo = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 worldPosition = g_WorldPositionTexture.Sample(g_SamplerState, In.TexCoord);
	float4 normal = g_NormalTexture.Sample(g_SamplerState, In.TexCoord);
	float depth = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
	
	//テクスチャから色をサンプリングする
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);


	// 高度フォグ
	if (Fog.HeightColor.a > 0.5)
	{
		float offset = 10.0f;
		float fogHeight = Fog.HeightLevel.x; // フォグが発生する縦幅
		float fogCenter = Fog.HeightLevel.y; // 基準となる高さ

		float fog = 1.0f;
		fog = (worldPosition.y + offset) / (fogCenter + fogHeight + 0.0001 + offset); 
		fog = saturate(fog);

		outDiffuse.rgb = outDiffuse.rgb * fog + Fog.HeightColor.rgb * (1.0f - fog);
	}


	// 深度フォグ
	if (Fog.DepthColor.a > 0.5)
	{
		float fogCenter = Fog.DepthLevel.x; // 基準となる位置
		float fogWidth = Fog.DepthLevel.y;  // フォグが発生する幅

		float dist = distance(worldPosition.xyz, Camera.Position.xyz);
		float fog = (fogCenter - (fogWidth / 2.0) - dist) / fogCenter; 
		fog = saturate(fog);
		outDiffuse.rgb = outDiffuse.rgb * fog + Fog.DepthColor.rgb * (1.0 - fog);
	}

}