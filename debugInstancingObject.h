#pragma once
#include "instancingObject.h"

// �f�o�b�O�p�C���X�^���V���O�I�u�W�F�N�g
class DebugInstancingObject : public InstancingObject
{
private:

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DrawZPrePass() override;
};