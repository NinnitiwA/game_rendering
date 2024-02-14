#include "common.hlsl"

// 座標
struct InstancedParam {
	float3 Position;
};
// インスタンス数
struct InstanceNum {
	int InstanceNum;
};


// 構造化バッファ
StructuredBuffer<InstancedParam> WorldPositionArgs : register(t0);

// RWStructuredBuffers
RWStructuredBuffer<InstancedParam> gCullingResultBuffer : register(u0);
RWStructuredBuffer<InstanceNum> gInstanceNumBuffer : register(u1);

// カリング処理を行う関数
bool CalcFrustumCulling(float3 worldPos);


// コンピュートシェーダ
[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadId : SV_DispatchThreadID, uint3 groupId : SV_GroupID, uint3 groupIdInThread : SV_GroupThreadID)
{
    // スレッドIDを取得
    uint threadID = dispatchThreadId.x;


    // 各スレッドが担当する範囲の処理
    for (int i = threadID; i < 40000; i+= 4)
    {
        // ワールド座標変換
        float3 worldPos = WorldPositionArgs[i].Position;

        // 視錐台カリング処理
        if (CalcFrustumCulling(worldPos))
        {
			// 描画するオブジェクトの座標を格納
			gCullingResultBuffer[gInstanceNumBuffer[0].InstanceNum].Position = worldPos;
			// インスタンスの数を加算
			gInstanceNumBuffer[0].InstanceNum++;
        }
    }
}


bool CalcFrustumCulling(float3 worldPos)
{
	float3 v, v1, v2;

	// ワールド座標系に変換
	v = worldPos - Camera.Position.xyz;

	// 左面判定
	{
		v1 = Camera.FrustumPosition[0].xyz - Camera.Position.xyz;
		v2 = Camera.FrustumPosition[2].xyz - Camera.Position.xyz;
		float3 normal = cross(v1, v2);

		if (dot(normal, v) < 0.0f) return false;
	}

	// 右面判定
	{
		v1 = Camera.FrustumPosition[3].xyz - Camera.Position.xyz;
		v2 = Camera.FrustumPosition[1].xyz - Camera.Position.xyz;
		float3 normal = cross(v1, v2);

		if (dot(normal, v) < 0.0f) return false;
	}

	return true;
}