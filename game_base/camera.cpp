#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "animationObject.h"


D3DXMATRIX Camera::m_ViewMatrix = {};
D3DXMATRIX Camera::m_ProjectionMatrix = {};
D3DXMATRIX Camera::m_ReflectionMatrix = {};

void Camera::Init()
{
	m_Position = D3DXVECTOR3( 0.0f, 8.0f, -12.0f );
	m_Target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_OldTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FixedDistance = D3DXVECTOR3(0.0f, 16.0f, -14.0f);
	m_EffectOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_EventOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FovY = 60.0f * D3DX_PI / 180.0f;
	m_Time = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}


void Camera::Uninit()
{

}


void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 playerPos = scene->GetGameObject<Player>()->GetPosition();
	GUI::target = m_Target;
	m_EffectOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	// カメラコントロール中の処理
	if (scene->GetSceneName() == TITLE_SCENE)
	{
		ActTitleSceneCamera();
		return;
	}


	// カメラコントロール中の処理
	if (GUI::cameraControllFlag)
	{
		m_Target = D3DXVECTOR3(playerPos.x, 0.0f, playerPos.z) + GUI::targetDebugOffset;
		m_Position = m_Target + m_FixedDistance + GUI::cameraDebugOffset + D3DXVECTOR3(sinf(GUI::cameraRotation) * 30.0f, -playerPos.y, 15.0f -cosf(GUI::cameraRotation) * 30.0f);
		return;
	}


	// カメラの振動
	if (m_CameraOscillationFrame < 10)
	{
		switch (m_CameraOscillationMode)
		{
		case CO_LARGE_MODE:
			m_EffectOffset = D3DXVECTOR3((5.0f - (float)(rand() % 10)) / 10.0f, (5.0f - (float)(rand() % 10)) / 10.0f, (5.0f - (float)(rand() % 10)) / 10.0f);
			break;
		case CO_MEDIUM_MODE:
			m_EffectOffset = D3DXVECTOR3((3.0f - (float)(rand() % 6)) / 6.0f, (3.0f - (float)(rand() % 6)) / 6.0f, (3.0f - (float)(rand() % 6)) / 6.0f);
			break;
		case CO_SMALL_MODE:
			m_EffectOffset = D3DXVECTOR3((1.5f - (float)(rand() % 3)) / 3.0f, (1.5f - (float)(rand() % 3)) / 3.0f, (1.5f - (float)(rand() % 3)) / 3.0f) * 0.5f;
			break;
		default:
			break;
		}
		// フレーム更新
		m_CameraOscillationFrame++;
	}



	// カメラのターゲット座標設定
	m_Target.x = playerPos.x;
	m_Target.z = playerPos.z;

	// Y座標は揺れが激しくなるため一定値以下では考慮しない
	if (abs(playerPos.y - m_Target.y) >= 0.03f)
	{
		// 歩いているときの小さな揺れは考慮しない
		if (playerPos.y < 1.5f && abs(playerPos.y - m_Target.y) <= 0.15f)
		{
			m_Target.y = m_OldTarget.y;
		}
		// ゆっくりプレイヤーのY座標へ近づく
		else if (abs(playerPos.y - m_Target.y) >= 0.15f)
		{
			m_Target.y += (playerPos.y - m_Target.y) / 60.0f;
		}
	}
	else
	{
		m_Target.y = playerPos.y;
	}
	m_Target += m_EffectOffset;

	// ターゲット座標を記録
	m_OldTarget = m_Target;
	// カメラの座標を反映
	m_Position = m_Target + m_EffectOffset + m_FixedDistance + m_EventOffset;
}


