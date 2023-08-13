#include <cstring>
#include <cstdio>
#include <vector>
#include <memory>

#include "sdk/sdk_script_commands.hpp"
#include "sdk/sdk_running_script.hpp"

#include "memory/module.hpp"
#include "memory/patterns.hpp"

#include <utilities/singleton.hpp>

class c_script
{
public:
	struct script_global_var_t
	{
		short val;
		script_global_var_t(short val) { this->val = val * 4; }
	};

	struct script_local_var_t
	{
		short val;
		script_local_var_t(short val) { this->val = val; }
	};

public:
	unsigned char* m_data;
	int                 m_offset;
	std::vector<int*>   m_savedParams;

	void append(const void* bytes, int size);

public:
	c_script(short opcodeId);
	~c_script();

	unsigned char* get_data() { return m_data; };

	void operator<< (char n);
	void operator<< (int n);
	void operator<< (short n);
	void operator<< (float n);
	void operator<< (script_global_var_t n);
	void operator<< (script_local_var_t n);
	void operator<< (int* n);
	void operator<< (float* n);
	void operator<< (const char* str);

	void store_parameters(sdk_running_script* script);

	/*******************************************************/
	template <typename T>
	void pack(T value)
	{
		operator << (value);
	}

	template <typename first_t, typename... rest_t>
	void pack(first_t first, rest_t... rest)
	{
		pack(first);
		pack(rest...);
	}

	void pack() { }
};

class c_scripting : public c_singleton<c_scripting> {
private:
	int32_t* m_script_params = 0;
	uint32_t m_table_offset = 0;

public:
	bool initialize();

	template <typename... Args>
	void call_opcode(sdk_script_commands command, Args... args)
	{
		sdk_running_script script;
		script.initialize();

		c_script opcode = c_script(command);
		opcode.pack(args...);

		script.m_pBaseIP = opcode.get_data();
		script.m_pCurrentIP = script.m_pBaseIP;

		static std::unique_ptr<int> original_script_params(new int32_t[10]);
		memcpy(original_script_params.get(), m_script_params, 10 * sizeof(int32_t));

		script.process_one_command(m_table_offset);
		opcode.store_parameters(&script);

		memcpy(m_script_params, original_script_params.get(), 10 * sizeof(int32_t));
	}
};