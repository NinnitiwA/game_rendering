#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DepthNearTexture : register(t1);
Texture2D g_DepthMiddleTexture : register(t2);
Texture2D g_DepthFarTexture : register(t3);
SamplerState g_SamplerState : register(s0);


void main(in PS_SHADOWMAP_IN In, out PS_DEFERRED_OUT Out)
{
	Out = (PS_DEFERRED_OUT)0;


	matrix wvp; 
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 color;
	color = g_Texture.Sample(g_SamplerState, In.TexCoord);
	color.a = 1.0f;
	Out.Albedo = color;

	float4 position;
	position = In.WorldPosition;
	position.a = 1.0f;
	Out.Position = position;

	float4 normal;
	normal = float4(normalize(In.Normal.rgb), 1.0f);
	Out.Normal = normal;



	float4 pbr = float4(1.0, 1.0, 1.0, 1.0);

	// Roughness
	pbr.x = Material.Roughness;
	// Metalic
	pbr.z = Material.Metalic;
	// Shininess
	pbr.w += Material.Shininess;


	// �V���h�E�}�b�s���O
	Texture2D<float4> shadowMapArray[3];
	shadowMapArray[0] = g_DepthNearTexture;
	shadowMapArray[1] = g_DepthMiddleTexture;
	shadowMapArray[2] = g_DepthFarTexture;
	bool cascaded = false; // �J�X�P�[�h���`��ςݎ��ʎq�i�͈͓��̒��ň�Ԏ�O�̃J�X�P�[�h�̂ݔ���j
	for (int i = 0; i < 3; i++)
	{
		// ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z
		In.LightViewPosition[i].xyz /= In.LightViewPosition[i].w; // ���K���f�o�C�X���W�ɕϊ�

		if (In.LightViewPosition[i].z >= 0 && In.LightViewPosition[i].z <= 1.0f)
		{
			//�e�N�X�`�����W�ɕϊ�
			In.LightViewPosition[i].x = In.LightViewPosition[i].x * 0.5 + 0.5;  
			In.LightViewPosition[i].y = -In.LightViewPosition[i].y * 0.5 + 0.5;

			// �V���h�E�}�b�v��UV�͈͓����ǂ���
			if (In.LightViewPosition[i].x >= 0.0 && In.LightViewPosition[i].x <= 1.0 &&
				In.LightViewPosition[i].y >= 0.0 && In.LightViewPosition[i].y <= 1.0)
			{
				// �J�X�P�[�h���`��i�f�o�b�O�p�j
				if (Light.Ambient.w > 0.5f && !cascaded)
				{
					float4 cascade = float4(1.0f * (1 - i), i, i * i - i, 1.0f);
					Out.Albedo.rgb = cascade.rgb;
					cascaded = true;
				}


				// �V���h�E�}�b�v����T���v�����O
				float depth = shadowMapArray[i].Sample(g_SamplerState, In.LightViewPosition[i].xy).r;

				if (depth < In.LightViewPosition[i].z - 0.0005f) //0.005��Z�t�@�C�e�B���O�␳�l
				{
					pbr.y = 0.5f;
				}


				// �V���h�E�}�b�v�͈͓��̏ꍇ�}�e���A�������i�[���ďI��
				Out.Material = pbr;
				return;
			}
		}
	}
	Out.Material = pbr;
}
