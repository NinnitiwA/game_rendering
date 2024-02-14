#include"common.hlsl"

Texture2D g_Texture: register(t0);
SamplerState g_SamplerState: register(s0);

void main(in PS_IN In, out float4 outDiffuse: SV_Target)
{

	// �P�x���̌v�Z
	float4 baseColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float2 texOffset = 1.5 / float2(Param.TexWidth, Param.TexHeight);

	// �אڃs�N�Z�����T���v�����O
	float3 rgbNW = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-texOffset.x, -texOffset.y)).rgb;
	float3 rgbNE = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(texOffset.x, -texOffset.y)).rgb;
	float3 rgbSW = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-texOffset.x, texOffset.y)).rgb;
	float3 rgbSE = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(texOffset.x, texOffset.y)).rgb;

	// �P�x�̌v�Z
	float3 luma = float3(0.299, 0.587, 0.114);
	float baseLuma = dot(baseColor.rgb, luma);
	float3 lumaNW = dot(rgbNW, luma);
	float3 lumaNE = dot(rgbNE, luma);
	float3 lumaSW = dot(rgbSW, luma);
	float3 lumaSE = dot(rgbSE, luma);

	// �G�b�W���o
	float3 minLuma = min(min(baseLuma, min(lumaNW, lumaNE)), min(lumaSW, lumaSE));
	float3 maxLuma = max(max(baseLuma, max(lumaNW, lumaNE)), max(lumaSW, lumaSE));

	float3 delta = maxLuma - minLuma;
	float4 finalColor = baseColor;

	// �G�b�W���o����
	float edgeThreshold = Param.Level.x;

	// �P�x���ɂ��G�b�W���o
	if (max(delta.x, max(delta.y, delta.z)) > edgeThreshold) {
		// �u�����f�B���O�ɂ��G�b�W���
		float2 dir = normalize(float2(baseLuma - 0.5, lumaNE.x - lumaNW.x));
		float2 offset = dir * texOffset * Param.Level.y;

		float4 leftSample = g_Texture.Sample(g_SamplerState, In.TexCoord - offset);
		float4 rightSample = g_Texture.Sample(g_SamplerState, In.TexCoord + offset);

		// �u�����f�B���O
		finalColor.rgb = 0.5 * (leftSample.rgb + rightSample.rgb);

		// �G�b�W�`�惂�[�h
		if (Param.Level.z > 0.5) {
			outDiffuse = float4(1.0, finalColor.gb, 1.0);
			return;
		}
	}

	outDiffuse = float4(finalColor.rgb, 1.0);

}