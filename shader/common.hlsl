#define PI 3.1415926


struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
    float4 Tangent		: TANGENT0;
    float4 Binormal		: BINORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
	uint   InstanceId   : SV_InstanceID;
};

struct PS_IN
{
	float4 Position			 : SV_POSITION;
	float4 WorldPosition	 : POSITION0;
	float4 LightViewPosition : POSITION1;
	float4 Normal			 : NORMAL0;
    float4 Tangent			 : TANGENT0;
    float4 Binormal			 : BINORMAL0;
	float4 Diffuse			 : COLOR0;
	float2 TexCoord			 : TEXCOORD0;
};

struct PS_SHADOWMAP_IN
{
	float4 Position			 : SV_POSITION;
	float4 WorldPosition	 : POSITION0;
	float4 LightViewPosition[3] : POSITION1;
	float4 Normal			 : NORMAL0;
	float4 Tangent			 : TANGENT0;
	float4 Binormal			 : BINORMAL0;
	float4 Diffuse			 : COLOR0;
	float2 TexCoord			 : TEXCOORD0;
};

struct PS_DEFERRED_OUT
{
	float4 Albedo     : SV_TARGET0; // rgb : アルベド  
	float4 Position   : SV_TARGET1; // xyz : world座標 
	float4 Normal     : SV_TARGET2; // xyz : 法線    
	float4 Material   : SV_TARGET3; // x   : roughness y : shadow z ; metalic w : bloom
};

struct PS_POSTPROCESS_IN
{
	float4 Position		: POSITION0;
	float2 TexCoord		: TEXCOORD0;
};

struct GS_GRASS_IN
{
	float4 Position		: SV_POSITION;
	float4 ViewPosition : POSITION0;
	float4 Normal		: NORMAL0;
	float4 Tangent      : TANGENT0;
	float4 Binormal     : BINORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};

struct PS_GRASS_IN
{
	float4 Position			: SV_POSITION;
	float4 ViewPosition     : POSITION0;
	float4 Normal			: NORMAL0;
	float4 Tangent          : TANGENT0;
	float4 Binormal         : BINORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
};

struct PS_REFLECT_IN
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: POSITION0;
	float4 ReflectPosition  : POSITION1;
	float4 Normal			: NORMAL0;
	float4 Tangent			: TANGENT0;
	float4 Binormal			: BINORMAL0;
	float4 Diffuse			: COLOR0;
	float2 TexCoord			: TEXCOORD0;
};



cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

struct MATERIAL
{
	float4  	Ambient;
	float4  	Diffuse;
	float4  	Specular;
	float4  	Emission;
	float       Shininess;
	float		Metalic;
	float       Roughness;
	float       TextureEnable;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}

