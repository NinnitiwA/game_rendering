
#include "common.hlsl"



void main(in VS_IN In, out PS_SHADOWMAP_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;

	float4 worldNormal, normal;

	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

	Out.WorldPosition = mul(In.Position, World);

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvpc = mul(World, LightVPC[i]);
		Out.LightViewPosition[i] = mul(In.Position, lightwvpc);
	}
}
