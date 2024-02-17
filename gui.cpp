#include "main.h"
#include "gui.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "enemy.h"
#include "enemyMagic.h"


// GUI�E�B���h�E�ړ��s�� imgui.cpp 6231�s�ڃR�����g�A�E�g

bool GUI::drawGuiFlag = false;
bool GUI::sceneEndFlag = false;

bool GUI::playerControllFlag = true;
D3DXVECTOR3 GUI::playerPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR4 GUI::playerMaterial = D3DXVECTOR4(0.5f, 0.12f, 0.0f, 0.0f);
bool GUI::cameraControllFlag = false;
D3DXVECTOR3 GUI::cameraDebugOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 GUI::targetDebugOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
float GUI::cameraRotation = 0.0f;
bool GUI::drawInvisibleBox = false;
bool GUI::addEnemyObjectFlag = false;

PlayerAnimation GUI::playerAnimation = { false, {false} };

bool GUI::drawLocalLightFlag = false;
int  GUI::lightType = 0;
int  GUI::pointLightNum = 20;
int  GUI::textureType = 6;

D3DXCOLOR GUI::PBRObjectColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
bool GUI::pbrObjectDrawFlag = false;
bool GUI::postProcessFlag = true;
bool GUI::depthOfField = true;
bool GUI::depthOfFieldDebugFlag = false;
D3DXVECTOR4 GUI::depthParameter = D3DXVECTOR4(0.940f, 0.970f, 1.25f, 0.0f);
bool GUI::fog[] = {false};
FOG GUI::fogParameter = {};
bool  GUI::bloom = false;
float GUI::luminanceValue = 0.775f;
bool  GUI::antiAliasing = true;
float GUI::antiAliasingBias = 0.075f;
float GUI::UVOffset = 0.1f;
bool  GUI::drawEdgeFlag = false;

bool GUI::profilingFlag = false;
int GUI::profileNum = 0;
std::vector<PROFILE*> GUI::profile = {};
PROFILE* GUI::allTimeProfile = {};

bool GUI::cascadeFlag = false;
D3DXVECTOR3 GUI::pos[] = {};
D3DXVECTOR3 GUI::target = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
D3DXVECTOR3 GUI::frustumPos[] = {};
D3DXVECTOR3 GUI::test = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


void GUI::ImGuiInit(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// ImGui�쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = static_cast<float>(SCREEN_WIDTH);
	io.DisplaySize.y = static_cast<float>(SCREEN_HEIGHT);
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();
}

