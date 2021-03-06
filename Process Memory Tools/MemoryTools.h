#pragma once
//
//       ██████╗ ██████╗ ██████╗ ███████╗    ██████╗ ██╗   ██╗██╗██╗     ██████╗ ██╗███╗   ██╗ ██████╗ 
//      ██╔════╝██╔═══██╗██╔══██╗██╔════╝    ██╔══██╗██║   ██║██║██║     ██╔══██╗██║████╗  ██║██╔════╝ 
//      ██║     ██║   ██║██║  ██║█████╗      ██████╔╝██║   ██║██║██║     ██║  ██║██║██╔██╗ ██║██║  ███╗
//      ██║     ██║   ██║██║  ██║██╔══╝      ██╔══██╗██║   ██║██║██║     ██║  ██║██║██║╚██╗██║██║   ██║
//      ╚██████╗╚██████╔╝██████╔╝███████╗    ██████╔╝╚██████╔╝██║███████╗██████╔╝██║██║ ╚████║╚██████╔╝
//       ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    ╚═════╝  ╚═════╝ ╚═╝╚══════╝╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
//
//											Process Memory Tools C++
//		FEATURES
//		• Suporte Nativo a processos x86 e x64 (automatic)
//		• Listagem de Processos em execução no sistema					- ListProcess();
//		• Listagem de Modulos de um Processo especificado				- ListProcessModules(DWORD _ProcessID)
//		• Listagem de Threads de um Processo especificado				- ListThreads(DWORD _ProcessID)
//		• Get ID de um Processo especificado (Comumente chamado de PID)	- GetProcessID(const char _ProcessName[])
//		• Get Handle de um Processo especificado						- GetProcessHandle(DWORD _ProcessID)
//		• Get base address de um Modulo especificado					- GetModuleBaseAddress(DWORD _ProcessID, const char _ModuleName[])
//		• Get tamanho de um Modulo especificado							- GetModuleBaseSize(DWORD _ProcessID, const char _ModuleName[])
//		• Get base address de um Ponteiro especificado					- GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, vector<uintptr_t> &_Offsets)
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Suspende a execução das Threads de um Processo especificado	- void SuspendThreads(DWORD _ProcessID)
//		• Retoma a execução das Threads de um Processo especificado		- ResumeThreads(DWORD _ProcessID)
//		• Read Process Memory v1.0										- ReadProcess(DWORD _ProcessID, uintptr_t _BaseAddress) OR ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) 
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Write Process Memory v1.0										- WriteProcess(DWORD _ProcessID, uintptr_t _BaseAddress, uintptr_t _buffer) OR WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, uintptr_t _buffer)
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//
//		CRÉDITOS:
//		Programador						-> Guilherme Lima
//		Mentor							-> SmoLL_iCe
//		Solucionador de dúvidas e erros -> Define, Devecchi, Luan Devecchi, EuPoder, Bolard, Walison.
//

//      
//      ██╗███╗   ██╗ ██████╗██╗     ██╗   ██╗██████╗ ███████╗███████╗
//      ██║████╗  ██║██╔════╝██║     ██║   ██║██╔══██╗██╔════╝██╔════╝
//      ██║██╔██╗ ██║██║     ██║     ██║   ██║██║  ██║█████╗  ███████╗
//      ██║██║╚██╗██║██║     ██║     ██║   ██║██║  ██║██╔══╝  ╚════██║
//      ██║██║ ╚████║╚██████╗███████╗╚██████╔╝██████╔╝███████╗███████║
//      ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝ ╚═════╝ ╚═════╝ ╚══════╝╚══════╝
#include "pch.h"
#include <Windows.h>
#include <tlhelp32.h> // Pega Informações de Processos em execução no Windows - https://docs.microsoft.com/en-us/windows/desktop/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
#include <thread>
#include <vector>
#include <iostream> // 01 Processos: Pega Informações de Processos em execução no Windows - https://docs.microsoft.com/en-us/windows/desktop/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot

//
//      ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
//      ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
//      ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
//      ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
//       ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
//        ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝                                                                                                                                                                                                                                      
//                              - Program Settings -

using namespace std;

#pragma region Prototipagem 
template<typename T> T ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress);
uintptr_t GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, vector<uintptr_t>& _Offsets);
#pragma endregion

//
//      ██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗     ███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
//      ██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
//      ███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    █████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
//      ██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
//      ██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
//      ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝


//		• Listagem de processos em execução no sistema
void ListProcess() {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // dwFlagListarProcessos
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			printf("ID: %d  \tNome: %s \n", PE.th32ProcessID, PE.szExeFile);
			Retorno = Process32Next(HandleSnapshot, &PE);
		}
	}
	CloseHandle(HandleSnapshot);
}

//		• Listagem de Modulos em execução de um Processo
void ListProcessModules(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // #dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			printf("Nome: %s \t\tBase: %I64X \t\tSize: %d\n", ME.szModule, ME.modBaseAddr, ME.modBaseSize);
			Retorno = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
}

//		• Listagem de Threads em execução de um Processo especificado
void ListThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				printf("Thread ID: %d\n", TE.th32ThreadID);
			}
			Retorno = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

//		• Get ID do Processo especificado
DWORD GetProcessID(const char _ProcessName[])
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // dwFlagListarProcessos
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL Retorno = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (_tcscmp(PE.szExeFile, _ProcessName) == 0) {
				CloseHandle(HandleSnapshot);
				return PE.th32ProcessID;
			}
			Retorno = Process32Next(HandleSnapshot, &PE);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}

