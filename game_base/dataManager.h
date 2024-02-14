#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>


template<class Archive>
void serialize(Archive & archive, D3DXVECTOR3 &vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
}

struct Vector3Data {
	std::string name;
	std::vector<D3DXVECTOR3> VectorArray;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(name), CEREAL_NVP(VectorArray));
	}
};


void LoadData();
void SaveData();

// �G���A�ɔz�u�����̍��W���X�g
static D3DXVECTOR3 TSI_RockPositionList[] =
{
	D3DXVECTOR3(-2.0f, 6.0f, 2.0f),
	D3DXVECTOR3(-5.0f, 3.5f, 0.0f),
	D3DXVECTOR3(-72.0f, 12.5f, 40.0f),
	D3DXVECTOR3(-76.0f, 13.5f, 34.0f),
	D3DXVECTOR3(-132.0f, 14.0f, 18.0f),
	D3DXVECTOR3(-130.0f, 13.5f, 20.0f),
		D3DXVECTOR3(-34.0f, 8.0f, 32.0f),
	D3DXVECTOR3(-64.0f, 11.0f, 44.0f),
	D3DXVECTOR3(-61.0f, 9.0f, 42.0f),
	D3DXVECTOR3(-115.0f, 14.0f, 4.0f),
	D3DXVECTOR3(-112.0f, 12.0f, 4.0f),
};

// �G���A�ɔz�u����؂̍��W���X�g
static D3DXVECTOR3 TSI_TreePositionList[] =
{
	D3DXVECTOR3(-18.0f, 4.0f, 0.0f),
	D3DXVECTOR3(-4.0f, 8.0f, 38.0f),
	D3DXVECTOR3(-30.0f, 8.0f, 58.0f),
	D3DXVECTOR3(-72.0f, 12.0f, 58.0f),
	D3DXVECTOR3(-78.0f, 14.0f, 7.0f),
	D3DXVECTOR3(-98.0f, 14.0f, 13.0f),
	D3DXVECTOR3(-123.0f, 14.0f, 2.0f),
	D3DXVECTOR3(-142.0f, 14.0f, 18.0f),
	D3DXVECTOR3(-116.0f, 16.0f, 68.0f),

	D3DXVECTOR3(-185.0f, 21.0f, 120.0f),
	D3DXVECTOR3(-95.0f, 21.0f, 195.0f),
	D3DXVECTOR3(-104.0f, 21.0f, 113.0f),
	D3DXVECTOR3(-89.0f, 21.0f, 135.0f),
	D3DXVECTOR3(-116.0f, 21.0f, 128.0f),
	D3DXVECTOR3(-147.0f, 21.0f, 125.0f),
	D3DXVECTOR3(-168.0f, 21.0f, 141.0f),
	D3DXVECTOR3(-190.0f, 21.0f, 173.0f),
	D3DXVECTOR3(-168.0f, 21.0f, 200.0f),
	D3DXVECTOR3(-192.0f, 21.0f, 227.0f),
	D3DXVECTOR3(-123.0f, 21.0f, 220.0f),
	D3DXVECTOR3(-127.0f, 21.0f, 184.0f),
};

// �G���A�ɔz�u����n�ʑ����̍��W���X�g
static D3DXVECTOR3 TSI_GroundGrassPositionList[] =
{
	D3DXVECTOR3(0.0f, 4.0f, 0.0f),
	D3DXVECTOR3(-20.0f, 8.0f, 40.0f),
	D3DXVECTOR3(-65.0f, 12.0f, 40.0f),
	D3DXVECTOR3(-85.0f, 14.0f, 20.0f),
	D3DXVECTOR3(-125.0f, 14.0f, 20.0f),
	D3DXVECTOR3(-135.0f, 16.0f, 70.0f),
	D3DXVECTOR3(-135.0f, 17.5f, 90.0f),
	D3DXVECTOR3(-135.0f, 19.0f, 110.0f),
	D3DXVECTOR3(-95.0f, 21.0f, 130.0f),
	D3DXVECTOR3(-95.0f, 21.0f, 170.0f),
	D3DXVECTOR3(-95.0f, 21.0f, 210.0f),
	D3DXVECTOR3(-135.0f, 21.0f, 130.0f),
	D3DXVECTOR3(-135.0f, 21.0f, 170.0f),
	D3DXVECTOR3(-135.0f, 21.0f, 210.0f),
	D3DXVECTOR3(-175.0f, 21.0f, 130.0f),
	D3DXVECTOR3(-175.0f, 21.0f, 170.0f),
	D3DXVECTOR3(-175.0f, 21.0f, 210.0f),
};

// �G���A�ɔz�u����ʏ�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TSI_EnemyPositionList[] =
{
	//D3DXVECTOR3(10.0f, 14.0f, 10.0f),
	D3DXVECTOR3(-68.0f, 18.0f, 48.0f),
	D3DXVECTOR3(-74.0f, 18.0f, 53.0f),
	D3DXVECTOR3(-81.0f, 18.0f, 44.0f),
	D3DXVECTOR3(-110.0f, 20.0f, 33.0f),
	D3DXVECTOR3(-110.0f, 20.0f, 17.0f),
};

// �G���A�ɔz�u���閂�@�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TSI_EnemyMagicPositionList[] =
{
	D3DXVECTOR3(-13.0f, 14.0f, 40.0f),
	D3DXVECTOR3(-72.0f, 20.0f, 7.0f),
	D3DXVECTOR3(-82.0f, 20.0f, 13.0f),
	D3DXVECTOR3(-124.0f, 22.0f, 54.0f),
	D3DXVECTOR3(-135.0f, 22.0f, 52.0f),
	D3DXVECTOR3(-146.0f, 22.0f, 54.0f),
};

// �G���A�ɔz�u����Ή��G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TSI_EnemyStonePositionList[] =
{
	D3DXVECTOR3(-25.0f, 14.0f, 31.0f),
	D3DXVECTOR3(-55.0f, 18.0f, 26.0f),
	D3DXVECTOR3(-102.0f, 20.0f, 25.0f),
	D3DXVECTOR3(-129.0f, 20.0f, 30.0f),
};

// �G���A�ɔz�u�����̍��W���X�g
static D3DXVECTOR3 TCF_RockPositionList[] =
{
	D3DXVECTOR3(-2.0f, 2.0f, 2.0f),
	D3DXVECTOR3(118.0f, 9.5f, 98.0f),
	D3DXVECTOR3(132.0f, 9.5f,110.0f),
	D3DXVECTOR3(130.0f, 8.5f,107.0f),
	D3DXVECTOR3(106.0f,10.0f,128.0f),
	D3DXVECTOR3(114.0f,12.0f,159.0f),
	D3DXVECTOR3(132.0f,11.5f,192.0f),
	D3DXVECTOR3(11.0f, 7.0f,215.0f),
	D3DXVECTOR3(88.0f, 5.0f, 42.0f),
	D3DXVECTOR3(98.0f, 6.5f, 68.0f),
	D3DXVECTOR3(128.0f,11.0f,137.0f),
	D3DXVECTOR3(108.0f,10.0f,130.0f),
	D3DXVECTOR3(112.0f,12.5f,160.0f),
	D3DXVECTOR3(124.0f,12.5f,186.0f),
	D3DXVECTOR3(124.0f,12.0f,183.0f),
	D3DXVECTOR3(121.0f,12.0f,184.0f),
	D3DXVECTOR3(104.0f,11.5f,174.0f),
	D3DXVECTOR3(60.0f, 7.0f,224.0f),
	D3DXVECTOR3(14.0f, 7.0f, 218.0f),
};

