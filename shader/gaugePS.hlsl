#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = float4(0.0f, 0.0f, 0.0f, Material.Diffuse.a);

	float max = PlayerParam.Status.x;
	float current = PlayerParam.Status.y + PlayerParam.Status.w;
	float old = PlayerParam.Status.z;

	float gauge = In.TexCoord * max;
	outDiffuse.rgb += Material.Diffuse.rgb * (1.0f - step(current, gauge));   // 現在の数値
	outDiffuse.rgb += float3(1.0f, 0.0f, 0.0f) * step(current, gauge);        // 最大の数値
	outDiffuse.rgb += float3(-0.5f, 0.5f, 0.5f) * step(current + old, gauge); // 減少している数値
}
