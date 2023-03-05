#include<iostream>
#include"Core.hpp"
#include<utility>
bool Init(DWORD* const outPid, DWORD* const outModuleAddr)
{
	system("cls");
	DWORD pid = Tools::GetPidByName(L"Winmine__XP.exe");
	if (!pid) {
		printf("[-] Can not find target EXE...\n");
		printf("[i] Need started target process and press any key to retry..\n");
		getchar();
		return  false;
	}
	DWORD moduleAddr = Tools::GetModuleBaseAddr(pid, L"Winmine__XP.exe");
	if (moduleAddr == -1) {
		printf("[-] Can not find main module...\n");
		printf("[i] Press any key to retry..\n");
		getchar();
		return false;
	}
	*outPid = pid;
	*outModuleAddr = moduleAddr;
	return true;
}

void MainThread()
{
	DWORD pid{};
	DWORD moduleAddr{};
	while (!Init(&pid, &moduleAddr));

	printf("***********************************************************\n");
	printf("*                    WinMineSweeper Cheat                 *\n");
	printf("*                        from moshui                      *\n");
	printf("*                                                         *\n");
	printf("*                https://github.com/moshuiD               *\n");
	printf("***********************************************************\n");

	printf("[+] Init done.\n");
	printf("[i] Press \"F1\" to get all bombers position.\n");

	if (GetAsyncKeyState(VK_F1) & 1) {
		auto BombsMap = Core::GetInstance(pid, moduleAddr).GetAllBombs();
	}
}

int main()
{

	MainThread();
}