// �G���A�ɔz�u����؂̍��W���X�g
static D3DXVECTOR3 TCF_TreePositionList[] =
{
	D3DXVECTOR3(-18.0f, 2.0f, 18.0f),
	D3DXVECTOR3(18.0f, 2.0f,-18.0f),
	D3DXVECTOR3(34.0f, 3.5f,-8.0f),
	D3DXVECTOR3(58.0f, 3.5f, 18.0f),

	D3DXVECTOR3(98.0f, 5.0f, 28.0f),  // �E���[�g
	D3DXVECTOR3(138.0f, 9.5f, 68.0f),
	D3DXVECTOR3(102.0f, 9.5f, 112.0f),
	D3DXVECTOR3(102.0f, 9.5f, 98.0f),
	D3DXVECTOR3(120.0f, 9.5f, 74.0f),
	D3DXVECTOR3(138.0f,11.0f, 122.0f),
	D3DXVECTOR3(128.0f,11.0f, 158.0f),
	D3DXVECTOR3(128.0f,12.6f, 198.0f),
	D3DXVECTOR3(84.0f,11.0f, 182.0f),
	D3DXVECTOR3(108.0f, 9.5f, 218.0f),
	D3DXVECTOR3(68.0f, 8.0f, 238.0f),
	D3DXVECTOR3(48.0f, 8.0f, 238.0f),
	D3DXVECTOR3(54.0f, 8.0f, 202.0f),
	D3DXVECTOR3(36.0f, 8.0f, 224.0f),
	D3DXVECTOR3(4.0f, 8.0f, 210.0f),

	D3DXVECTOR3(28.0f, 3.5f,  48.0f), // �����[�g
	D3DXVECTOR3(-8.0f, 3.5f,  42.0f),
	D3DXVECTOR3(-38.0f, 2.0f, 102.0f),
	D3DXVECTOR3(-48.0f, 3.5f, 162.0f),
	D3DXVECTOR3(-54.0f, 5.0f, 238.0f),

	D3DXVECTOR3(25.0f, 0.0f, 90.0f), // ����
	D3DXVECTOR3(85.0f, 0.0f,130.0f),
};

// �G���A�ɔz�u����n�ʑ������W���X�g
static D3DXVECTOR3 TCF_GroundGrassPositionList[] =
{
	D3DXVECTOR3(10.0f, 2.0f, 0.0f),
	D3DXVECTOR3(-30.0f, 2.0f, 0.0f),
	D3DXVECTOR3(-70.0f, 2.0f, 0.0f),
	D3DXVECTOR3(40.0f, 3.5f, 10.0f),
	D3DXVECTOR3(40.0f, 3.5f, 50.0f),
	D3DXVECTOR3(0.0f, 3.5f, 60.0f),
	D3DXVECTOR3(-20.0f, 2.0f, 80.0f),
	D3DXVECTOR3(-20.0f, 2.0f, 120.0f),
	D3DXVECTOR3(-30.0f, 3.5f, 160.0f),
	D3DXVECTOR3(-30.0f, 3.5f, 200.0f),
	D3DXVECTOR3(-60.0f, 5.0f, 220.0f),
	D3DXVECTOR3(-20.0f, 5.0f, 220.0f),
	D3DXVECTOR3(80.0f, 5.0f, 40.0f),
	D3DXVECTOR3(100.0f, 6.5f, 60.0f),
	D3DXVECTOR3(120.0f, 9.5f, 80.0f),
	D3DXVECTOR3(120.0f, 9.5f, 120.0f),
	D3DXVECTOR3(120.0f, 11.0f, 140.0f),
	D3DXVECTOR3(120.0f, 12.5f, 180.0f),
	D3DXVECTOR3(100.0f, 11.0f, 200.0f),
	D3DXVECTOR3(20.0f, 8.0f, 220.0f),
	D3DXVECTOR3(60.0f, 8.0f, 220.0f),
};

// �G���A�ɔz�u����ʏ�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TCF_EnemyPositionList[] =
{
	D3DXVECTOR3(10.0f, 5.0f, 10.0f),
	D3DXVECTOR3(20.0f, 6.5f, -15.0f),
	D3DXVECTOR3(25.0f, 6.5f, 35.0f),
	D3DXVECTOR3(75.0f, 6.5f, 40.0f),
	D3DXVECTOR3(85.0f, 6.5f, 35.0f),
	D3DXVECTOR3(125.0f, 8.0f, 70.0f),
	D3DXVECTOR3(-25.0f,  5.0f,110.0f),
	D3DXVECTOR3(45.0f,  1.0f,160.0f),
	D3DXVECTOR3(25.0f,  1.0f,170.0f),
	D3DXVECTOR3(0.0f,  5.0f, 70.0f),
};

// �G���A�ɔz�u���閂�@�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TCF_EnemyMagicPositionList[] =
{
	D3DXVECTOR3(-35.0f, 5.0f, 10.0f),
	D3DXVECTOR3(-35.0f, 5.0f, -10.0f),
	D3DXVECTOR3(35.0f, 6.5f, 10.0f),
	D3DXVECTOR3(95.0f, 6.5f, 45.0f),
	D3DXVECTOR3(115.0f, 8.0f, 90.0f),
	D3DXVECTOR3(125.0f, 11.0f, 170.0f),
	D3DXVECTOR3(5.0f,  5.0f, 55.0f),
	D3DXVECTOR3(-5.0f,  5.0f,160.0f),
	D3DXVECTOR3(45.0f,  1.0f,140.0f),
};

// �G���A�ɔz�u����Ή��G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TCF_EnemyStonePositionList[] =
{
	D3DXVECTOR3(45.0f, 1.0f, 45.0f),
	D3DXVECTOR3(115.0f, 11.0f, 175.0f),
	D3DXVECTOR3(45.0f,  1.0f, 85.0f),
	D3DXVECTOR3(15.0f,  5.0f, 95.0f),
	D3DXVECTOR3(-15.0f,  5.0f,100.0f),
	D3DXVECTOR3(25.0f,  5.0f,150.0f),
	D3DXVECTOR3(85.0f, 0.0f,120.0f),
};

// �G���A�ɔz�u�����̍��W���X�g
static D3DXVECTOR3 TLOB_RockPositionList[] =
{
	D3DXVECTOR3(-16.0f, 8.0f, 5.0f),
	D3DXVECTOR3(-14.0f, 8.0f, 3.5f),
	D3DXVECTOR3(16.0f, 9.0f, 10.0f),
	D3DXVECTOR3(-34.0f, 9.0f, 46.0f),
	D3DXVECTOR3(-26.0f, 9.0f, 34.0f),

	D3DXVECTOR3(-26.0f, 6.0f, 104.0f), // ����
	D3DXVECTOR3(22.0f, 5.0f, 110.0f),
	D3DXVECTOR3(26.0f, 5.0f, 106.0f),
	D3DXVECTOR3(18.0f, 5.0f, 84.0f),

	D3DXVECTOR3(-10.0f, 2.0f, 182.0f), // �卂��
	D3DXVECTOR3(-8.0f, 1.0f, 180.0f),
	D3DXVECTOR3(18.0f, 2.0f, 202.0f),
	D3DXVECTOR3(16.0f, 2.0f, 194.0f),
	D3DXVECTOR3(18.0f, 4.0f, 202.0f), 
	D3DXVECTOR3(22.0f, 3.0f, 198.0f),
};

