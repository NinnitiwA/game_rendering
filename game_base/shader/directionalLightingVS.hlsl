
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;//matrix�^�̕ϐ�wvp���`����
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	//���͂��ꂽ���_���W��ϊ����ďo�͐�֑������
	Out.Position = mul(In.Position, wvp);

	//���͂��ꂽUV���W���o�͐�֑������
	Out.TexCoord = In.TexCoord;

	//���_�̖@�������[���h�s��ŕϊ�����
	float4 worldNormal, normal;

	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0);
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0);

	//���_�J���[�͂��̂܂܃R�s�[
	Out.Diffuse = In.Diffuse;

	Out.WorldPosition = mul(In.Position, World);

	matrix lightwvp;
	lightwvp = mul(World, Light.View);                  //���[���h�s�񁖃��C�g�r���[�s��
	lightwvp = mul(lightwvp, Light.Projection);         //����Ɂ����C�g�v���W�F�N�V�����s��
	Out.LightViewPosition = mul(In.Position, lightwvp); //���C�g�J��������݂����_���W�o��
}
