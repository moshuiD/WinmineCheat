#include<iostream>
#include"Core.hpp"
#include<utility>
#include<algorithm>
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
static void Menu()
{
	printf("***********************************************************\n");
	printf("*                    WinMineSweeper Cheat                 *\n");
	printf("*                        from moshui                      *\n");
	printf("*                                                         *\n");
	printf("*                https://github.com/moshuiD               *\n");
	printf("***********************************************************\n");

	printf("[+] Init done.\n");
	printf("[i] Press \"F1\" to get all bombers position.\n");
	printf("[i] Press \"F2\" to set timer delay\n");
	printf("[i] Press \"F5\" to refresh screen\n");
}
void MainThread()
{
	DWORD pid{};
	DWORD moduleAddr{};
	while (!Init(&pid, &moduleAddr));

	Menu();
	Core c(pid, moduleAddr);
	while (true)
	{
		if (GetAsyncKeyState(VK_F5) & 1) {
			system("cls");
			Menu();
		}
		if (GetAsyncKeyState(VK_F1) & 1) {
			auto BombsMap = c.GetAllBombs();
			printf("[+] Found bombs at:\n");
			std::sort(BombsMap.begin(), BombsMap.end(), [](std::pair<int, int> map1, std::pair<int, int> map2) {
				return map1.first < map2.first;
				});
			for (const auto& item : BombsMap)
			{
				printf("X:%d Y:%d \n", item.first, item.second);
			}
			printf("\n");
		}
		if(GetAsyncKeyState(VK_F2)&1){
			printf("[+]Input delay (ms)\n");
			char szDelay[1024];
			gets_s(szDelay, 1024);
			c.SetTimerDelay(atoi(szDelay));
		}
		Sleep(1);
	}

}

int main()
{

	MainThread();
}