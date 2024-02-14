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
	
	// ��ʊE�[�x
	void DrawDepthOfField();

	// �t�H�O
	void DrawFog();

	// �u���[��
	void DrawLuminance();
	void DrawBlooms(int num);
	void DrawBloom();

	// FXAA�A���`�G�C���A�V���O
	void DrawAntiAliasing();
}; 