void Camera::Draw()
{
	// ビューマトリクス設定
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);


	// 反射マトリクス計算
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 target = m_Target;
	pos.y = fabs(m_Position.y);
	target.y = fabs(m_Target.y);
	float reflectionHeight = 0.0f * 2; // 水面の高さ
	D3DXVECTOR3 reflectPos(pos.x, -pos.y + reflectionHeight, pos.z);
	D3DXVECTOR3 reflectTarget(target.x, -target.y, target.z);
	D3DXMatrixLookAtLH(&m_ReflectionMatrix, &reflectPos, &reflectTarget, &up);
	D3DXMatrixTranspose(&m_ReflectionMatrix, &m_ReflectionMatrix);


	// カメラ定数バッファ設定--------------------------------------------------------
	CAMERA camera{};

	// 視錐台座標計算
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4]; // プロジェクション座標

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	// 視錐台４頂点
	D3DXVec3TransformCoord(&vpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&vpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&vpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&vpos[3], &vpos[3], &invvp);

	camera.FrustumPosition[0] = D3DXVECTOR4(vpos[0], 1.0f);
	camera.FrustumPosition[1] = D3DXVECTOR4(vpos[1], 1.0f);
	camera.FrustumPosition[2] = D3DXVECTOR4(vpos[2], 1.0f);
	camera.FrustumPosition[3] = D3DXVECTOR4(vpos[3], 1.0f);

	// カメラ座標
	camera.Position = D3DXVECTOR4(m_Position, 1.0f);
	camera.Time = m_Time;
	camera.ReflectionMatrix = m_ReflectionMatrix;
	Renderer::SetCamera(camera);
}

void Camera::DrawReflection()
{
	// ビューマトリクス設定
	D3DXVECTOR3 pos = m_Position;
	D3DXVECTOR3 target = m_Target;
	pos.y = -fabs(m_Position.y) - 1.0f;
	target.y = -fabs(m_Target.y);

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);


	// カメラパラメータ設定
	CAMERA camera;
	camera.Position = D3DXVECTOR4(pos, 1.0f);
	m_Time.r += 0.001f;
	camera.Time = m_Time;
	Renderer::SetCamera(camera);
}