void GUI::ImGuiRender()
{
	// ImGui�t���[���쐬
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static int counter = 0;



	// GUI����E�B���h�E�쐬
	ImGui::Begin("GUI ");
	ImGui::Checkbox("Draw GUI", &drawGuiFlag);
	ImGui::SameLine();
	if (ImGui::Button("Scene Change")) sceneEndFlag = true;
	ImGui::End();

	// GUI�`��
	if (drawGuiFlag)
	{
		// �����_�����O���j���[�`��
		ImGuiRenderRenderingMenu();

		// �v���t�@�C���[�`��
		ImGuiRenderProfiler();

		// SRV�r���A�[�`��
		ImGuiRenderSRV();

		// ImGui�f���`��
		//ImGui::ShowDemoWindow();
	}


	// ImGui�`��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void GUI::ImGuiRenderRenderingMenu()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	// ImGui�E�B���h�E�쐬
	ImGui::Begin("Test");

	// �v���C���[����t���O
	ImGui::Checkbox("PlayerControll", &playerControllFlag);
	playerPosition = player->GetPosition();
	ImGui::Text("X:%f Y:%f Z:%f", playerPosition.x, playerPosition.y, playerPosition.z);
	// �J�����R���g���[���t���O
	ImGui::Checkbox("CameraControll", &cameraControllFlag);
	if (cameraControllFlag) ImGuiRenderCameraControll();

	// �A�j���[�V�����E�B���h�E�`��
	ImGui::Checkbox("PlayerAnimation", &playerAnimation.DrawDebugWindow);
	if (playerAnimation.DrawDebugWindow) ImGuiRenderPlayerAnimation();
	// �v���C���[�}�e���A��
	ImGui::SliderFloat("Roughness:%.4f", &playerMaterial.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Metalic:%.4f", &playerMaterial.y, 0.0f, 1.0f);
	// �����I�u�W�F�N�g�`��t���O
	ImGui::Checkbox("DrawInvisibleBox", &drawInvisibleBox);

	// �G�l�~�[�ǉ�
	if (ImGui::Button("Enemy")) scene->AddGameObject<Enemy>(1)->SetPosition(player->GetPosition() + D3DXVECTOR3(0.0f, 7.0f, 7.0f));
	ImGui::SameLine();
	if (ImGui::Button("EnemyMagic")) scene->AddGameObject<EnemyMagic>(1)->SetPosition(player->GetPosition() + D3DXVECTOR3(0.0f, 7.0f, 7.0f));
	ImGui::SameLine();
	if (ImGui::Button("EnemyStone")) scene->AddGameObject<EnemyStone>(1)->SetPosition(player->GetPosition() + D3DXVECTOR3(0.0f, 7.0f, 7.0f));

	// �V���h�E�E���C�e�B���O
	if (ImGui::CollapsingHeader("Shadow Lighting"))
	{
		// �J�X�P�[�h���`��t���O
		ImGui::Checkbox("DrawCascadeShadow", &cascadeFlag);

		// �������f���`��t���O
		ImGui::Checkbox("DrawLightModel", &drawLocalLightFlag);

		ImGui::Text("RenderMode");
		ImGui::RadioButton("PointLight", &lightType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("SpotLight", &lightType, 1);

		ImGui::Text("PointLight Num");
		ImGui::SliderInt("Num:%d", &pointLightNum, 0.0f, 200.0f);

	}
	// �J�X�P�[�h���`��
	if (cascadeFlag) GUI::ImGuiRenderCascadeShadow();


	// �`��e�N�X�`��
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::RadioButton("Albedo", &textureType, 0);
		ImGui::RadioButton("Position", &textureType, 1);
		ImGui::RadioButton("Normal", &textureType, 2);
		ImGui::RadioButton("LightViewDepth", &textureType, 3);
		ImGui::RadioButton("Roughness / Metalic", &textureType, 4);
		ImGui::RadioButton("Directional Lighting", &textureType, 5);
		ImGui::RadioButton("Local Lighting", &textureType, 6);

	}
	// �|�X�g�v���Z�X
	if (ImGui::CollapsingHeader("PostProcess"))
	{
		// PBR�I�u�W�F�N�g�`��t���O
		ImGui::Checkbox("DrawPBRObject", &pbrObjectDrawFlag);
		// PBR�I�u�W�F�N�g�J���[
		ImGui::Text("Object Color");
		ImGui::ColorEdit4("", (float*)&PBRObjectColor);
		ImGui::Text(" ");

		if (textureType == 5 || textureType == 6) postProcessFlag = true;
		else postProcessFlag = false;
		if (postProcessFlag)
		{
			ImGui::Text("PostProcess");

			// �A���`�G�C���A�V���O
			ImGui::CheckboxNameless("AntiAliasing", &antiAliasing);
			ImGui::SameLine();
			if (ImGui::TreeNode("AntiAliasing"))
			{
				ImGui::SliderFloat("Bias:%.4f", &antiAliasingBias, 0.0f, 0.6f);
				ImGui::SliderFloat("Blur Value:%.4f", &UVOffset, 0.0f, 2.0f);
				ImGui::Checkbox("Draw Edge", &drawEdgeFlag);
				ImGui::TreePop();
			}


			// ��ʊE�[�x
			ImGui::CheckboxNameless("Depth Of Field", &depthOfField);
			ImGui::SameLine();
			if (ImGui::TreeNode("Depth Of Field"))
			{
				ImGui::SliderFloat("Front:%.4f", &depthParameter.x, 0.5f, 1.0f);
				ImGui::SliderFloat("Back:%.4f", &depthParameter.y, 0.5f, 1.0f);

				// ��ʊE�[�x�f�o�b�O�\��
				ImGui::Checkbox("DrawLightModel", &depthOfFieldDebugFlag);
				if(depthOfFieldDebugFlag) depthParameter.w = 1.0f;
				else depthParameter.w = 0.0f;

				ImGui::TreePop();
			}


			// �t�H�O
			ImGui::CheckboxNameless("Fog", &fog[0]);
			ImGui::SameLine();
			if (ImGui::TreeNode("Fog"))
			{
				ImGui::Checkbox("Height Fog", &fog[1]);
				if (fogParameter.HeightColor.w > 0.5f)
				{
					ImGui::ColorEdit4("", (float*)&fogParameter.HeightColor);
					ImGui::SliderFloat("CenterY:%.4f", &fogParameter.HeightLevel.y, -10.0f, 10.0f);
					ImGui::SliderFloat("Height:%.4f", &fogParameter.HeightLevel.x, 0.0f, 10.0f);

				}
				ImGui::Checkbox("Depth Fog", &fog[2]);
				if (fogParameter.DepthColor.w > 0.5f)
				{
					ImGui::ColorEdit4("", (float*)&fogParameter.DepthColor);
					ImGui::SliderFloat("CenterZ:%.4f", &fogParameter.DepthLevel.y, -1000.0f, 1000.0f);
					ImGui::SliderFloat("Width:%.4f", &fogParameter.DepthLevel.x, -1000.0f, 1000.0f);

				}

				// �萔�o�b�t�@�p�t�H�O���ʎq
				if (fog[1]) fogParameter.HeightColor.w = 1.0f;
				else fogParameter.HeightColor.w = 0.0f;
				if (fog[2]) fogParameter.DepthColor.w = 1.0f;
				else fogParameter.DepthColor.w = 0.0f;


				ImGui::TreePop();
			}


			// �u���[��
			ImGui::CheckboxNameless("Bloom", &bloom);
			ImGui::SameLine();
			if (ImGui::TreeNode("Bloom"))
			{
				ImGui::SliderFloat("Bias:%.4f", &luminanceValue, 0.0f, 1.0f);
				ImGui::TreePop();
			}
		}

	}


	ImGui::End();


}

void GUI::ImGuiRenderProfiler()
{
	// �v���t�@�C���[�쐬
	ImGui::Begin("Profiler");
	profilingFlag = false;
	if (ImGui::Button("Snap Shot"))
	{
		profilingFlag = true;
		profile.clear();
		profileNum = 0;
	}


	// �������ԃe�L�X�g�\��
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.15f, 150), false, window_flags);
		for (int i = 0; i < profile.size(); i++)
		{
			ImGui::Text("%s %lf[ms]", profile[i]->ProcessName, profile[i]->Time);
		}
		ImGui::EndChild();
	}


	ImGui::SameLine();


	// �������ԃO���t�B�b�N�\��
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.ItemSpacing = ImVec2(1.0f, style.ItemSpacing.y);


		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(-40, 120), true, windowFlags);

		float timeWidth = 1.0f;
		for (int i = 0; i < profile.size(); i++)
		{
			ImGui::SameLine();

			if (profile[i]->Time < 0.05) timeWidth = 5.0f;
			else timeWidth = profile[i]->Time * 45.0f;

			char buf[32];
			sprintf(buf, "%s", profile[i]->ProcessName);

			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
			ImGui::Button(buf, ImVec2(timeWidth, 0.0f));
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		// �t���[������
		EndAllTimeProfiling();
		static float values[90] = {};
		static int values_offset = 0;
		static double refresh_time = 0.0;

		if (refresh_time == 0.0)
			refresh_time = ImGui::GetTime();

		while (refresh_time < ImGui::GetTime())
		{
			// allTimeProfile���玞�Ԃ��擾
			float currentTime = allTimeProfile->Time;

			// �f�[�^���X�V
			values[values_offset] = currentTime;
			values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);

			// �X�V���Ԃ̐i�s
			refresh_time += ImGui::GetIO().DeltaTime;
		}

		// ���ϒl�̌v�Z�ƃO���t�̕`��
		{
			float average = 0.0f;
			for (int n = 0; n < IM_ARRAYSIZE(values); n++)
				average += values[n];
			average /= (float)IM_ARRAYSIZE(values);

			// ���ϒl���I�[�o�[���C�Ƃ��ĕ\��
			char overlay[32];
			sprintf(overlay, "avg %f , fps %f", average, 1000.0f / average);

			// �O���t�̕`��
			ImGui::PlotHistogram("", values, IM_ARRAYSIZE(values), values_offset, overlay, 0.0f, 45.0f, ImVec2(0, 80.0f));
		}
		BeginAllTimeProfiling();

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	



	ImGui::End();
}

