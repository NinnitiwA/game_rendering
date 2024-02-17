#include "common.hlsl"

// 生成する頂点の相対座標
static float4 trianglePos[3] = {
  float4(-0.25f, 0.0f,-0.25f, 0.0f),
  float4(0.25f, 0.0f, 0.25f, 0.0f),
  float4(0.0f , 0.8f, 0.0f , 0.0f),
};


[maxvertexcount(6)]
void main(
	point GS_GRASS_IN input[1], //１つの頂点から草を生成
	inout TriangleStream <PS_SHADOWMAP_IN> output
)
{
	PS_SHADOWMAP_IN element = (PS_SHADOWMAP_IN)0;

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);



	// 表面
	// 一つ目の頂点
	float4 ownPos = input[0].Position + float4(input[0].Tangent.x, 0.0f, input[0].Tangent.z, 0.0f);
	element.Position = mul(ownPos, wvp);
	element.WorldPosition = mul(ownPos, World);
	element.Normal = input[0].Normal;
	element.TexCoord = float2(0, 0.99);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 裏面用情報取得
	float4 pos1 = element.Position;
	float4 wpos1 = element.WorldPosition;

	// 頂点生成
	output.Append(element);


	// 二つ目の頂点
	ownPos = input[0].Position - float4(input[0].Tangent.x, 0.0f, input[0].Tangent.z, 0.0f);
	element.Position = mul(ownPos, wvp);
	element.WorldPosition = mul(ownPos, World);
	element.Normal = input[0].Normal;
	element.TexCoord = float2(1, 0.99);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 裏面用情報取得
	float4 pos2 = element.Position;
	float4 wpos2 = element.WorldPosition;

	// 頂点生成
	output.Append(element);


	// 三つ目の頂点
	float4 swayPos = float4(input[0].Tangent.z, 0.0f, input[0].Tangent.x, 0.0f) * sin(input[0].Tangent.y);
	trianglePos[2].y += Grass.z;
	ownPos = input[0].Position + trianglePos[2] + swayPos;
	element.Position = mul(ownPos, wvp);
	element.WorldPosition = mul(ownPos, World);
	element.Normal = input[0].Normal;
	element.TexCoord = float2(1, 0.5);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 裏面用情報取得
	float4 pos3 = element.Position;
	float4 wpos3 = element.WorldPosition;

	// 頂点生成
	output.Append(element);


	output.RestartStrip();// Append関数で生成した頂点を三角形として構成



	// 裏面
	// 一つ目の頂点
	element.Position = pos2;
	element.WorldPosition = wpos2;
	element.Normal = -input[0].Normal;
	element.TexCoord = float2(0, 0.99);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 頂点生成
	output.Append(element);


	// 二つ目の頂点
	element.Position = pos1;
	element.WorldPosition = wpos1;
	element.Normal = -input[0].Normal;
	element.TexCoord = float2(1, 0.99);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 頂点生成
	output.Append(element);


	// 三つ目の頂点
	element.Position = pos3;
	element.WorldPosition = wpos3;
	element.Normal = -input[0].Normal;
	element.TexCoord = float2(1, 0.5);
	element.Diffuse = input[0].Diffuse;

	for (int i = 0; i < 3; i++)
	{
		matrix lightwvp = mul(World, LightVP[i]);
		element.LightViewPosition[i] = mul(ownPos, lightwvp);
	}

	// 頂点生成
	output.Append(element);


	output.RestartStrip();// Append関数で生成した頂点を三角形として構成
}