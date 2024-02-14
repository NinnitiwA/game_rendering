#include"common.hlsl"

Texture2D g_Texture: register(t0);
Texture2D g_BloomTexture01: register(t1);
Texture2D g_BloomTexture02: register(t2);
Texture2D g_BloomTexture03: register(t3);
Texture2D g_BloomTexture04: register(t4);
Texture2D g_BloomTexture05: register(t5);
SamplerState g_SamplerState: register(s0);

void main(in PS_IN In, out float4 outDiffuse: SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	float4 bloom = 0.0;
	bloom += g_BloomTexture01.Sample(g_SamplerState, In.TexCoord) * 0.4;
	bloom += g_BloomTexture02.Sample(g_SamplerState, In.TexCoord) * 0.3;
	bloom += g_BloomTexture03.Sample(g_SamplerState, In.TexCoord) * 0.25;
	bloom += g_BloomTexture04.Sample(g_SamplerState, In.TexCoord) * 0.2;
	bloom += g_BloomTexture05.Sample(g_SamplerState, In.TexCoord) * 0.15;
	outDiffuse.rgb += bloom.rgb;
}