void GUI::ImGuiRenderSRV()
{
	ImGui::Begin("SRV");

	const char* items[] = { "G-Buffer", "Reflection", "ShadowMap", "AA", "DOF", "Fog", "Luminance", "Bloom"};
	static int currentItem = 0;
	ImGui::Combo("SRV", &currentItem, items, IM_ARRAYSIZE(items));

	ID3D11ShaderResourceView* srv;
	switch (currentItem)
	{
	case 0:
		srv = Renderer::GetAlbedoColorTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetPositionTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetNormalTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));

		srv = Renderer::GetMaterialTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetDepthTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		break;


	case 1:
		srv = Renderer::GetReflectionTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(800, 450));
		break;


	case 2:
		srv = Renderer::GetShadowMapTexture(0);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetShadowMapTexture(1);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetShadowMapTexture(2);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		break;


	case 3:
		srv = Renderer::GetBloomTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(800, 450));
		break;


	case 4:
		srv = Renderer::GetDepthOfFieldTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(800, 450));
		break;


	case 5:
		srv = Renderer::GetFogTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(800, 450));
		break;


	case 6:
		srv = Renderer::GetLuminanceTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetBloomsTexture(0);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetBloomsTexture(1);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));

		srv = Renderer::GetBloomsTexture(2);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		ImGui::SameLine();
		srv = Renderer::GetBloomsTexture(3);
		ImGui::Image((ImTextureID)srv, ImVec2(335, 200));
		break;


	case 7:
		srv = Renderer::GetAntiAliasingTexture();
		ImGui::Image((ImTextureID)srv, ImVec2(800, 450));
		break;
	}

	ImGui::End();
}

