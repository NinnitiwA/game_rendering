#include "common.hlsl"

// ���W
struct InstancedParam {
	float3 Position;
};
// �C���X�^���X��
struct InstanceNum {
	int InstanceNum;
};


// �\�����o�b�t�@
StructuredBuffer<InstancedParam> WorldPositionArgs : register(t0);

// RWStructuredBuffers
RWStructuredBuffer<InstancedParam> gCullingResultBuffer : register(u0);
RWStructuredBuffer<InstanceNum> gInstanceNumBuffer : register(u1);

// �J�����O�������s���֐�
bool CalcFrustumCulling(float3 worldPos);


// �R���s���[�g�V�F�[�_
[numthreads(256, 1, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
    // �X���b�hID���擾
	uint threadID = dispatchThreadId.x * 256;


    // �e�X���b�h���S������͈͂̏���
    for (int i = 0; i < 256; i++)
	if(threadID < WorldPositionArgs.Length)
    {
        // ���[���h���W�ϊ�
        float3 worldPos = WorldPositionArgs[threadID + i].Position;

        // ������J�����O����
        if (CalcFrustumCulling(worldPos))
        {
			// �X���b�h�Z�[�t
			InterlockedAdd(gInstanceNumBuffer[0].InstanceNum, 1);

			// �`�悷��I�u�W�F�N�g�̍��W���i�[
			gCullingResultBuffer[1 + gInstanceNumBuffer[0].InstanceNum].Position = worldPos;
			// �C���X�^���X�̐������Z
			//gInstanceNumBuffer[0].InstanceNum++;
        }
    }
}


bool CalcFrustumCulling(float3 worldPos)
{
	float3 v, v1, v2;

	// ���[���h���W�n�ɕϊ�
	v = worldPos - Camera.Position.xyz;

	// ���ʔ���
	{
		v1 = Camera.FrustumPosition[0].xyz - Camera.Position.xyz;
		v2 = Camera.FrustumPosition[2].xyz - Camera.Position.xyz;
		float3 normal = cross(v1, v2);

		if (dot(normal, v) < 0.0f) return false;
	}

	// �E�ʔ���
	{
		v1 = Camera.FrustumPosition[3].xyz - Camera.Position.xyz;
		v2 = Camera.FrustumPosition[1].xyz - Camera.Position.xyz;
		float3 normal = cross(v1, v2);

		if (dot(normal, v) < 0.0f) return false;
	}

	return true;
}