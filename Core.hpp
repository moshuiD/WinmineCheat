#pragma once
#include"Tools.hpp"
#include"Offsetes.h"
#include<vector>
#include<utility>
class Core
{
	template<class T>
	using vector = std::vector<T>;

	using BombMap = std::pair<int, int>;//x,y

	template<class T>
	using shared_ptr = std::shared_ptr<T>;
	
private:
	Tools::Mem* m;
	const DWORD m_Pid;
	const DWORD m_ModuleAddr;
public:
	Core& operator=(const Core& m) {
		return *this;
	}
	Core(DWORD pid, DWORD moduleAddr) :
		m_Pid(pid), m_ModuleAddr(moduleAddr)
	{
		m = new Tools::Mem(pid);
	};
	~Core()
	{
		delete m;
		m = nullptr;
	};

	__forceinline vector<BombMap>GetAllBombs();
	__forceinline bool SetTimerDelay(int delay);
};


Core::vector<Core::BombMap> Core::GetAllBombs()
{
	int maxX = m->ReadMem<int>(m_ModuleAddr + Offsets::MaxX);
	int maxY = m->ReadMem<int>(m_ModuleAddr + Offsets::MaxY);
	shared_ptr<byte> map = m->ReadMem(m_ModuleAddr + Offsets::BombMap, 32 * maxY + maxX);
	vector<BombMap> RetBombsMap;
	for (int y = 0; y <= maxY; y++)
	{
		for (int x = 0; x <= maxX; x++)
		{
			if (map.get()[32 * y + x] == 0x8F) {
				RetBombsMap.push_back({ x,y });
			}
		}
	}

	return RetBombsMap;
}

inline bool Core::SetTimerDelay(int delay)
{
	if (!m->ReadMem<byte>(m_ModuleAddr + Offsets::IsGameStart))
		return m->WriteMem((LPVOID)(m_ModuleAddr + Offsets::TimerDelay), delay);
	else
		return false;
}