void Camera::DrawCascadeShadowMap(int i)
{
	// カスケードシャドウ処理-----------------------------------------------------------

	// 視錐台の各頂点計算---------------------------------------------------------------

	// 上方向ベクトルを計算
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	// 前方向ベクトルを計算
	D3DXVECTOR3 forward = m_Target - m_Position;
	D3DXVec3Normalize(&forward, &forward);
	// 右方向ベクトルを計算
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &forward, &up);


	float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	float nearDepth = 1.0f;
	if (i != 0)nearDepth = m_CascadeArea[i - 1];
	float farDepth = m_CascadeArea[i];

	// エリアの近平面の中心からの距離を計算
	float nearY = tanf(m_FovY * 0.5f) * nearDepth;
	float nearX = nearY * aspect;

	// エリアの遠平面の中心からの距離を計算
	float farY = tanf(m_FovY * 0.5f) * farDepth;
	float farX = farY * aspect;


	// エリアの近平面の中心座標を求める
	D3DXVECTOR3 nearPos(m_Position + forward * nearDepth);
	// エリアの遠平面の中心座標を求める
	D3DXVECTOR3 farPos(m_Position + forward * farDepth);

	// 8頂点
	D3DXVECTOR3 vertex[8];

	// 近平面
	vertex[0] = nearPos + up * nearY + right * nearX;  	// 右上
	vertex[1] = nearPos + up * nearY + right * -nearX;  // 左上
	vertex[2] = nearPos + up * -nearY + right * nearX;  // 右下
	vertex[3] = nearPos + up * -nearY + right * -nearX; // 左下

	// 遠平面
	vertex[4] = farPos + up * farY + right * farX;      // 右上
	vertex[5] = farPos + up * farY + right * -farX;     // 左上
	vertex[6] = farPos + up * -farY + right * farX;     // 右下
	vertex[7] = farPos + up * -farY + right * -farX;    // 左下


	// カスケードの中心座標を計算
	D3DXVECTOR3 vec(0.0f, 0.0f, 0.0f);
	for (D3DXVECTOR3& v : vertex)
	{
		vec += v;
	}
	D3DXVECTOR3 cascadeCenter(vec / 8.0f);
	GUI::pos[i] = cascadeCenter;

	// 視錐台の四頂点取得
	if (i == 2)
	{
		GUI::frustumPos[0] = vertex[4];
		GUI::frustumPos[1] = vertex[5];
		GUI::frustumPos[2] = vertex[6];
		GUI::frustumPos[3] = vertex[7];
	}


	// ライト作成---------------------------------------------------------------------

    // ライトカメラ構造体の初期化
	LIGHT light;
	light.LightType = DIRECTIONAL_LIGHT;
	light.Direction = D3DXVECTOR4(1.0f, -3.0f, 2.0f, 0.0f); // （座標 - 原点）のベクトル
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightDir(light.Direction.x, light.Direction.y, light.Direction.z);
	D3DXVECTOR3 mainPos(cascadeCenter.x, m_Position.y, cascadeCenter.z);
	float length = D3DXVec3Length(&cascadeCenter);
	D3DXVECTOR3 lightPos = mainPos - lightDir * length;
	D3DXVECTOR3 lightTarget = mainPos;
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

	// ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	Renderer::SetLight(light);


	// カスケード区画描画フラグ（デバッグ用）
	float cascade = 0.0f;
	if (GUI::cascadeFlag) cascade = 1.0f;
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, cascade);



	// 最大値最小値を計算
	D3DXVECTOR3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 vMin(FLT_MAX, FLT_MAX, FLT_MAX);
	for (D3DXVECTOR3& v : vertex)
	{
		D3DXVECTOR3 vec;
		// 座標変換
		D3DXMATRIX lightViewProjection = light.ViewMatrix * light.ProjectionMatrix;
		D3DXVec3TransformCoord(&vec, &v, &light.ViewMatrix);
		// 最大値更新
		D3DXVec3Maximize(&vMax, &vMax, &vec);
		// 最小値更新
		D3DXVec3Minimize(&vMin, &vMin, &vec);
	}

	// 範囲を拡大
	float margin = 5.0f;
	vMin -= D3DXVECTOR3(margin, margin, margin);
	vMax += D3DXVECTOR3(margin, margin, margin);

	// 直行射影行列に変換
	D3DXMatrixOrthoOffCenterLH(&light.ProjectionMatrix, vMin.x, vMax.x, vMin.y, vMax.y, vMin.z, vMax.z);

	// プロジェクション行列を直行射影行列に変換したものをセット
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);
	Renderer::SetLight(light);


	// 定数バッファに格納
	m_LVPC[i] = light.ViewMatrix * light.ProjectionMatrix;
	if (i == 2) Renderer::SetLightViewProjectionCropMatrix(m_LVPC);
}


void Camera::DrawDebugCascadeShadow()
{
	//ビューマトリクス設定
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 cameraPos(0.0f, 400.0f, -1.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &cameraPos, &up, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}


void Camera::DrawDebugAnimation()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	D3DXVECTOR3 pos = target + D3DXVECTOR3(-10.0f, 10.0f, -10.0f);

	//ビューマトリクス設定
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, m_FovY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 3000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::ActTitleSceneCamera()
{
	// カメラ処理
	switch (m_Phase)
	{
	case 0:
		m_Target = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
		m_Position += D3DXVECTOR3(0.04f, 0.004f, 0.01f);

		break;
	case 1:
		m_Target = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		m_Position = D3DXVECTOR3(5.0f, 4.0f, -15.0f) + D3DXVECTOR3(15.0f, 10.0f, -15.0f) * (m_PhaseFrame / 210.0f);

		break;
	case 2:
		m_Target = D3DXVECTOR3(26.0f, 6.0f, 25.0f) + D3DXVECTOR3(3.0f, 1.0f, 2.0f) * (m_PhaseFrame / 210.0f);
		m_Position = m_Target + D3DXVECTOR3(2.0f, 1.5f, -5.0f);

		break;
	}


	// フレーム更新
	m_PhaseFrame++;

	// フェーズ更新
	if (m_PhaseFrame >= 210)
	{
		m_PhaseFrame = 0;
		m_Position = D3DXVECTOR3(0.0f, 8.0f, -12.0f);
		
		m_Phase++;
		m_Phase %= 3;
	}
}
