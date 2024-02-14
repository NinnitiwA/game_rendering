#include "common.hlsl"

// ���W
struct InstancedParam {
	float3 Position;
};
// �C���X�^���X��
struct InstanceNum {
	int InstanceNum;
};

StructuredBuffer<InstancedParam> WorldPositionArgs : register(t0);

StructuredBuffer<InstancedParam> gCullingResultBuffer : register(t1); // �J�����O���ʂ��i�[����o�b�t�@
StructuredBuffer<InstanceNum> gInstanceNumBuffer : register(t2);      // �C���X�^���X�����i�[����o�b�t�@


void main(in VS_IN In, out PS_IN Out)
{
	// �C���X�^���X�̍��W�擾
	uint index = In.InstanceId;

	In.Position.xyz += WorldPositionArgs[index].Position;

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);


	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
}