struct LIGHT
{
	int    LightType;
	int3   Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;    // w : cascadeShadow
	matrix View;       //ライトカメラのビュー行列
	matrix Projection; //ライトカメラのプロジェクション行列
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

struct CAMERA
{
	float4 Position;
	float4 FrustumPosition[4];  // 視錐台の四頂点座標
	float4 Time;
	matrix Reflection;
};

cbuffer CameraBuffer : register(b5)
{
	CAMERA Camera;
}

struct PostProcessParameter
{
	float4 Color;     // PBRObjectColor
	float4 Level;     // x : AA基準値        y : AAエッジ描画フラグ z : AA時UVオフセット     w : ブルーム基準値
	float4 Depth;     // x : 手前ぼけライン, y : 奥ぼけライン,      z : ぼけオフセット基準値 w : デバッグ識別子
	int    TexWidth;
	int    TexHeight;
	int2    Dummy;
};

cbuffer ParamBuffer : register(b6)
{
	PostProcessParameter Param;
}


struct POINTLIGHT
{
	float4 Position;
	float4 Attenuation;
	float4 Color; 
};

cbuffer PointLightBuffer : register(b7)
{
	POINTLIGHT PointLight[300];
	int        PointLightNum;
	int3       Dummy;
}


struct SPOTLIGHT
{
	float4 Position;    // w : range
	float4 Direction;   // w : radius
	float4 Attenuation;
	float4 Color; 
};

cbuffer SpotLightBuffer : register(b8)
{
	SPOTLIGHT SpotLight;
}


struct FOG
{
	float4 HeightColor;
	float4 HeightLevel; // x : 基準高度
	float4 DepthColor;
	float4 DepthLevel;  // x : 基準距離, y : 減衰距離
};

cbuffer FogBuffer : register(b9)
{
	FOG Fog;
}


cbuffer LightViewProjectionBuffer : register(b10)
{
	matrix LightVP[3];
}


struct PlayerParameter
{
	float4 Status;   // プレイヤーUI用
	float4 Material; // x : roughness y : metalic
};

cbuffer PlayerBuffer : register(b11)
{
	PlayerParameter PlayerParam;
}


cbuffer GrassBuffer : register(b12)
{
	float4 Grass;
}


struct WAVE
{
	float2 dir;
	float amplitude;
	float waveLength;
};

cbuffer WaveBuffer : register(b13)
{
	WAVE waves[20];
};



// 関数------------------------------------------------------------------------

// フレネル反射の計算
float3 CalcFresnelSchlick(float u, float f0, float f90) 
{
	return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}


// 正規化ディズニーモデルによる拡散反射光を計算
float CalcDisneyDiffuse(float3 normal, float3 lightDir, float3 eyev, float roughness, float metalic)
{
	// 光源ベクトルと視線ベクトルとのハーフベクトルを計算
	float3 H = normalize(lightDir + eyev);

	float LH = saturate(dot(-lightDir, H));
	float NL = saturate(dot(normal, lightDir));
	float NV = abs(dot(normal, eyev));

	float energyBias = lerp(0.0, 0.5, roughness);
	float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);
	float F90 = energyBias + 2.0 * LH * LH * roughness;
	float F0 = lerp(0.04, 1.0, metalic);

	// 光線方向と法線方向におけるフレネル反射成分の計算
	float FL = CalcFresnelSchlick(NL, F0, F90);
	float FV = CalcFresnelSchlick(NV, F0, F90);

	// フレネル反射の計算
	float F = (FV * FL * energyFactor);
	
	return F;
}


// Cook-Torranceモデルの鏡面反射光を計算
float CalcCookTorranceSpecular(float3 lightDir, float3 eyev, float3 normal, float3 albedo, float roughness, float metalic)
{
	// 光源ベクトルと視線ベクトルとのハーフベクトルを計算
	float3 H = normalize(lightDir + eyev);

	float NH = saturate(dot(normal, H));
	float NV = abs(dot(normal, eyev));
	float NL = saturate(dot(normal, lightDir));
	float VH = saturate(dot(eyev, H));
	float HV = saturate(dot(H, eyev));
	float LH = saturate(dot(lightDir, H));


	// 反射光の分散率を計算 GGXモデル
	float3 NHcross = cross(normal, H);
	float a = roughness * roughness;
	float f = (NH * a - NH) * NH + 1;
	float D = a / (f * f * PI);


	// 幾何減衰項を計算 SmithGGXモデル
	float GGX1 = NV / (NV * (1.0 - a) + a);
	float GGX2 = NL / (NL * (1.0 - a) + a);
	float G = GGX1 * GGX2;


	// フレネル項を計算
	float3 F0 = metalic * float3(0.04, 0.04, 0.04) + (1.0 - metalic) * albedo.rgb;
	float3 F = CalcFresnelSchlick(VH, F0, 1.0);


	// Cook-Torrance
	float CT = (D * G * F) * NL;

	return CT;
}

// 合成波計算
static int   numWaves = 20;
static float steepness = 0.08;
static float speed = 30;

float3 CalcWaveOffset(float3 vec)
{
	float3 offset = float3(0, 0, 0);
	[unroll]
	for (int i = 0; i < numWaves; i++)
	{
		WAVE wave = waves[i];
		float wi = 2 / wave.waveLength;
		float Qi = steepness / (wave.amplitude * wi * numWaves);
		float phi = speed * wi;
		float rad = dot(wave.dir, vec.xz) * wi + Camera.Time.x * phi;
		offset.y += sin(rad) * wave.amplitude;
		offset.xz += cos(rad) * wave.amplitude * Qi * wave.dir;
	}
	return offset;
}