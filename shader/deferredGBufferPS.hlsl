#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DepthNearTexture : register(t1);
Texture2D g_DepthMiddleTexture : register(t2);
Texture2D g_DepthFarTexture : register(t3);
SamplerState g_SamplerState : register(s0);


void main(in PS_SHADOWMAP_IN In, out PS_DEFERRED_OUT Out)
{
	Out = (PS_DEFERRED_OUT)0;


	matrix wvp; 
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 color;
	color = g_Texture.Sample(g_SamplerState, In.TexCoord);
	color.a = 1.0f;
	Out.Albedo = color;

	float4 position;
	position = In.WorldPosition;
	position.a = 1.0f;
	Out.Position = position;

	float4 normal;
	normal = float4(normalize(In.Normal.rgb), 1.0f);
	Out.Normal = normal;



	float4 pbr = float4(1.0, 1.0, 1.0, 1.0);

	// Roughness
	pbr.x = Material.Roughness;
	// Metalic
	pbr.z = Material.Metalic;
	// Shininess
	pbr.w += Material.Shininess;


	// シャドウマッピング
	Texture2D<float4> shadowMapArray[3];
	shadowMapArray[0] = g_DepthNearTexture;
	shadowMapArray[1] = g_DepthMiddleTexture;
	shadowMapArray[2] = g_DepthFarTexture;
	bool cascaded = false; // カスケード区画描画済み識別子（範囲内の中で一番手前のカスケードのみ判定）
	for (int i = 0; i < 3; i++)
	{
		// ライトビュースクリーン空間でのZ値を計算
		In.LightViewPosition[i].xyz /= In.LightViewPosition[i].w; // 正規化デバイス座標に変換

		if (In.LightViewPosition[i].z >= 0 && In.LightViewPosition[i].z <= 1.0f)
		{
			//テクスチャ座標に変換
			In.LightViewPosition[i].x = In.LightViewPosition[i].x * 0.5 + 0.5;  
			In.LightViewPosition[i].y = -In.LightViewPosition[i].y * 0.5 + 0.5;

			// シャドウマップがUV範囲内かどうか
			if (In.LightViewPosition[i].x >= 0.0 && In.LightViewPosition[i].x <= 1.0 &&
				In.LightViewPosition[i].y >= 0.0 && In.LightViewPosition[i].y <= 1.0)
			{
				// カスケード区画描画（デバッグ用）
				if (Light.Ambient.w > 0.5f && !cascaded)
				{
					float4 cascade = float4(1.0f * (1 - i), i, i * i - i, 1.0f);
					Out.Albedo.rgb = cascade.rgb;
					cascaded = true;
				}


				// シャドウマップからサンプリング
				float depth = shadowMapArray[i].Sample(g_SamplerState, In.LightViewPosition[i].xy).r;

				if (depth < In.LightViewPosition[i].z - 0.0005f) //0.005はZファイティング補正値
				{
					pbr.y = 0.5f;
				}


				// シャドウマップ範囲内の場合マテリアル情報を格納して終了
				Out.Material = pbr;
				return;
			}
		}
	}
	Out.Material = pbr;
}
