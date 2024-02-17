#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_MaterialTexture : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float shininess = g_MaterialTexture.Sample(g_SamplerState, In.TexCoord).a;
	outDiffuse *= In.Diffuse;

	//グレースケール変換
	float gray = dot(outDiffuse.rgb, float3(0.299, 0.587, 0.114)) * shininess;
	clip(gray - Param.Level.w);
}