// �G���A�ɔz�u����؂̍��W���X�g
static D3DXVECTOR3 TLOB_TreePositionList[] =
{
	D3DXVECTOR3(-3.0f, 9.0f, -18.0f), // �ŏ��̍���
	D3DXVECTOR3(-18.0f, 9.0f, -18.0f),
	D3DXVECTOR3(17.0f, 9.0f, -18.0f),
	D3DXVECTOR3(-18.0f, 9.0f, -15.0f),
	D3DXVECTOR3(-18.0f, 9.0f, -14.0f),
	D3DXVECTOR3(5.0f, 9.0f, -18.0f),
	D3DXVECTOR3(16.0f, 9.0f, -22.0f),
	D3DXVECTOR3(-10.0f, 9.0f, -22.0f),
	D3DXVECTOR3(-5.0f, 9.0f, -22.0f),
	D3DXVECTOR3(8.0f, 9.0f, -22.0f),
	D3DXVECTOR3(5.0f, 9.0f, -22.0f),
	D3DXVECTOR3(14.0f, 9.0f, -25.0f),
	D3DXVECTOR3(-16.0f, 9.0f, -25.0f),
	D3DXVECTOR3(2.0f, 9.0f, -25.0f),
	D3DXVECTOR3(18.0f, 9.0f, -25.0f),
	D3DXVECTOR3(11.0f, 9.0f, -30.0f),
	D3DXVECTOR3(-4.0f, 9.0f, -30.0f),
	D3DXVECTOR3(-18.0f, 9.0f, -30.0f),
	D3DXVECTOR3(5.0f, 9.0f, -30.0f),
	D3DXVECTOR3(16.0f, 9.0f, -34.0f),
	D3DXVECTOR3(-10.0f, 9.0f, -34.0f),
	D3DXVECTOR3(-5.0f, 9.0f, -38.0f),
	D3DXVECTOR3(8.0f, 9.0f, -38.0f),
	D3DXVECTOR3(5.0f, 9.0f, -38.0f),
	D3DXVECTOR3(14.0f, 9.0f, -42.0f),
	D3DXVECTOR3(-16.0f, 9.0f, -42.0f),
	D3DXVECTOR3(2.0f, 9.0f, -42.0f),
	D3DXVECTOR3(18.0f, 9.0f, -42.0f),
	D3DXVECTOR3(11.0f, 9.0f, -46.0f),
	D3DXVECTOR3(-4.0f, 9.0f, -46.0f),
	D3DXVECTOR3(-18.0f, 9.0f, -46.0f),
	D3DXVECTOR3(5.0f, 9.0f, -46.0f),

	D3DXVECTOR3(18.0f, 9.0f, 18.0f),
	D3DXVECTOR3(-28.0f, 9.0f, 22.0f),
	D3DXVECTOR3(-38.0f, 9.0f, 58.0f),
	D3DXVECTOR3(38.0f, 9.0f, 58.0f),
	D3DXVECTOR3(48.0f, 2.0f, 32.0f),

	D3DXVECTOR3(-38.0f, 6.0f, 78.0f), // ����
	D3DXVECTOR3(-38.0f, 6.0f, 111.0f),
	D3DXVECTOR3(-38.0f, 6.0f, 118.0f),
	D3DXVECTOR3(-20.0f, 6.0f, 82.0f),
	D3DXVECTOR3(8.0f, 6.0f, 130.0f),
	D3DXVECTOR3(38.0f, 6.0f, 98.0f),
	D3DXVECTOR3(38.0f, 6.0f, 122.0f),

	D3DXVECTOR3(38.0f, 6.0f, 138.0f), // �卂��
	D3DXVECTOR3(-38.0f, 2.0f, 168.0f),
	D3DXVECTOR3(-38.0f, 6.0f, 138.0f),
	D3DXVECTOR3(-38.0f, 2.0f, 198.0f),
	D3DXVECTOR3(-34.0f, 2.0f, 218.0f),
	D3DXVECTOR3(-24.0f, 2.0f, 178.0f),
	D3DXVECTOR3(-12.0f, 2.0f, 160.0f),
	D3DXVECTOR3(-28.0f, 2.0f, 210.0f),
	D3DXVECTOR3(0.0f, 2.0f, 188.0f),
	D3DXVECTOR3(10.0f, 2.0f, 218.0f),
	D3DXVECTOR3(20.0f, 2.0f, 172.0f),

	D3DXVECTOR3(45.0f, 2.0f, 72.0f),// ��X��
	D3DXVECTOR3(52.0f, 2.0f, 68.0f),
	D3DXVECTOR3(59.0f, 2.0f, 68.0f),
	D3DXVECTOR3(67.0f, 2.0f, 68.0f),
	D3DXVECTOR3(77.0f, 2.0f, 68.0f),
	D3DXVECTOR3(85.0f, 2.0f, 74.0f),
	D3DXVECTOR3(106.0f, 2.0f, 68.0f),
	D3DXVECTOR3(114.0f, 2.0f, 68.0f),
	D3DXVECTOR3(61.0f, 2.0f, 80.0f),
	D3DXVECTOR3(67.0f, 2.0f, 82.0f),
	D3DXVECTOR3(73.0f, 2.0f, 78.0f),
	D3DXVECTOR3(85.0f, 2.0f, 80.0f),
	D3DXVECTOR3(91.0f, 2.0f, 80.0f),
	D3DXVECTOR3(97.0f, 2.0f, 80.0f),
	D3DXVECTOR3(103.0f, 2.0f, 78.0f),
	D3DXVECTOR3(110.0f, 2.0f, 80.0f),
	D3DXVECTOR3(117.0f, 2.0f, 80.0f),
	D3DXVECTOR3(42.0f, 2.0f, 92.0f),
	D3DXVECTOR3(60.0f, 2.0f, 92.0f),
	D3DXVECTOR3(68.0f, 2.0f, 92.0f),
	D3DXVECTOR3(80.0f, 2.0f, 92.0f),
	D3DXVECTOR3(92.0f, 2.0f, 92.0f),
	D3DXVECTOR3(110.0f, 2.0f, 92.0f),
	D3DXVECTOR3(44.0f, 2.0f, 102.0f),
	D3DXVECTOR3(50.0f, 2.0f, 102.0f),
	D3DXVECTOR3(61.0f, 2.0f, 102.0f),
	D3DXVECTOR3(65.0f, 2.0f, 102.0f),
	D3DXVECTOR3(73.0f, 2.0f, 108.0f),
	D3DXVECTOR3(80.0f, 2.0f, 102.0f),
	D3DXVECTOR3(86.0f, 2.0f, 104.0f),
	D3DXVECTOR3(96.0f, 2.0f, 102.0f),
	D3DXVECTOR3(110.0f, 2.0f, 102.0f),
	D3DXVECTOR3(118.0f, 2.0f, 102.0f),
	D3DXVECTOR3(60.0f, 2.0f, 112.0f),
	D3DXVECTOR3(68.0f, 2.0f, 116.0f),
	D3DXVECTOR3(80.0f, 2.0f, 112.0f),
	D3DXVECTOR3(92.0f, 2.0f, 112.0f),
	D3DXVECTOR3(110.0f, 2.0f, 112.0f),
	D3DXVECTOR3(116.0f, 2.0f, 114.0f),
	D3DXVECTOR3(50.0f, 2.0f, 128.0f),
	D3DXVECTOR3(65.0f, 2.0f, 122.0f),
	D3DXVECTOR3(73.0f, 2.0f, 126.0f),
	D3DXVECTOR3(80.0f, 2.0f, 124.0f),
	D3DXVECTOR3(96.0f, 2.0f, 122.0f),
	D3DXVECTOR3(102.0f, 2.0f, 122.0f),
	D3DXVECTOR3(110.0f, 2.0f, 122.0f),
	D3DXVECTOR3(118.0f, 2.0f, 122.0f),
	D3DXVECTOR3(43.0f, 2.0f, 132.0f),
	D3DXVECTOR3(51.0f, 2.0f, 134.0f),
	D3DXVECTOR3(58.0f, 2.0f, 132.0f),
	D3DXVECTOR3(65.0f, 2.0f, 132.0f),
	D3DXVECTOR3(74.0f, 2.0f, 136.0f),
	D3DXVECTOR3(82.0f, 2.0f, 132.0f),
	D3DXVECTOR3(90.0f, 2.0f, 132.0f),
	D3DXVECTOR3(100.0f, 2.0f, 132.0f),
	D3DXVECTOR3(50.0f, 2.0f, 144.0f),
	D3DXVECTOR3(61.0f, 2.0f, 144.0f),
	D3DXVECTOR3(65.0f, 2.0f, 142.0f),
	D3DXVECTOR3(80.0f, 2.0f, 146.0f),
	D3DXVECTOR3(86.0f, 2.0f, 142.0f),
	D3DXVECTOR3(90.0f, 2.0f, 148.0f),
	D3DXVECTOR3(96.0f, 2.0f, 142.0f),
	D3DXVECTOR3(110.0f, 2.0f, 142.0f),
	D3DXVECTOR3(118.0f, 2.0f, 142.0f),
	D3DXVECTOR3(42.0f, 2.0f, 152.0f),
	D3DXVECTOR3(54.0f, 2.0f, 156.0f),
	D3DXVECTOR3(60.0f, 2.0f, 154.0f),
	D3DXVECTOR3(68.0f, 2.0f, 152.0f),
	D3DXVECTOR3(92.0f, 2.0f, 152.0f),
	D3DXVECTOR3(110.0f, 2.0f, 152.0f),
	D3DXVECTOR3(116.0f, 2.0f, 158.0f),
	D3DXVECTOR3(44.0f, 2.0f, 162.0f),
	D3DXVECTOR3(50.0f, 2.0f, 168.0f),
	D3DXVECTOR3(56.0f, 2.0f, 162.0f),
	D3DXVECTOR3(65.0f, 2.0f, 164.0f),
	D3DXVECTOR3(73.0f, 2.0f, 162.0f),
	D3DXVECTOR3(86.0f, 2.0f, 162.0f),
	D3DXVECTOR3(90.0f, 2.0f, 162.0f),
	D3DXVECTOR3(96.0f, 2.0f, 162.0f),
	D3DXVECTOR3(102.0f, 2.0f, 162.0f),
	D3DXVECTOR3(110.0f, 2.0f, 162.0f),
	D3DXVECTOR3(43.0f, 2.0f, 172.0f),
	D3DXVECTOR3(51.0f, 2.0f, 172.0f),
	D3DXVECTOR3(58.0f, 2.0f, 172.0f),
	D3DXVECTOR3(65.0f, 2.0f, 174.0f),
	D3DXVECTOR3(82.0f, 2.0f, 172.0f),
	D3DXVECTOR3(90.0f, 2.0f, 178.0f),
	D3DXVECTOR3(115.0f, 2.0f, 172.0f),
	D3DXVECTOR3(44.0f, 2.0f, 182.0f),
	D3DXVECTOR3(61.0f, 2.0f, 186.0f),
	D3DXVECTOR3(65.0f, 2.0f, 182.0f),
	D3DXVECTOR3(73.0f, 2.0f, 182.0f),
	D3DXVECTOR3(80.0f, 2.0f, 182.0f),
	D3DXVECTOR3(86.0f, 2.0f, 186.0f),
	D3DXVECTOR3(90.0f, 2.0f, 182.0f),
	D3DXVECTOR3(102.0f, 2.0f, 182.0f),
	D3DXVECTOR3(110.0f, 2.0f, 186.0f),
	D3DXVECTOR3(53.0f, 2.0f, 192.0f),
	D3DXVECTOR3(70.0f, 2.0f, 192.0f),
	D3DXVECTOR3(80.0f, 2.0f, 195.0f),
	D3DXVECTOR3(92.0f, 2.0f, 192.0f),
	D3DXVECTOR3(110.0f, 2.0f, 192.0f),
	D3DXVECTOR3(116.0f, 2.0f, 198.0f),
	D3DXVECTOR3(48.0f, 2.0f, 206.0f),
	D3DXVECTOR3(66.0f, 2.0f, 202.0f),
	D3DXVECTOR3(73.0f, 2.0f, 202.0f),
	D3DXVECTOR3(82.0f, 2.0f, 202.0f),
	D3DXVECTOR3(102.0f, 2.0f, 206.0f),
	D3DXVECTOR3(112.0f, 2.0f, 202.0f),
	D3DXVECTOR3(43.0f, 2.0f, 212.0f),
	D3DXVECTOR3(58.0f, 2.0f, 212.0f),
	D3DXVECTOR3(70.0f, 2.0f, 210.0f),
	D3DXVECTOR3(82.0f, 2.0f, 212.0f),
	D3DXVECTOR3(108.0f, 2.0f, 210.0f),
};

