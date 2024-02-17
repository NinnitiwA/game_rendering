#pragma once
#include "gameObject.h"

class DebugObject : public GameObject
{
private:
	int m_DebugNumber; // 固有の番号
	static int m_Num;  // デバッグオブジェクトの総数

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawDebug();

	// デバッグオブジェクト毎のパラメータを設定
	void CalcDebugParameter(int num);
};