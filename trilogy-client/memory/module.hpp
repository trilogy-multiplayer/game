#pragma once

#include <windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <thread>
#include <chrono>
#include <winternl.h>
#include <algorithm>
#include <utility>
#include <optional>
#include <intrin.h>
#include <iostream>

namespace memory
{
	class module_t
	{
	public:
		explicit module_t(HMODULE mod) :
			m_start(reinterpret_cast<uintptr_t>(mod))
		{
			auto module_dos_header = PIMAGE_DOS_HEADER(mod);
			auto nt_file_header = PIMAGE_NT_HEADERS(PBYTE(mod) + module_dos_header->e_lfanew);

			m_end = nt_file_header->OptionalHeader.SizeOfImage;
		}

		explicit module_t(std::pair<HMODULE, size_t> module) :
			m_start(reinterpret_cast<uintptr_t>(module.first)),
			m_end(module.second)
		{}

		explicit module_t(std::string mod) :
			module_t(peb_get_module(std::wstring(mod.begin(), mod.end())))
		{ }

		explicit module_t(const wchar_t* mod) :
			module_t(peb_get_module(mod))
		{ }

		explicit module_t(std::nullptr_t) :
			module_t(GetModuleHandleW(nullptr))
		{ }

		explicit module_t() :
			module_t(GetModuleHandleW(nullptr))
		{ }

		uintptr_t base()
		{
			return m_start;
		}

		std::size_t size()
		{
			return m_end;
		}

		inline static std::pair<HMODULE, size_t> peb_get_module(std::optional<std::wstring> name)
		{
			auto peb = reinterpret_cast<PEB*>(__readgsqword(0x60));

			auto head = peb->Ldr->InMemoryOrderModuleList;
			auto curr = head;

			if (!name)
			{
				wchar_t file_name[MAX_PATH];
				if (GetModuleFileNameW(GetModuleHandleW(nullptr), file_name, MAX_PATH))
					*name = file_name;
			}

			while (curr.Flink != head.Blink)
			{
				auto mod = reinterpret_cast<LDR_DATA_TABLE_ENTRY*>((uintptr_t)curr.Flink - 0x10);
				auto base_dll_name = *reinterpret_cast<UNICODE_STRING*>(&mod->Reserved4);
				if (base_dll_name.Buffer)
				{
					if (base_dll_name.Buffer == *name)
						return { reinterpret_cast<HMODULE>(mod->DllBase), reinterpret_cast<size_t>(mod->Reserved3[1]) };

				}

				curr = *curr.Flink;
			}

			return {};
		}

	private:
		uintptr_t	m_start;
		std::size_t	m_end;
	};
}