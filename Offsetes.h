#pragma once
#include <Windows.h>
namespace Offsets {
	static DWORD constexpr BombMap = 0x5340;
	static DWORD constexpr MaxX = 0x5334;
	static DWORD constexpr MaxY = 0x5338;
	static DWORD constexpr TimerDelay = 0x383E;
	static DWORD constexpr IsGameStart = 0x5164;
	static DWORD constexpr MarkedBombsNums = 0x5164;//unused
	static DWORD constexpr BombsSum = 0x56A4;//unused
}