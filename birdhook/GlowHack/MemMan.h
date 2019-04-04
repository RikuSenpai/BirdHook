#pragma once
#include <Windows.h>
#include <vector>

class MemMan
{
public:
	MemMan();
	~MemMan();
	template <class t>
	t readMem(uintptr_t addr)
	{
		t x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}
	template <class t>
	t writeMem(uintptr_t addr, t x)
	{
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

private:
	HANDLE handle;
};

extern MemMan MemClass;