// �G���A�ɔz�u����n�ʑ������W���X�g
static D3DXVECTOR3 TLOB_GroundGrassPositionList[] =
{
	D3DXVECTOR3(30.0f, 2.0f, 40.5f),
	D3DXVECTOR3(0.0f, 9.0f, -40.0f),
	D3DXVECTOR3(0.0f, 9.0f, 0.0f),
	D3DXVECTOR3(20.0f, 9.0f, 40.0f),
	D3DXVECTOR3(-20.0f, 9.0f, 40.0f),
	D3DXVECTOR3(-20.0f, 6.0f, 80.0f),
	D3DXVECTOR3(20.0f, 6.0f, 80.0f),
	D3DXVECTOR3(-20.0f, 6.0f, 120.0f),
	D3DXVECTOR3(20.0f, 6.0f, 120.0f),
	D3DXVECTOR3(100.0f, 2.0f, 120.0f),
	D3DXVECTOR3(60.0f, 2.0f, 120.0f),
	D3DXVECTOR3(100.0f, 2.0f, 80.0f),
	D3DXVECTOR3(60.0f, 2.0f, 80.0f),
	D3DXVECTOR3(20.0f, 2.0f, 160.0f),
	D3DXVECTOR3(-20.0f, 2.0f, 160.0f),
	D3DXVECTOR3(60.0f, 2.0f, 160.0f),
	D3DXVECTOR3(100.0f, 2.0f, 160.0f),
	D3DXVECTOR3(20.0f, 2.0f, 200.0f),
	D3DXVECTOR3(-20.0f, 2.0f, 200.0f),
	D3DXVECTOR3(60.0f, 2.0f, 200.0f),
	D3DXVECTOR3(100.0f, 2.0f, 200.0f),
	D3DXVECTOR3(30.0f, 6.0f, 240.0f),
	D3DXVECTOR3(70.0f, 10.0f, 240.0f),
};

