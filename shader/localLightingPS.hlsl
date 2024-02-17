#include "common.hlsl"

Texture2D g_AlbedoTexture : register(t0);
Texture2D g_PositionTexture : register(t1);
Texture2D g_NormalTexture : register(t2);
Texture2D g_MaterialTexture  : register(t3);
Texture2D g_DirectionalLightingTexture  : register(t4);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outColor : SV_TARGET0)
{
	float2 uv = In.WorldPosition.xy / float2(Param.TexWidth, Param.TexHeight);
	
    // G�o�b�t�@��������擾
	float3 albedo = g_AlbedoTexture.Sample(g_SamplerState, uv).xyz;
	float4 worldPosition = g_PositionTexture.Sample(g_SamplerState, uv);
    float4 normal = g_NormalTexture.Sample(g_SamplerState, uv);
	float4 pbr = g_MaterialTexture.Sample(g_SamplerState, In.TexCoord);

	float4 diffuse = g_DirectionalLightingTexture.Sample(g_SamplerState, uv);

	float roughness = pbr.x;
	float metalic = pbr.z;
	float3 eyev = normalize(Camera.Position.xyz - worldPosition.xyz);
	float3 refv = normalize(reflect(eyev, normal.xyz));

	// �|�C���g���C�g************************************************************
	if (Light.LightType == 1)
	{
		for (int i = 0; i < PointLightNum; i++)
		{
			// �������W
			float3 pointLightPos = mul(PointLight[i].Position.xyz, World);

			// ����
			float4 ambient = 1.0f;

			// �g�U����
			float3 lightDir = pointLightPos - worldPosition.xyz;
			float distance = length(lightDir);
			lightDir = normalize(lightDir);


			// �e����
			float affect = 1.0f - (1.0f / 3.0f) * distance;
			if (affect < 0.0f) affect = 0.0f;

			affect = pow(affect, 3.0f);


			if (distance < 3.0f)
			{
				// ���ʔ���
				lightDir = saturate(lightDir);
				float NL = saturate(dot(normal, lightDir));
				float3 refv = normalize(2 * NL * normal - lightDir);
				float3 eyev = normalize(worldPosition.xyz - Camera.Position.xyz);
				float4 specular = pow(saturate(dot(refv, eyev)), 4) * 1.0f;
				// �t�H�����f���ŏI�F�@�R�̍��̍��v
				float4 color = ambient + diffuse + specular;
				color *= PointLight[i].Color;

				diffuse = color * affect + diffuse * (1.0f - affect);
				diffuse.a = 1.0f;
			}

		}
	}


	// �X�|�b�g���C�g************************************************************
	else if (Light.LightType == 2)
	{
		// �������W�ƕ���
		float3 spotLightPos = mul(SpotLight.Position.xyz, World);
		float3 spotLightDir = normalize(SpotLight.Direction.xyz);

		// �X�|�b�g���C�g�̃J�b�g�I�t�p�x�ƊO�����̃X�|�b�g���C�g���a
		float spotLightCosCutoff = 0.95f;
		float spotLightOuterRadius = tan(30.0f * 0.5);

		// ����
		float4 ambient = 1.0f;

		// �g�U����
		float3 lightDir = spotLightPos - worldPosition.xyz;
		float distance = length(lightDir);
		lightDir = normalize(lightDir);

		// �X�|�b�g���C�g�̉e���͈͓��ɂ��邩�ǂ������m�F
		float spotEffect = saturate((dot(-lightDir, spotLightDir) - spotLightCosCutoff) / (1.0 - spotLightCosCutoff));

		{ 
			// ���ʔ���
			lightDir = saturate(lightDir);
			float NL = saturate(dot(normal, lightDir));
			float3 refv = normalize(2 * NL * normal - lightDir);
			float3 eyev = normalize(worldPosition.xyz - Camera.Position.xyz);
			float4 specular = pow(saturate(dot(refv, eyev)), 4) * 1.0f;

			// �t�H�����f���ŏI�F�@�R�̍��̍��v
			float4 color = ambient + diffuse + specular;
			color *= SpotLight.Color;

			diffuse = color * spotEffect + diffuse * (1.0f - spotEffect);
			diffuse.a = 1.0f;
		}
	}

	// �ŏI�o��
	outColor = float4(diffuse.rgb, 1.0f);
}
