#include "common.hlsl"

// �������钸�_�̑��΍��W
static float4 trianglePos[3] = {
  float4(-0.25f, 0.0f,-0.25f, 0.0f),
  float4(0.25f, 0.0f, 0.25f, 0.0f),
  float4(0.0f , 0.8f, 0.0f , 0.0f),
};


[maxvertexcount(6)]
void main(
	point GS_GRASS_IN input[1], //�P�̒��_���瑐�𐶐�
	inout TriangleStream <PS_SHADOWMAP_IN> output
)
{
	PS_SHADOWMAP_IN element = (PS_SHADOWMAP_IN)0;

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);



	// �\��
	// ��ڂ̒��_
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

	// ���ʗp���擾
	float4 pos1 = element.Position;
	float4 wpos1 = element.WorldPosition;

	// ���_����
	output.Append(element);


	// ��ڂ̒��_
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

	// ���ʗp���擾
	float4 pos2 = element.Position;
	float4 wpos2 = element.WorldPosition;

	// ���_����
	output.Append(element);


	// �O�ڂ̒��_
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

	// ���ʗp���擾
	float4 pos3 = element.Position;
	float4 wpos3 = element.WorldPosition;

	// ���_����
	output.Append(element);


	output.RestartStrip();// Append�֐��Ő����������_���O�p�`�Ƃ��č\��



	// ����
	// ��ڂ̒��_
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

	// ���_����
	output.Append(element);


	// ��ڂ̒��_
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

	// ���_����
	output.Append(element);


	// �O�ڂ̒��_
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

	// ���_����
	output.Append(element);


	output.RestartStrip();// Append�֐��Ő����������_���O�p�`�Ƃ��č\��
}