void GUI::ImGuiRenderCascadeShadow()
{
	ImGui::Begin("Cascaded Shadow");

	ImGui::Text("x : %f y : %f z : %f", pos[0].x, pos[0].y, pos[0].z);
	ImGui::Text("x : %f y : %f z : %f", pos[1].x, pos[1].y, pos[1].z);
	ImGui::Text("x : %f y : %f z : %f", pos[2].x, pos[2].y, pos[2].z);

	ID3D11ShaderResourceView* srv = Renderer::GetDebugCascadeShadowTexture();
	ImGui::Image((ImTextureID)srv, ImVec2(400, 225));

	ImGui::End();
}

void GUI::ImGuiRenderPlayerAnimation()
{
	ImGui::Begin("Animation");

	// Walk
	if (ImGui::Button("Walk") && !playerAnimation.Walk) playerAnimation.Walk = true;

	// Jump
	if (ImGui::Button("Jump") && !playerAnimation.Jump) playerAnimation.Jump = true;

	// PressAttack
	if (ImGui::Button("PressAttack") && !playerAnimation.PressAttack) playerAnimation.PressAttack = true;

	// DashAttack
	if (ImGui::Button("DashAttack") && !playerAnimation.DashAttack) playerAnimation.DashAttack = true;

	// MagicAttack
	if (ImGui::Button("MagicAttack") && !playerAnimation.MagicAttack) playerAnimation.MagicAttack = true;

	// MagicChant
	if (ImGui::Button("MagicChant") && !playerAnimation.MagicChant) playerAnimation.MagicChant = true;

	// �A�j���[�V�����`��
	ID3D11ShaderResourceView* srv = Renderer::GetDebugAnimationTexture();
	ImGui::Image((ImTextureID)srv, ImVec2(400, 225));

	ImGui::End();
}

