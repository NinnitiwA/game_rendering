
#include "common.hlsl"


void main(in VS_IN In, out PS_REFLECT_IN Out)
{
	In.Position.w = 1.0f;

	// Gerstner�g�Œ��_���ړ�
	float4 position = In.Position;
	position.xyz += CalcWaveOffset(position.xyz);

	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(position, World);



	// WVP�ϊ��������_���W���o��
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(position, wvp);


	//UV�l
	Out.TexCoord = In.TexCoord;

	//���_�̖@�������[���h�s��ŕϊ�����
	float4 worldNormal, normal;

	//Normal
	normal = float4(In.Normal.xyz, 0.0);
	//�@�������[���h�ϊ�����
	worldNormal = mul(normal, World);
	//���[���h�ϊ������@���𐳋K������i�g��k���̏����폜����j
	Out.Normal = normalize(worldNormal);

	//Tangent
	normal = float4(In.Tangent.xyz, 0.0);
	//�@�������[���h�ϊ�����
	worldNormal = mul(normal, World);
	//���[���h�ϊ������@���𐳋K������i�g��k���̏����폜����j
	Out.Tangent = normalize(worldNormal);

	//Binormal
	normal = float4(In.Binormal.xyz, 0.0);
	//�@�������[���h�ϊ�����
	worldNormal = mul(normal, World);
	//���[���h�ϊ������@���𐳋K��
	Out.Binormal = normalize(worldNormal);



	// ���ʔ��˂������W��ݒ�
	matrix reflectionProjectionWorld;
	reflectionProjectionWorld = mul(World, Camera.Reflection);
	reflectionProjectionWorld = mul(reflectionProjectionWorld, Projection);
	Out.ReflectPosition = mul(In.Position, reflectionProjectionWorld);
}
