#pragma once
#include "instancingObject.h"


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