void GUI::ImGuiRenderCameraControll()
{
	ImGui::Begin("Camera");

	if (ImGui::Button("Reset"))
	{
		cameraDebugOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		targetDebugOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cameraRotation = 0.0f;
	}

	ImGui::Text("Camera Position");
	ImGui::SliderFloat("cp X:%.4f", &cameraDebugOffset.x, -60.0f, 60.0f);
	ImGui::SliderFloat("cp Y:%.4f", &cameraDebugOffset.y, -60.0f, 60.0f);
	ImGui::SliderFloat("cp Z:%.4f", &cameraDebugOffset.z, -60.0f, 60.0f);
	ImGui::Text("Target Position");
	ImGui::SliderFloat("tp X:%.4f", &targetDebugOffset.x, -60.0f, 60.0f);
	ImGui::SliderFloat("tp Y:%.4f", &targetDebugOffset.y, -60.0f, 60.0f);
	ImGui::SliderFloat("tp Z:%.4f", &targetDebugOffset.z, -60.0f, 60.0f);
	ImGui::Text("Camera Rotation");
	ImGui::SliderFloat("c Rot:%.4f", &cameraRotation, -4.0f, 4.0f);
	
	ImGui::End();
}


void GUI::BeginProfiling()
{
	if (!profilingFlag) return;


	// �V�K�v���t�@�C�����O������z��ɒǉ�
	PROFILE* p = new PROFILE;
	profile.push_back(p);

	// �v���t�@�C�����O�J�n
	QueryPerformanceFrequency(&p->Freq);
	QueryPerformanceCounter(&p->Start);

	// �ԍ��E���O��ݒ�
	p->Num = (PROFILING_DIVISION)profileNum;
	p->ProcessName = SetProcessName((PROFILING_DIVISION)profileNum);
	profileNum++;
}

void GUI::EndProfiling(int num)
{
	if (!profilingFlag) return;


	// �Y��������z�񂩂�擾
	PROFILE* p = profile.at(num);

	// �v���t�@�C�����O�I��
	QueryPerformanceCounter(&p->End);
	p->Time = (double)(p->End.QuadPart - p->Start.QuadPart) * 1000.0 / p->Freq.QuadPart;
}

void GUI::BeginAllTimeProfiling()
{
	// �V�K�v���t�@�C�����O������z��ɒǉ�
	PROFILE* p = new PROFILE;
	allTimeProfile = p;

	// �v���t�@�C�����O�J�n
	QueryPerformanceFrequency(&p->Freq);
	QueryPerformanceCounter(&p->Start);

	// �ԍ��E���O��ݒ�
	p->Num = (PROFILING_DIVISION)profileNum;
	p->ProcessName = SetProcessName((PROFILING_DIVISION)profileNum);
}

void GUI::EndAllTimeProfiling()
{
	if (allTimeProfile == nullptr) return;

	// �v���t�@�C�����O�I�� 
	QueryPerformanceCounter(&allTimeProfile->End);
	allTimeProfile->Time = (double)(allTimeProfile->End.QuadPart - allTimeProfile->Start.QuadPart) * 1000.0 / allTimeProfile->Freq.QuadPart;
}

const char* GUI::SetProcessName(PROFILING_DIVISION pd)
{
	switch (pd)
	{
	case UPDATE_DIVISION:
		return "UPDATE";
		break;

	case REFLECTION_DIVISION:
		return "REFLECTION";
		break;

	case ZPREPASS_DIVISION:
		return "Z PRE-PASS";
		break;

	case SHADOWMAP_DIVISION:
		return "SHADOWMAP";
		break;

	case GBUFFER_DIVISION:
		return "G-BUFFER";
		break;

	case LIGHTING_DIVISION:
		return "LIGHTING";
		break;

	case POSTPROCESS_DIVISION:
		return "POSTPROCESS";
		break;

	default:
		break;
	}

	return 0;
}
