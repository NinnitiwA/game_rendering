
#include "common.hlsl"

Texture2D g_TextureReflection : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_REFLECT_IN In, out PS_DEFERRED_OUT Out)
{
	// 融合率
	float fusionRate = 0.45f;

	//// 法線マッピング
	//// このピクセルの法線マップのデータRGBA値を取得
	//float4 normalMap = g_TextureBamp.Sample(g_SamplerState, In.TexCoord);
	//// 取得したRGBA値をベクトル情報XYZWとして展開する
	//normalMap = (normalMap * 2.0f) - 1.0f;

	//float4 n = normalize(In.Normal);
	//float4 t = normalize(In.Tangent);
	//float4 b = normalize(In.Binormal);
	//float4 normal;
	//normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
	//normal.w = 0.0;
	//normal = normalize(normal);

	//// 光源の向きと回転済みの法線とで内積を計算し、符号を反転して輝度を求める
	//float light = -dot(Light.Direction.xyz, normal.xyz);
	//// 飽和演算(0.0～1.0の値にクランプする)
	//light = saturate(light);


	// 鏡面反射
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 waterColor;

	waterColor = float4(0.032, 0.422, 0.8, 1.0);

	reflectTexCoord.x = In.ReflectPosition.x / In.ReflectPosition.w / 2.05f + 0.5025f;
	reflectTexCoord.y = -In.ReflectPosition.y / In.ReflectPosition.w / 2.05f + 0.5025f;
	// テクスチャから色をサンプリングする
	reflectionColor = g_TextureReflection.Sample(g_SamplerState, reflectTexCoord);
	// マテリアルの透過率に応じてテクスチャの色に輝度を乗算する
	//reflectionColor.rgb = (fusionRate > 0.1) ? reflectionColor.rgb * light : reflectionColor.rgb;


	// デバッグ用
	//if (reflectTexCoord.x < 0.0 || reflectTexCoord.y < 0.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);
	//if (reflectTexCoord.x > 1.0 || reflectTexCoord.y > 1.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);


	// 合成
	float4 finalColor;
	finalColor = lerp(waterColor, reflectionColor, (1.0f - fusionRate));

	//// ブリンフォン鏡面反射
	//// 視線ベクトルを作成する（カメラからピクセルへ向かうベクトル）
	//float3 evev = In.WorldPosition.xyz - Camera.Position.xyz;
	//evev = normalize(evev);
	//// ハーフベクトルを作成する（視線ベクトル＋ライトベクトル）
	//float3 halfv = evev + Light.Direction.xyz;
	//halfv = normalize(halfv);

	//float specular = -dot(halfv, normal.xyz);
	//saturate(specular);
	//specular = pow(specular, 20);
	//finalColor.rgb = saturate(finalColor.rgb + specular);

	//// フレネル反射率を反映
	//float dotNormal = dot(evev, In.Normal);
	//float fresnel = 0.05 + (1 - 0.05) * pow(1 - dotNormal, 10);
	//finalColor.a = fresnel;

	Out.Albedo = finalColor;
	Out.Position = In.Position;
	Out.Normal = float4(0.0, 1.0, 0.0, 1.0);
	Out.Material = float4(Material.Roughness, 1.0, Material.Metalic, 1.0);
}
