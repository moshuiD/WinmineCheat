#pragma once
#include<Windows.h>
#include<TlHelp32.h>
#include<string>
#include<memory>
namespace Tools {
	/// <summary>
	/// Get pid by process name  *need complete process name(included ".exe")
	/// </summary>
	/// <param name="exeName">process name</param>
	/// <returns>if can't get procressID will return -1</returns>
	static DWORD GetPidByName(const wchar_t* exeName)
	{
		HANDLE procSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 procEntry{};
		procEntry.dwSize = sizeof(procEntry);
		for (bool ok = Process32First(procSnapShot, &procEntry); ok; ok = Process32Next(procSnapShot, &procEntry))
		{
#ifdef _DEBUG
			OutputDebugString((std::wstring(procEntry.szExeFile) + std::wstring(L"\n")).c_str());
#endif
			if (wcscmp(exeName, procEntry.szExeFile) == 0) {
				CloseHandle(procSnapShot);
				return procEntry.th32ProcessID;
			}
		}
		CloseHandle(procSnapShot);
		return -1;
	}
	/// <summary>
	/// Get module base address
	/// </summary>
	/// <param name="pid"></param>
	/// <param name="moduleName">module complete name</param>
	/// <returns>if can't get module addr will return -1</returns>
	static DWORD GetModuleBaseAddr(DWORD pid, const wchar_t* moduleName)
	{
		MODULEENTRY32 moduleEntry{};
		moduleEntry.dwSize = sizeof(moduleEntry);
		HANDLE moduleSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		for (bool ok = Module32First(moduleSnapShot, &moduleEntry); ok; ok = Module32Next(moduleSnapShot, &moduleEntry))
		{
#ifdef _DEBUG
			OutputDebugString((std::wstring(moduleEntry.szModule) + std::wstring(L"\n")).c_str());
#endif
			if (wcscmp(moduleName, moduleEntry.szModule) == 0) {
				CloseHandle(moduleSnapShot);
				return (DWORD)(moduleEntry.modBaseAddr);
			}
		}
		CloseHandle(moduleSnapShot);
		return -1;
	}
	class Mem
	{
	public:
		Mem(DWORD pid) 
		{
			m_TargetHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			if (m_TargetHandle == INVALID_HANDLE_VALUE) {
				m_TargetHandle = (HANDLE)-1;
				printf("[-] Error: Not a VALID_HANDLE_VALUE for process\n");
			}
		};
		~Mem()
		{
			CloseHandle(m_TargetHandle);
		};
		Mem& operator=(const Mem&) { return *this; };
	private:
		HANDLE m_TargetHandle;
	public:
		template<class T>
		T ReadMem(DWORD addr)
		{
			T buffer{};
			size_t readNums = 0;
			ReadProcessMemory(m_TargetHandle, (LPVOID)addr, &buffer, sizeof(buffer), &readNums);
			return buffer;
		};

		std::shared_ptr<byte> ReadMem(DWORD addr, size_t len) 
		{
			std::shared_ptr<byte> buffer(new byte[len]);
			size_t readNums = 0;
			ReadProcessMemory(m_TargetHandle, (LPVOID)addr, buffer.get(), len, &readNums);
			return buffer;
		}
		template<class T>
		bool WriteMem(LPVOID addr, T buffer) 
		{
			size_t writeNums = 0;
			return WriteProcessMemory(m_TargetHandle, addr, &buffer, sizeof(buffer), &writeNums);
		}
	};
}

