#include <vector>

struct PlayerDataStorage
{
	std::vector<D3DXVECTOR3> WalkAnimPos;
	std::vector<D3DXVECTOR3> WalkAnimScale;

	std::vector<D3DXVECTOR3> JumpAnimPos;
	std::vector<D3DXVECTOR3> JumpAnimScale;

	std::vector<D3DXVECTOR3> PressAttackAnimPos;
	std::vector<D3DXVECTOR3> PressAttackAnimScale;

	std::vector<D3DXVECTOR3> DashAttackAnimPos;
	std::vector<D3DXVECTOR3> DashAttackAnimScale;

	std::vector<D3DXVECTOR3> MagicAnimPos;
	std::vector<D3DXVECTOR3> MagicAnimScale;
};

struct EnemyFieldDataStorage
{
	std::vector<D3DXVECTOR3> WalkAnimPos;
	std::vector<D3DXVECTOR3> WalkAnimScale;

	std::vector<D3DXVECTOR3> AttackAnimPos;
	std::vector<D3DXVECTOR3> AttackAnimScale;

	std::vector<D3DXVECTOR3> MagicAttackAnimPos;
	std::vector<D3DXVECTOR3> MagicAttackAnimScale;
};

struct AreaDataStorage
{
	std::vector<D3DXVECTOR3> RockPos;
	std::vector<D3DXVECTOR3> TreePos;
	std::vector<D3DXVECTOR3> GroundGrassPos;
	
	std::vector<D3DXVECTOR3> EnemyPos;
	std::vector<D3DXVECTOR3> EnemyMagicPos;
	std::vector<D3DXVECTOR3> EnemyStonePos;
};

class DataStorage
{
private:
	static PlayerDataStorage m_PlayerDataStorage;
	static EnemyFieldDataStorage m_EnemyFieldDataStorage;
	static AreaDataStorage m_TutorialDataStorage;
	static AreaDataStorage m_TheLandOfBeginningsDataStorage;
	static AreaDataStorage m_TheCrystalFountainheadDataStorage;
	static AreaDataStorage m_TheSkyIslandDataStorage;

public:
	static PlayerDataStorage* GetPlayerDataStorage() { return &m_PlayerDataStorage; }
	static EnemyFieldDataStorage* GetEnemyFieldDataStorage() { return &m_EnemyFieldDataStorage; }
	static AreaDataStorage* GetTutorialDataStorage() { return &m_TutorialDataStorage; }
	static AreaDataStorage* GetTheLandOfBeginningsDataStorage() { return &m_TheLandOfBeginningsDataStorage; }
	static AreaDataStorage* GetTheCrystalFountainheadDataStorage() { return &m_TheCrystalFountainheadDataStorage; }
	static AreaDataStorage* GetTheSkyIslandDataStorage() { return &m_TheSkyIslandDataStorage; }
};

inline PlayerDataStorage     DataStorage::m_PlayerDataStorage{};
inline EnemyFieldDataStorage DataStorage::m_EnemyFieldDataStorage{};
inline AreaDataStorage       DataStorage::m_TutorialDataStorage{};
inline AreaDataStorage       DataStorage::m_TheLandOfBeginningsDataStorage{};
inline AreaDataStorage       DataStorage::m_TheCrystalFountainheadDataStorage{};
inline AreaDataStorage       DataStorage::m_TheSkyIslandDataStorage{};