// �G���A�ɔz�u����ʏ�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TLOB_EnemyPositionList[] =
{
	D3DXVECTOR3(-23.0f, 7.0f, 80.0f),
	D3DXVECTOR3(-17.0f, 7.0f, 82.0f),
	D3DXVECTOR3(24.0f, 7.0f, 100.0f),
	D3DXVECTOR3(-26.0f, 7.0f, 109.0f),

	D3DXVECTOR3(14.0f, 3.0f, 172.0f), // �卂��
	D3DXVECTOR3(18.0f, 3.0f, 168.0f),
	D3DXVECTOR3(22.0f, 3.0f, 168.0f),
	D3DXVECTOR3(26.0f, 3.0f, 172.0f),
	D3DXVECTOR3(-10.0f, 3.0f, 198.0f),
	D3DXVECTOR3(-26.0f, 3.0f, 182.0f),
	D3DXVECTOR3(-36.0f, 3.0f, 202.0f),

	D3DXVECTOR3(70.0f, 3.0f, 68.0f), // ��X��
	D3DXVECTOR3(98.0f, 3.0f, 74.0f),
	D3DXVECTOR3(56.0f, 3.0f, 80.0f),
	D3DXVECTOR3(78.0f, 3.0f, 84.0f),
	D3DXVECTOR3(102.0f, 3.0f, 82.0f),
	D3DXVECTOR3(68.0f, 3.0f, 88.0f),
	D3DXVECTOR3(88.0f, 3.0f, 92.0f),
	D3DXVECTOR3(98.0f, 3.0f, 98.0f),
	D3DXVECTOR3(104.0f, 3.0f, 98.0f),
	D3DXVECTOR3(67.0f, 3.0f, 106.0f),
	D3DXVECTOR3(52.0f, 3.0f, 114.0f),
	D3DXVECTOR3(80.0f, 3.0f, 120.0f),
	D3DXVECTOR3(100.0f, 3.0f, 130.0f),
	D3DXVECTOR3(112.0f, 3.0f, 134.0f),
	D3DXVECTOR3(76.0f, 3.0f, 140.0f),
	D3DXVECTOR3(64.0f, 3.0f, 148.0f),
	D3DXVECTOR3(92.0f, 3.0f, 160.0f),
	D3DXVECTOR3(102.0f, 3.0f, 168.0f),
	D3DXVECTOR3(80.0f, 3.0f, 176.0f),
	D3DXVECTOR3(67.0f, 3.0f, 182.0f),
	D3DXVECTOR3(114.0f, 3.0f, 194.0f),
	D3DXVECTOR3(74.0f, 3.0f, 200.0f),
	D3DXVECTOR3(98.0f, 3.0f, 206.0f),
	D3DXVECTOR3(110.0f, 3.0f, 210.0f),
};

// �G���A�ɔz�u���閂�@�G�l�~�[�̍��W���X�g
static D3DXVECTOR3 TLOB_EnemyMagicPositionList[] =
{
	D3DXVECTOR3(4.0f, 7.0f, 128.0f),
};

// �v���C���[
// �ړ����̃A�j���[�V����
static D3DXVECTOR3 g_PlayerWalkAnimPos[14] =
{
	D3DXVECTOR3(0.0f, -0.05f, 0.0f), // ���˂�O
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),

	D3DXVECTOR3(0.0f, 0.35f, 0.0f), // ���˂Ă��鎞
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, 0.35f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.12f, 0.0f),
	D3DXVECTOR3(0.0f, 0.08f, 0.0f),
	D3DXVECTOR3(0.0f, 0.04f, 0.0f),
};
static D3DXVECTOR3 g_PlayerWalkAnimScale[14] =
{
	D3DXVECTOR3(1.05f, 0.95f, 1.05f), // ���˂�O
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),

	D3DXVECTOR3(1.0f, 1.2f, 1.0f), // ���˂Ă��鎞
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
};


// �W�����v���̃A�j���[�V����
static D3DXVECTOR3 g_PlayerJumpAnimPos[13] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // �W�����v�O
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),

	D3DXVECTOR3(0.0f, 0.2f, 0.0f), // �W�����v��
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.25f, 0.0f),
	D3DXVECTOR3(0.0f, 0.25f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
};
static D3DXVECTOR3 g_PlayerJumpAnimScale[13] =
{
	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // �W�����v�O
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),

	D3DXVECTOR3(0.8f, 1.4f, 0.8f),    // �W�����v��
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
};


// �U�����̃A�j���[�V����
static D3DXVECTOR3 g_PlayerPressAttackAnimPos[14] =
{
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.3f, 0.0f),
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.22f, 0.0f),
	D3DXVECTOR3(0.0f, -0.17f, 0.0f),
	D3DXVECTOR3(0.0f, -0.14f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.6f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
};
static D3DXVECTOR3 g_PlayerPressAttackAnimScale[14] =
{
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.75f, 1.6f, 0.75f),
	D3DXVECTOR3(0.7f, 1.7f, 0.7f),
	D3DXVECTOR3(2.8f, 0.7f, 2.8f),
	D3DXVECTOR3(3.2f, 0.65f, 3.2f),
	D3DXVECTOR3(3.3f, 0.65f, 3.3f),
	D3DXVECTOR3(3.2f, 0.65f, 3.2f),
	D3DXVECTOR3(2.8f, 0.68f, 2.8f),
	D3DXVECTOR3(2.5f, 0.72f, 2.5f),
	D3DXVECTOR3(2.3f, 0.77f, 2.3f),
	D3DXVECTOR3(2.0f, 0.83f, 2.0f),
	D3DXVECTOR3(1.6f, 0.88f, 1.6f),
	D3DXVECTOR3(1.2f, 0.95f, 1.2f),
	D3DXVECTOR3(1.05f, 1.0f, 1.05f),
};

