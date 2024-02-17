#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DepthNearTexture : register(t1);
Texture2D g_DepthMiddleTexture : register(t2);
Texture2D g_DepthFarTexture : register(t3);
SamplerState g_SamplerState : register(s0);


void main(in PS_SHADOWMAP_IN In, out float4 outDiffuse : SV_TARGET)
{
	float4 pbr = float4(0.0, 0.0, 0.0, 1.0);
	outDiffuse = float4(0.0, 0.0, 0.0, 1.0);


	Texture2D<float4> shadowMapArray[3];
	shadowMapArray[0] = g_DepthNearTexture;
	shadowMapArray[1] = g_DepthMiddleTexture;
	shadowMapArray[2] = g_DepthFarTexture;
	for (int i = 0; i < 3; i++)
	{
		// ライトビュースクリーン空間でのZ値を計算
		In.LightViewPosition[i].xyz /= In.LightViewPosition[i].w;               //正規化デバイス座標に変換

		if (In.LightViewPosition[i].z >= 0 && In.LightViewPosition[i].z <= 1.0f)
		{
			In.LightViewPosition[i].x = In.LightViewPosition[i].x * 0.5 + 0.5;  //テクスチャ座標に変換
			In.LightViewPosition[i].y = -In.LightViewPosition[i].y * 0.5 + 0.5;

			// シャドウマップがUV範囲内かどうか
			if (In.LightViewPosition[i].x >= 0.0 && In.LightViewPosition[i].x <= 1.0 &&
				In.LightViewPosition[i].y >= 0.0 && In.LightViewPosition[i].y <= 1.0)
			{
				pbr.xyz = float3(1.0f * (1 - i), i, i * i - i);
				outDiffuse = pbr;
				return;
			}
		}
	}
	outDiffuse = pbr;
}
