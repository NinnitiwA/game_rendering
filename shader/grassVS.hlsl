#include "common.hlsl"

void main(in VS_IN In, out GS_GRASS_IN Out)
{
	Out.Position = In.Position;

	matrix wv = mul(World, View);
	Out.ViewPosition = mul(In.Position, wv);

	Out.TexCoord = In.TexCoord;
	Out.Binormal = In.Binormal;
	Out.Tangent = In.Tangent;
	Out.Tangent.y += Grass.x; // ëêå¥ÇÃóhÇÍïùê›íË

	// ñ@ê¸ÇÃê›íË
	float3 pos1 = In.Position.xyz + float3(In.Tangent.z, 0.0, In.Tangent.x) * sin(In.Tangent.y) + float3(0.0, Grass.z + 0.8f, 0.0);
	float3 pos2 = In.Position.xyz + float3(In.Tangent.x, 0.0, In.Tangent.z);
	float3 pos3 = In.Position.xyz - float3(In.Tangent.x, 0.0, In.Tangent.z);
	float3 vec1 = pos2 - pos1;
	float3 vec2 = pos3 - pos1;
	float3 normal = normalize(cross(vec1, vec2));
	Out.Normal = float4(normal, 1.0);

	Out.Diffuse = In.Diffuse;
}