// �U�����̃A�j���[�V����
static D3DXVECTOR3 g_PlayerDashAttackAnimPos[40] =
{
	D3DXVECTOR3(0.0f,0.0f, -0.1f),  // �U���O
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),
	D3DXVECTOR3(0.0f,0.0f, -0.05f),
	D3DXVECTOR3(0.0f,0.0f,  0.1f),
	D3DXVECTOR3(0.0f,0.0f, -0.1f),
	D3DXVECTOR3(0.0f,0.0f,  0.05f),

	D3DXVECTOR3(0.0f, 0.0f,  0.095f), // �L�т�
	D3DXVECTOR3(0.0f, 0.0f,  0.14f),
	D3DXVECTOR3(0.0f, 0.0f,  0.185f),
	D3DXVECTOR3(0.0f, 0.0f,  0.23f),
	D3DXVECTOR3(0.0f, 0.0f,  0.245f),
	D3DXVECTOR3(0.0f, 0.0f,  0.29f),
	D3DXVECTOR3(0.0f, 0.0f,  0.335f),
	D3DXVECTOR3(0.0f, 0.0f,  0.38f),
	D3DXVECTOR3(0.0f, 0.0f,  0.425f),
	D3DXVECTOR3(0.0f, 0.0f,  0.47f),
	D3DXVECTOR3(0.0f, 0.0f,  0.515f),
	D3DXVECTOR3(0.0f, 0.0f,  0.56f),
	D3DXVECTOR3(0.0f, 0.0f,  0.605f),
	D3DXVECTOR3(0.0f, 0.0f,  0.65f),
	D3DXVECTOR3(0.0f, 0.0f,  0.695f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.15f),
	D3DXVECTOR3(0.0f, 0.0f,  0.05f),

	D3DXVECTOR3(0.0f, 0.0f,  0.6f), // �U����
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.5f),
	D3DXVECTOR3(0.0f, 0.0f,  0.5f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f,  0.05f),
};
static D3DXVECTOR3 g_PlayerDashAttackAnimScale[40] =
{
	D3DXVECTOR3(1.02f, 1.0f, 1.02f), // �U���O
	D3DXVECTOR3(1.05f, 1.0f, 1.05f),
	D3DXVECTOR3(1.08f, 1.0f, 1.08f),
	D3DXVECTOR3(1.11f, 1.0f, 1.11f),
	D3DXVECTOR3(1.14f, 1.0f, 1.14f),
	D3DXVECTOR3(1.16f, 1.0f, 1.16f),
	D3DXVECTOR3(1.19f, 1.0f, 1.19f),
	D3DXVECTOR3(1.21f, 1.0f, 1.21f),
	D3DXVECTOR3(1.24f, 1.0f, 1.24f),
	D3DXVECTOR3(1.27f, 1.0f, 1.27f),
	D3DXVECTOR3(1.3f, 1.0f, 1.3f),
	D3DXVECTOR3(1.32f, 1.0f, 1.32f),
	D3DXVECTOR3(1.34f, 1.0f, 1.34f),
	D3DXVECTOR3(1.35f, 1.0f, 1.35f),

	D3DXVECTOR3(1.0f, 1.0f,  1.25f), // �L�т�
	D3DXVECTOR3(1.0f, 1.0f,  1.5f),
	D3DXVECTOR3(1.0f, 1.0f,  1.75f),
	D3DXVECTOR3(1.0f, 1.0f,  2.0f),
	D3DXVECTOR3(1.0f, 1.0f,  2.4f),
	D3DXVECTOR3(1.0f, 1.0f,  2.8f),
	D3DXVECTOR3(1.0f, 1.0f,  3.4f),
	D3DXVECTOR3(1.0f, 1.0f,  3.8f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),
	D3DXVECTOR3(1.0f, 1.0f,  4.2f),

	D3DXVECTOR3(1.2f, 1.2f,  0.8f), // �U����
	D3DXVECTOR3(1.2f, 1.2f,  0.8f),
	D3DXVECTOR3(1.16f, 1.15f,  0.85f),
	D3DXVECTOR3(1.1f, 1.1f,  0.9f),
	D3DXVECTOR3(1.5f, 1.5f,  0.93f),
	D3DXVECTOR3(1.3f, 1.3f,  0.96f),
	D3DXVECTOR3(1.2f, 1.2f,  0.98f),
	D3DXVECTOR3(1.1f, 1.1f,  0.99f),
};


// �G�l�~�[
// �ړ����̃A�j���[�V����
static D3DXVECTOR3 g_EnemyFieldWalkAnimPos[14] =
{
	D3DXVECTOR3(0.0f, -0.05f, 0.0f), // ���˂�O
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),

	D3DXVECTOR3(0.0f, 0.35f, 0.0f),  // ���˂Ă��鎞
	D3DXVECTOR3(0.0f, 0.4f, 0.0f),
	D3DXVECTOR3(0.0f, 0.35f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.12f, 0.0f),
	D3DXVECTOR3(0.0f, 0.08f, 0.0f),
	D3DXVECTOR3(0.0f, 0.04f, 0.0f),
};
static D3DXVECTOR3 g_EnemyFieldWalkAnimScale[14] =
{
	D3DXVECTOR3(1.05f, 0.95f, 1.05f), // ���˂�O
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),
	D3DXVECTOR3(1.2f, 0.8f, 1.2f),

	D3DXVECTOR3(1.0f, 1.2f, 1.0f), // ���˂Ă��鎞
	D3DXVECTOR3(0.8f, 1.3f, 0.8f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVECTOR3(1.0f, 1.0f, 1.0f),
};

// �U�����̃A�j���[�V����
static D3DXVECTOR3 g_EnemyFieldAttackAnimPos[32] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.0f), // �U���O
	D3DXVECTOR3(0.0f, 0.0f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.15f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),

	D3DXVECTOR3(0.0f, 0.0f, -0.9f), // �U����
	D3DXVECTOR3(0.0f, 0.0f, -0.9f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
	D3DXVECTOR3(0.0f, 0.0f, 0.4f),
};
static D3DXVECTOR3 g_EnemyFieldAttackAnimScale[32] =
{
	D3DXVECTOR3(1.05f, 1.05f, 0.9f), // �U���O
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.15f, 1.15f, 0.75f),
	D3DXVECTOR3(1.15f, 1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),

	D3DXVECTOR3(0.7f,  0.7f, 2.0f), // �U����
	D3DXVECTOR3(0.7f,  0.7f, 2.0f),
	D3DXVECTOR3(0.75f, 0.75f,2.0f),
	D3DXVECTOR3(0.8f,  0.8f, 2.0f),
	D3DXVECTOR3(0.85f, 0.85f,2.0f),
	D3DXVECTOR3(0.9f,  0.9f, 2.0f),
	D3DXVECTOR3(0.95f, 0.95f,2.0f),
	D3DXVECTOR3(0.95f, 0.95f,1.8f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.3f),
	D3DXVECTOR3(0.95f, 0.95f,1.15f),
};


// ���@�r�����̃A�j���[�V����
static D3DXVECTOR3 g_PlayerMagicAnimPos[86] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // �k��
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),

	D3DXVECTOR3(0.0f, -0.2f, 0.0f),// �L�т�
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f), 
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.15f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.125f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),

	D3DXVECTOR3(0.0f, 0.0f, 0.0f), // �k��
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.15f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, -0.25f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.3f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.4f, 0.0f),
	D3DXVECTOR3(0.0f, -0.45f, 0.0f),
	D3DXVECTOR3(0.0f, -0.45f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),
	D3DXVECTOR3(0.0f, -0.5f, 0.0f),

	D3DXVECTOR3(0.0f, -0.2f, 0.0f),// �L�т�
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.1f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.14f, 0.0f),
	D3DXVECTOR3(0.0f, 0.155f, 0.0f),
	D3DXVECTOR3(0.0f, 0.17f, 0.0f),
	D3DXVECTOR3(0.0f, 0.19f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.0f, 0.23f, 0.0f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(-0.1f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.05f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(0.0f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.1f),
	D3DXVECTOR3(-0.05f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.05f),
	D3DXVECTOR3(0.05f, 0.225f, 0.0f),
	D3DXVECTOR3(0.0f, 0.225f, 0.05f),
	D3DXVECTOR3(0.0f, 0.225f, -0.1f),
	D3DXVECTOR3(0.0f, 0.015f, 0.05f),
	D3DXVECTOR3(0.05f, 0.015f, 0.0f),
	D3DXVECTOR3(-0.1f, 0.015f, 0.05f),
	D3DXVECTOR3(0.0f, 0.015f, 0.0f),
	D3DXVECTOR3(0.05f, 0.015f, -0.05f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.1f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.025f, 0.0f),
	D3DXVECTOR3(0.0f, 0.025f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0125f, 0.0f),
};
static D3DXVECTOR3 g_PlayerMagicAnimScale[86] =
{
	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // �k��
	D3DXVECTOR3(1.15f, 0.75f, 1.15f),
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.25f, 0.7f, 1.25f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.40f, 0.7f, 1.40f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),

	D3DXVECTOR3(1.45f, 0.65f, 1.45f), // �L�т�
	D3DXVECTOR3(1.25f, 0.75f, 1.25f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),    
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.7f, 1.5f, 0.7f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.75f, 1.45f, 0.75f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),

	D3DXVECTOR3(1.1f, 0.8f, 1.1f),    // �k��
	D3DXVECTOR3(1.15f, 0.75f, 1.15f),
	D3DXVECTOR3(1.2f, 0.7f, 1.2f),
	D3DXVECTOR3(1.25f, 0.7f, 1.25f),
	D3DXVECTOR3(1.35f, 0.7f, 1.35f),
	D3DXVECTOR3(1.40f, 0.7f, 1.40f),
	D3DXVECTOR3(1.45f, 0.7f, 1.45f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.55f, 0.65f, 1.55f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.65f, 0.55f, 1.65f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.70f, 0.50f, 1.70f),
	D3DXVECTOR3(1.75f, 0.50f, 1.75f),
	D3DXVECTOR3(1.75f, 0.50f, 1.75f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),
	D3DXVECTOR3(1.80f, 0.45f, 1.80f),

	D3DXVECTOR3(1.45f, 0.65f, 1.45f), // �L�т�
	D3DXVECTOR3(1.35f, 0.70f, 1.35f),
	D3DXVECTOR3(1.2f, 0.80f, 1.2f),
	D3DXVECTOR3(1.1f, 0.9f, 1.1f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.7f, 1.6f, 0.7f),
	D3DXVECTOR3(0.775f, 1.75f, 0.775f),
	D3DXVECTOR3(0.75f, 1.85f, 0.75f),
	D3DXVECTOR3(0.75f, 1.95f, 0.75f),
	D3DXVECTOR3(0.75f, 2.05f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.25f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.2f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.75f, 2.15f, 0.75f),
	D3DXVECTOR3(0.8f, 1.85f, 0.8f),
	D3DXVECTOR3(0.8f, 1.65f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.45f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.8f, 1.4f, 0.8f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.3f, 0.9f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(0.9f, 1.2f, 0.9f),
	D3DXVECTOR3(0.95f, 1.1f, 0.95f),
	D3DXVECTOR3(0.95f, 1.05f, 0.95f),
};

