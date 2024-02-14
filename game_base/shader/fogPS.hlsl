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
	
	//�e�N�X�`������F���T���v�����O����
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);


	// ���x�t�H�O
	if (Fog.HeightColor.a > 0.5)
	{
		float offset = 10.0f;
		float fogHeight = Fog.HeightLevel.x; // �t�H�O����������c��
		float fogCenter = Fog.HeightLevel.y; // ��ƂȂ鍂��

		float fog = 1.0f;
		fog = (worldPosition.y + offset) / (fogCenter + fogHeight + 0.0001 + offset); 
		fog = saturate(fog);

		outDiffuse.rgb = outDiffuse.rgb * fog + Fog.HeightColor.rgb * (1.0f - fog);
	}


	// �[�x�t�H�O
	if (Fog.DepthColor.a > 0.5)
	{
		float fogCenter = Fog.DepthLevel.x; // ��ƂȂ�ʒu
		float fogWidth = Fog.DepthLevel.y;  // �t�H�O���������镝

		float dist = distance(worldPosition.xyz, Camera.Position.xyz);
		float fog = (fogCenter - (fogWidth / 2.0) - dist) / fogCenter; 
		fog = saturate(fog);
		outDiffuse.rgb = outDiffuse.rgb * fog + Fog.DepthColor.rgb * (1.0 - fog);
	}

}