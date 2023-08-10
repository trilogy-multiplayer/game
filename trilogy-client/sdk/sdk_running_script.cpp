#include "sdk_running_script.hpp"
#include <Windows.h>
#include "memory/module.hpp"
#include "memory/patterns.hpp"

static int commands_executed = 0;

static int16_t read_two_bytes_from_script(uint8_t*& ip) {
	int16_t retval = *reinterpret_cast<int16_t*>(ip);
	ip += 2;
	return retval;
}

static int8_t read_one_byte_from_script(uint8_t*& ip) {
	int8_t retval = *reinterpret_cast<int8_t*>(ip);
	ip += 1;
	return retval;
}

using process_command_t = char(*)(sdk_running_script*, int);

using command_handler_t = char(__fastcall*)(sdk_running_script*, int32_t);
using command_handler_table_t = std::array<command_handler_t, 27>;

char sdk_running_script::process_one_command(uint32_t table_offset)
{
	++commands_executed;

	union {
		int16_t op;
		struct {
			uint16_t command : 15;
			uint16_t notFlag : 1;
		};
	} op = { read_two_bytes_from_script(this->m_pCurrentIP) };

	this->m_bNotFlag = op.notFlag;

	static command_handler_table_t& s_OriginalCommandHandlerTable = *(command_handler_table_t*)((uintptr_t)GetModuleHandleA(0) + table_offset);

	return std::invoke(s_OriginalCommandHandlerTable.at((size_t)op.command / 100), this, (int32_t)(op.command));
}

void sdk_running_script::initialize()
{
	strcpy_s(m_szName, "noname");
	m_pBaseIP = nullptr;
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_pCurrentIP = nullptr;
	memset(m_apStack, 0, sizeof(m_apStack));
	m_nSP = 0;
	m_nWakeTime = 0;
	m_bIsActive = false;
	m_bCondResult = false;
	m_bUseMissionCleanup = false;
	m_bIsExternal = false;
	m_bTextBlockOverride = false;
	memset(m_aLocalVars, 0, sizeof(m_aLocalVars));
	m_nLogicalOp = 0;
	m_bNotFlag = false;
	m_bWastedOrBusted = false;
	m_pSceneSkipIP = nullptr;
	m_bIsMission = false;
	m_bWastedBustedCheck = true;
}

void sdk_running_script::SetCharCoordinates(__int64* ped, vec3_t pos, bool bWarpGang, bool bOffset)
{
	c_log::Info("sdk_running_script::SetCharCoordinates");
}

char* sdk_running_script::GetPointerToScriptVariable(int a2)
{
	c_log::Info("sdk_running_script::GetPointerToScriptVariable");

	return nullptr;
}

void sdk_running_script::ReadTextLabelFromScript(char* text, char length)
{
	c_log::Info("sdk_running_script::ReadTextLabelFromScript");
}

void sdk_running_script::CollectParameters(short num)
{
	c_log::Info("sdk_running_script::CollectParameters");
}

bool sdk_running_script::CheckName(const char* name)
{
	if (std::string(name) == this->m_szName)
		return true;
	return false;
}

void sdk_running_script::ProcessCommands1526to1537(int opcode)
{
	c_log::Info("sdk_running_script::ProcessCommands1526to1537");
}

char sdk_running_script::ProcessCommands0to99(int opcode)
{
	c_log::Info("sdk_running_script::ProcessCommands0to99");

	return 0;
}

void sdk_running_script::UpdateCompareFlag(char flag)
{
	c_log::Info("sdk_running_script::UpdateCompareFlag");
}

int sdk_running_script::EndThread()
{
	c_log::Info("sdk_running_script::EndThread");

	return 0;
}