// ���@�U�����̃A�j���[�V����
static D3DXVECTOR3 g_EnemyMagicAttackAnimPos[54] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.0f), // �U���O
	D3DXVECTOR3(0.0f, 0.0f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.1f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.15f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
	D3DXVECTOR3(0.0f, 0.0f, -0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f,  0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.35f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // �U������
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.2f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // �U������
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.3f),
	D3DXVECTOR3(0.0f, 0.0f, 0.2f),

	D3DXVECTOR3(0.0f, 0.0f, -0.6f), // �U���O���
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.3f),
	D3DXVECTOR3(0.0f, 0.0f, -0.2f),
	D3DXVECTOR3(0.0f, 0.0f,  0.0f),
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.6f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.3f),
	D3DXVECTOR3(0.0f, 0.0f,  0.2f),
};
static D3DXVECTOR3 g_EnemyMagicAttackAnimScale[54] =
{
	D3DXVECTOR3(1.05f, 1.05f, 0.9f), // �U���O
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.05f, 1.05f, 0.85f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.1f,  1.1f, 0.8f),
	D3DXVECTOR3(1.15f, 1.15f, 0.75f),
	D3DXVECTOR3(1.15f, 1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),
	D3DXVECTOR3(1.15f,  1.15f, 0.7f),
	D3DXVECTOR3(1.2f,  1.2f, 0.65f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // �U������
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // �U������
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),

	D3DXVECTOR3(0.7f,  0.7f, 1.65f), // �U���O���
	D3DXVECTOR3(0.7f,  0.7f, 1.65f),
	D3DXVECTOR3(0.75f, 0.75f,1.65f),
	D3DXVECTOR3(0.8f,  0.8f, 1.65f),
	D3DXVECTOR3(0.85f, 0.85f,1.6f),
	D3DXVECTOR3(0.9f,  0.9f, 1.55f),
	D3DXVECTOR3(0.95f, 0.95f,1.5f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(0.95f, 0.95f,1.45f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
	D3DXVECTOR3(1.0f, 1.0f,1.4f),
};


// �X�e�[�W�ɔz�u�����̍��W���X�g
static D3DXVECTOR3 m_RockPositionList[] =
{
	D3DXVECTOR3(-3.0f, 2.0f, -10.0f),
	D3DXVECTOR3(50.0f, 4.0f, 12.0f),

	D3DXVECTOR3(118.0f, 8.0f, 64.0f), // �Ǌ�
	D3DXVECTOR3(123.0f, 8.0f, 68.0f),
	D3DXVECTOR3(120.0f, 8.0f, 72.0f),
	D3DXVECTOR3(128.0f, 8.0f, 76.0f),
	D3DXVECTOR3(119.5f, 8.0f, 80.0f),
	D3DXVECTOR3(122.0f, 8.0f, 84.0f),
	D3DXVECTOR3(120.0f, 8.0f, 88.0f),
	D3DXVECTOR3(116.5f, 8.0f, 92.0f),
	D3DXVECTOR3(120.0f, 8.0f, 95.0f),

	D3DXVECTOR3(168.0f, 8.0f, 150.0f),
};

// �X�e�[�W�ɔz�u����؂̍��W���X�g
static D3DXVECTOR3 m_TreePositionList[] =
{
	D3DXVECTOR3(-17.0f, 3.0f, -16.0f),
	D3DXVECTOR3(-5.0f, 3.0f, 18.0f),
	D3DXVECTOR3(38.0f, 3.0f, -10.0f),
	D3DXVECTOR3(28.0f, 6.0f, 58.0f),
	D3DXVECTOR3(68.0f, 6.0f, 38.0f),
	D3DXVECTOR3(78.0f, 7.5f, 98.0f),
	D3DXVECTOR3(179.0f, 7.5f, 64.0f),
	D3DXVECTOR3(142.0f, 7.5f, 124.0f),

	D3DXVECTOR3(142.0f, 7.5f, 198.0f), // ��������Ŋ��̓�
	D3DXVECTOR3(176.0f, 7.5f, 187.0f),
	D3DXVECTOR3(144.0f, 7.5f, 177.0f),
	D3DXVECTOR3(177.0f, 7.5f, 173.0f),

	D3DXVECTOR3(143.0f, 7.5f, 225.0f),
	D3DXVECTOR3(177.0f, 7.5f, 225.0f),

	D3DXVECTOR3(145.0f, 7.5f, 229.0f),
	D3DXVECTOR3(175.0f, 7.5f, 229.0f),

	D3DXVECTOR3(144.0f, 7.5f, 233.0f),
	D3DXVECTOR3(176.0f, 7.5f, 233.0f),
	D3DXVECTOR3(170.0f, 7.5f, 235.0f),

	D3DXVECTOR3(147.0f, 7.5f, 237.0f),
	D3DXVECTOR3(173.0f, 7.5f, 237.0f),

	D3DXVECTOR3(145.0f, 7.5f, 241.0f),
	D3DXVECTOR3(175.0f, 7.5f, 241.0f),

	D3DXVECTOR3(146.0f, 7.5f, 245.0f),
	D3DXVECTOR3(174.0f, 7.5f, 245.0f),

	D3DXVECTOR3(149.0f, 7.5f, 249.0f),
	D3DXVECTOR3(171.0f, 7.5f, 249.0f),
	D3DXVECTOR3(144.0f, 7.5f, 250.0f),

	D3DXVECTOR3(147.0f, 7.5f, 253.0f),
	D3DXVECTOR3(173.0f, 7.5f, 253.0f),

	D3DXVECTOR3(148.0f, 7.5f, 257.0f),
	D3DXVECTOR3(172.0f, 7.5f, 257.0f),
	D3DXVECTOR3(142.0f, 7.5f, 257.0f),
	D3DXVECTOR3(178.0f, 7.5f, 257.0f),

	D3DXVECTOR3(150.0f, 7.5f, 261.0f),
	D3DXVECTOR3(170.0f, 7.5f, 261.0f),

	D3DXVECTOR3(151.0f, 7.5f, 265.0f),
	D3DXVECTOR3(169.0f, 7.5f, 265.0f),

	D3DXVECTOR3(149.0f, 7.5f, 269.0f),
	D3DXVECTOR3(171.0f, 7.5f, 269.0f),
	D3DXVECTOR3(143.0f, 7.5f, 269.0f),
	D3DXVECTOR3(177.0f, 7.5f, 269.0f),

	D3DXVECTOR3(150.0f, 7.5f, 273.0f),
	D3DXVECTOR3(170.0f, 7.5f, 273.0f),
	D3DXVECTOR3(145.0f, 7.5f, 273.0f),
	D3DXVECTOR3(175.0f, 7.5f, 273.0f),

	D3DXVECTOR3(153.0f, 7.5f, 277.0f),
	D3DXVECTOR3(167.0f, 7.5f, 277.0f),

	D3DXVECTOR3(152.0f, 7.5f, 281.0f),
	D3DXVECTOR3(168.0f, 7.5f, 281.0f),
	D3DXVECTOR3(144.0f, 7.5f, 281.0f),
	D3DXVECTOR3(176.0f, 7.5f, 281.0f),

	D3DXVECTOR3(150.0f, 7.5f, 285.0f),
	D3DXVECTOR3(170.0f, 7.5f, 285.0f),
	D3DXVECTOR3(142.0f, 7.5f, 285.0f),
	D3DXVECTOR3(178.0f, 7.5f, 285.0f),

	D3DXVECTOR3(153.0f, 7.5f, 289.0f),
	D3DXVECTOR3(167.0f, 7.5f, 289.0f),

	D3DXVECTOR3(154.0f, 7.5f, 293.0f),
	D3DXVECTOR3(166.0f, 7.5f, 293.0f),
	D3DXVECTOR3(145.0f, 7.5f, 293.0f),
	D3DXVECTOR3(175.0f, 7.5f, 293.0f),

	D3DXVECTOR3(152.0f, 7.5f, 297.0f),
	D3DXVECTOR3(168.0f, 7.5f, 297.0f),
	D3DXVECTOR3(143.0f, 7.5f, 297.0f),
	D3DXVECTOR3(177.0f, 7.5f, 297.0f),

	D3DXVECTOR3(155.0f, 7.5f, 301.0f),
	D3DXVECTOR3(165.0f, 7.5f, 301.0f),
	D3DXVECTOR3(145.0f, 7.5f, 302.0f),
	D3DXVECTOR3(175.0f, 7.5f, 302.0f),
	D3DXVECTOR3(150.0f, 7.5f, 302.0f),
	D3DXVECTOR3(170.0f, 7.5f, 302.0f),
	D3DXVECTOR3(152.0f, 7.5f, 303.0f),
	D3DXVECTOR3(168.0f, 7.5f, 303.0f),
	D3DXVECTOR3(156.0f, 7.5f, 303.0f),
	D3DXVECTOR3(164.0f, 7.5f, 303.0f),
	D3DXVECTOR3(160.0f, 7.5f, 305.0f),

	D3DXVECTOR3(143.0f, 7.5f, 308.0f),
	D3DXVECTOR3(148.0f, 7.5f, 308.0f),
	D3DXVECTOR3(152.0f, 7.5f, 308.0f),
	D3DXVECTOR3(157.0f, 7.5f, 308.0f),
	D3DXVECTOR3(162.0f, 7.5f, 308.0f),
	D3DXVECTOR3(172.0f, 7.5f, 308.0f),
	D3DXVECTOR3(177.0f, 7.5f, 308.0f),

	D3DXVECTOR3(142.0f, 7.5f, 311.0f),
	D3DXVECTOR3(146.0f, 7.5f, 311.0f),
	D3DXVECTOR3(155.0f, 7.5f, 311.0f),
	D3DXVECTOR3(160.0f, 7.5f, 311.0f),
	D3DXVECTOR3(165.0f, 7.5f, 311.0f),
	D3DXVECTOR3(170.0f, 7.5f, 311.0f),
	D3DXVECTOR3(175.0f, 7.5f, 311.0f),
	D3DXVECTOR3(178.0f, 7.5f, 311.0f),

	D3DXVECTOR3(143.0f, 7.5f, 315.0f),
	D3DXVECTOR3(148.0f, 7.5f, 315.0f),
	D3DXVECTOR3(152.0f, 7.5f, 315.0f),
	D3DXVECTOR3(157.0f, 7.5f, 315.0f),
	D3DXVECTOR3(162.0f, 7.5f, 315.0f),
	D3DXVECTOR3(167.0f, 7.5f, 315.0f),
	D3DXVECTOR3(172.0f, 7.5f, 315.0f),

	D3DXVECTOR3(142.0f, 7.5f, 320.0f),
	D3DXVECTOR3(146.0f, 7.5f, 320.0f),
	D3DXVECTOR3(150.0f, 7.5f, 320.0f),
	D3DXVECTOR3(155.0f, 7.5f, 320.0f),
	D3DXVECTOR3(165.0f, 7.5f, 320.0f),
	D3DXVECTOR3(170.0f, 7.5f, 320.0f),
	D3DXVECTOR3(175.0f, 7.5f, 320.0f),
	D3DXVECTOR3(178.0f, 7.5f, 320.0f),

	D3DXVECTOR3(148.0f, 7.5f, 326.0f),
	D3DXVECTOR3(152.0f, 7.5f, 326.0f),
	D3DXVECTOR3(157.0f, 7.5f, 326.0f),
	D3DXVECTOR3(162.0f, 7.5f, 326.0f),
	D3DXVECTOR3(167.0f, 7.5f, 326.0f),
	D3DXVECTOR3(172.0f, 7.5f, 326.0f),
	D3DXVECTOR3(177.0f, 7.5f, 326.0f),

	D3DXVECTOR3(142.0f, 7.5f, 332.0f),
	D3DXVECTOR3(150.0f, 7.5f, 332.0f),
	D3DXVECTOR3(155.0f, 7.5f, 332.0f),
	D3DXVECTOR3(160.0f, 7.5f, 332.0f),
	D3DXVECTOR3(165.0f, 7.5f, 332.0f),
	D3DXVECTOR3(175.0f, 7.5f, 332.0f),
	D3DXVECTOR3(178.0f, 7.5f, 332.0f),
};

// �X�e�[�W�ɔz�u����n�ʂƑ����̍��W���X�g
static D3DXVECTOR3 m_GroundGrassPositionList[] =
{
	D3DXVECTOR3(0.0f, 3.0f, 0.0f),
	D3DXVECTOR3(40.0f, 3.0f, 0.0f),
	D3DXVECTOR3(40.0f, 6.0f, 40.0f),
	D3DXVECTOR3(80.0f, 6.0f, 40.0f),
	D3DXVECTOR3(80.0f, 7.5f, 80.0f),
	D3DXVECTOR3(120.0f, 7.5f, 80.0f),
	D3DXVECTOR3(160.0f, 7.5f, 80.0f),
	D3DXVECTOR3(160.0f, 7.5f, 120.0f),
	D3DXVECTOR3(160.0f, 7.5f, 160.0f),
	D3DXVECTOR3(160.0f, 7.5f, 200.0f),
	D3DXVECTOR3(160.0f, 7.5f, 240.0f),
	D3DXVECTOR3(160.0f, 7.5f, 280.0f),
	D3DXVECTOR3(160.0f, 7.5f, 320.0f),
};