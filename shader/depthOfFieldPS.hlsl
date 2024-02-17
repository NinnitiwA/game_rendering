#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DepthTexture : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	// �[�x�e�N�X�`������[�x�l���擾
	float4 depthColor = g_DepthTexture.Sample(g_SamplerState, In.TexCoord);
	float level = Param.Depth.z; // �ڂ����x��


	// �f�o�b�O�\��-----------------------------------------------------------------
	if (Param.Depth.w > 0.5f)
	{
		if (depthColor.r >= Param.Depth.y)      // ��
		{
			outDiffuse = float4(0.0f, 1.0f, 0.0f, 1.0f);
			return;
		}
		else if (depthColor.r <= Param.Depth.x) // ��O
		{
			outDiffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
			return;
		}
		else                                    // �ʏ펞
		{
			float4 ColorDefault = g_Texture.Sample(g_SamplerState, In.TexCoord);
			outDiffuse = ColorDefault;
			return;
		}
	}
	// -----------------------------------------------------------------------------


	// �ڂ��pUV�l�ݒ�
	if (depthColor.r >= Param.Depth.y) level = level * ((1.0f - Param.Depth.y) / 0.35f); // ��
	else if (depthColor.r <= Param.Depth.x) level = level * (1.730f - depthColor.r);     // ��O
	// �ʏ펞
	else 
	{
		float4 ColorDefault = g_Texture.Sample(g_SamplerState, In.TexCoord);
		outDiffuse = ColorDefault;
		return;
	}


	// �ڂ���
	float offsetX = level / Param.TexWidth;
	float offsetY = level / Param.TexHeight;
	float2 TCNW = In.TexCoord + float2(-offsetX, -offsetY);
	float2 TCNE = In.TexCoord + float2(offsetX, -offsetY);
	float2 TCSW = In.TexCoord + float2(-offsetX, offsetY);
	float2 TCSE = In.TexCoord + float2(offsetX, offsetY);

	// �אڃs�N�Z�����T���v�����O
	float4 ColorNW = g_Texture.Sample(g_SamplerState, TCNW);
	float4 ColorNE = g_Texture.Sample(g_SamplerState, TCNE);
	float4 ColorSW = g_Texture.Sample(g_SamplerState, TCSW);
	float4 ColorSE = g_Texture.Sample(g_SamplerState, TCSE);

	// �u�����f�B���O
	float4 FinalColor = (ColorNW + ColorNE + ColorSW + ColorSE) / 4.0f;

	outDiffuse = FinalColor;
}