//		• Get Handle Process
HANDLE GetProcessHandle(DWORD _ProcessID)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ProcessID); // dwDesiredAccess
}

//		• Get base address de um Modulo especificado
uintptr_t GetModuleBaseAddress(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseAddr;
			}
			Retorno = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}

//		• Get tamanho de um Modulo especificado
uintptr_t GetModuleBaseSize(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL Retorno = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseSize;
			}
			Retorno = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}

//		• Get base address de um Ponteiro especificado
uintptr_t GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, const vector<uintptr_t>& _Offsets) {
	uintptr_t Retorno = 0;
	int tamanho = _Offsets.size();

	uintptr_t MooduleBase = GetModuleBaseAddress(_ProcessID, _MooduleBase);
	//printf("Quantidade do array de OffSets: %d\n", tamanho);
	Retorno = ReadProcess<uintptr_t>(_ProcessHandle, MooduleBase + _Pointer);
	tamanho--;
	//printf("module + pointer %I64X  \n", Retorno);
	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			Retorno = ReadProcess<uintptr_t>(_ProcessHandle, Retorno + _Offsets[i]);
			//	printf("OffSet%d: %I64X  \n", i + 1, Retorno);
		}
		else
		{
			Retorno = Retorno + _Offsets[i];
			//printf("OffSet%d: %I64X  \n", i + 1, Retorno);
			//return Retorno;
			//return Retorno = Retorno + _Offsets[i];
		}
	}
	return Retorno;
}
uintptr_t GetPointerBaseAddress(DWORD _ProcessID, const char _MooduleBase[], uintptr_t _Pointer, const vector<uintptr_t> & _Offsets) {
	uintptr_t Retorno = 0;
	int tamanho = _Offsets.size();
	HANDLE processHandle = GetProcessHandle(_ProcessID);

	uintptr_t MooduleBase = GetModuleBaseAddress(_ProcessID, _MooduleBase);
	//printf("Quantidade do array de OffSets: %d\n", tamanho);
	Retorno = ReadProcess<uintptr_t>(processHandle, MooduleBase + _Pointer);
	tamanho--;
	//printf("module + pointer %I64X  \n", Retorno);
	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			Retorno = ReadProcess<uintptr_t>(processHandle, Retorno + _Offsets[i]);
			//	printf("OffSet%d: %I64X  \n", i + 1, Retorno);
		}
		else
		{
			Retorno = Retorno + _Offsets[i];
			//printf("OffSet%d: %I64X  \n", i + 1, Retorno);
			//return Retorno;
			//return Retorno = Retorno + _Offsets[i];
		}
	}
	CloseHandle(processHandle); // Fecha o Handle aberto
	return Retorno;
}

//		• Suspende a execução das Threads de um Processo especificado
void SuspendThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				SuspendThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			Retorno = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

//		• Retoma a execução das Threads de um Processo especificado
void ResumeThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL Retorno = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (Retorno)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				ResumeThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			Retorno = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

//		• Read Process Memory
template<typename T> T ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) {
	T Retorno = 0;
	SIZE_T NumberOfBytesToRead = sizeof(Retorno);
	SIZE_T NumberOfBytesActuallyRead;
	BOOL err = ReadProcessMemory(_ProcessHandle, (LPCVOID)_BaseAddress, &Retorno, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
	if (err || NumberOfBytesActuallyRead != NumberOfBytesToRead)
		return Retorno;
}
template<typename T> T ReadProcess(DWORD _ProcessID, uintptr_t _BaseAddress) {
	T Retorno = 0;
	HANDLE processHandle = GetProcessHandle(_ProcessID);

	SIZE_T NumberOfBytesToRead = sizeof(Retorno);
	SIZE_T NumberOfBytesActuallyRead;
	BOOL err = ReadProcessMemory(processHandle, (LPCVOID)_BaseAddress, &Retorno, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
	if (err || NumberOfBytesActuallyRead != NumberOfBytesToRead)
		CloseHandle(processHandle); // Fecha o Handle aberto
	return Retorno;
}
//		• Write Process Memory
template<typename T> void WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, T _buffer) {
	//SIZE_T NumberOfBytesToRead = 4;
	SIZE_T NumberOfBytesToRead = sizeof(_buffer);
	WriteProcessMemory(_ProcessHandle, (LPVOID)_BaseAddress, &_buffer, NumberOfBytesToRead, NULL);
}
template<typename T> void WriteProcess(DWORD _ProcessID, uintptr_t _BaseAddress, T _buffer) {
	//SIZE_T NumberOfBytesToRead = 4;
	HANDLE processHandle = GetProcessHandle(_ProcessID);
	SIZE_T NumberOfBytesToRead = sizeof(_buffer);
	WriteProcessMemory(processHandle, (LPVOID)_BaseAddress, &_buffer, NumberOfBytesToRead, NULL);
	CloseHandle(processHandle); // Fecha o Handle aberto
}

template<typename T> bool  WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, T * _buffer, SIZE_T _NumberOfBytesToRead) {
	return WriteProcessMemory(_ProcessHandle, (LPVOID)_BaseAddress, _buffer, _NumberOfBytesToRead, NULL);
}
template<typename T> bool  WriteProcess(DWORD _ProcessID, uintptr_t _BaseAddress, T * _buffer, SIZE_T _NumberOfBytesToRead) {
	HANDLE processHandle = GetProcessHandle(_ProcessID);
	bool retorno = WriteProcessMemory(processHandle, (LPVOID)_BaseAddress, _buffer, _NumberOfBytesToRead, NULL);
	CloseHandle(processHandle); // Fecha o Handle aberto
	return retorno;
}

