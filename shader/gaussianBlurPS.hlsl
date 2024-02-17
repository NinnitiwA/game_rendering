#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float4 bloom = 0.0; // ブルーム強度

	float textureWidth = Param.TexWidth;
	float textureHeight = Param.TexHeight;

	 // ブラー処理
	[unroll]
	for (int i = -6; i <= 6; ++i)
	{
		float offset = (float)i;
		bloom += g_Texture.Sample(g_SamplerState, In.TexCoord + float2(offset / textureWidth, offset / (textureHeight * 2)));
	}
	bloom /= float(2 * 6 + 1);

	outDiffuse.rgb += bloom;
}