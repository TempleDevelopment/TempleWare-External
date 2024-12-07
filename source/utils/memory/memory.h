#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>

enum MODE : int
{
	WINDOW,
	PROCESS
};

class Memory
{
private:
	DWORD PID;

	uintptr_t GetModuleBase(const std::string moduleName);
	PROCESSENTRY32 GetProcess(const std::string processName);
public:
	HANDLE pHandle;
	uintptr_t g_BaseAddress;

	bool AttachProcess(const std::string targetName, int mode);
	void DetachProcess();

	template <typename T>
	constexpr const T Read(const uintptr_t& address) const noexcept
	{
		T value{};
		ReadProcessMemory(pHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	constexpr void Write(const uintptr_t& address, const T& value) const noexcept
	{
		WriteProcessMemory(pHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
	bool ReadString(uintptr_t address, LPVOID buffer, SIZE_T size) const
	{
		SIZE_T size_read;
		return !!::ReadProcessMemory(pHandle, LPCVOID(address), buffer, size, &size_read) && size_read > 0;
	}
};

extern Memory m;