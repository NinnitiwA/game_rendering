#pragma once

class PostProcess
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;

	POSTPROCESSPARAMETER m_Param;
	FOG       m_FogParam;
public:
	void Init();
	void Uninit();
	void Update();
	
	// 被写界深度
	void DrawDepthOfField();

	// フォグ
	void DrawFog();

	// ブルーム
	void DrawLuminance();
	void DrawBlooms(int num);
	void DrawBloom();

	// FXAAアンチエイリアシング
	void DrawAntiAliasing();
}; 
