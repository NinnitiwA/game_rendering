
#include "common.hlsl"

Texture2D g_TextureReflection : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_REFLECT_IN In, out PS_DEFERRED_OUT Out)
{
	// �Z����
	float fusionRate = 0.45f;

	//// �@���}�b�s���O
	//// ���̃s�N�Z���̖@���}�b�v�̃f�[�^RGBA�l���擾
	//float4 normalMap = g_TextureBamp.Sample(g_SamplerState, In.TexCoord);
	//// �擾����RGBA�l���x�N�g�����XYZW�Ƃ��ēW�J����
	//normalMap = (normalMap * 2.0f) - 1.0f;

	//float4 n = normalize(In.Normal);
	//float4 t = normalize(In.Tangent);
	//float4 b = normalize(In.Binormal);
	//float4 normal;
	//normal.xyz = t.xyz * normalMap.x + b.xyz * normalMap.y + n.xyz * normalMap.z;
	//normal.w = 0.0;
	//normal = normalize(normal);

	//// �����̌����Ɖ�]�ς݂̖@���Ƃœ��ς��v�Z���A�����𔽓]���ċP�x�����߂�
	//float light = -dot(Light.Direction.xyz, normal.xyz);
	//// �O�a���Z(0.0�`1.0�̒l�ɃN�����v����)
	//light = saturate(light);


	// ���ʔ���
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 waterColor;

	waterColor = float4(0.032, 0.422, 0.8, 1.0);

	reflectTexCoord.x = In.ReflectPosition.x / In.ReflectPosition.w / 2.05f + 0.5025f;
	reflectTexCoord.y = -In.ReflectPosition.y / In.ReflectPosition.w / 2.05f + 0.5025f;
	// �e�N�X�`������F���T���v�����O����
	reflectionColor = g_TextureReflection.Sample(g_SamplerState, reflectTexCoord);
	// �}�e���A���̓��ߗ��ɉ����ăe�N�X�`���̐F�ɋP�x����Z����
	//reflectionColor.rgb = (fusionRate > 0.1) ? reflectionColor.rgb * light : reflectionColor.rgb;


	// �f�o�b�O�p
	//if (reflectTexCoord.x < 0.0 || reflectTexCoord.y < 0.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);
	//if (reflectTexCoord.x > 1.0 || reflectTexCoord.y > 1.0) reflectionColor = float4(0.032, 0.022, 0.8, 0.7);


	// ����
	float4 finalColor;
	finalColor = lerp(waterColor, reflectionColor, (1.0f - fusionRate));

	//// �u�����t�H�����ʔ���
	//// �����x�N�g�����쐬����i�J��������s�N�Z���֌������x�N�g���j
	//float3 evev = In.WorldPosition.xyz - Camera.Position.xyz;
	//evev = normalize(evev);
	//// �n�[�t�x�N�g�����쐬����i�����x�N�g���{���C�g�x�N�g���j
	//float3 halfv = evev + Light.Direction.xyz;
	//halfv = normalize(halfv);

	//float specular = -dot(halfv, normal.xyz);
	//saturate(specular);
	//specular = pow(specular, 20);
	//finalColor.rgb = saturate(finalColor.rgb + specular);

	//// �t���l�����˗��𔽉f
	//float dotNormal = dot(evev, In.Normal);
	//float fresnel = 0.05 + (1 - 0.05) * pow(1 - dotNormal, 10);
	//finalColor.a = fresnel;

	Out.Albedo = finalColor;
	Out.Position = In.Position;
	Out.Normal = float4(0.0, 1.0, 0.0, 1.0);
	Out.Material = float4(Material.Roughness, 1.0, Material.Metalic, 1.0);
}
