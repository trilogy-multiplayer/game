#include "scripting.hpp"

bool c_scripting::initialize()
{
    m_script_params = memory::as_relative<int32_t*>(
        memory::find_pattern(memory::module_t(nullptr), "c_scripting::m_script_params", "48 63 05 ? ? ? ? C6 45 28 ?"));

    m_table_offset = *memory::find_pattern<int32_t*>(memory::module_t(nullptr), "c_scripting::m_table_offset", "49 8B 84 D6 ? ? ? 04", 4);

    return false;
}

void c_script::append(const void* bytes, int size)
{
    if (m_offset + size >= 128)
        return;
    memcpy(this->m_data + m_offset, bytes, size);

    this->m_offset += size;
}

c_script::c_script(short opcodeId)
{
    this->m_data = new unsigned char[128];
    this->m_offset = 0;

    append(&opcodeId, sizeof(short));
}

c_script::~c_script() { delete[] m_data; }

void c_script::store_parameters(sdk_running_script* script)
{
    int j = 0;
    for (auto i : m_savedParams)
        *i = script->m_aLocalVars[j++];
}

void c_script::operator << (const char* str)
{
    unsigned char typeId = 0xE;
    unsigned char len = strlen(str);

    append(&typeId, 1);
    append(&len, 1);

    append(str, len);
}

void c_script::operator << (int32_t* ptr)
{
    *this << script_local_var_t(this->m_savedParams.size());
    this->m_savedParams.push_back(ptr);
}

void c_script::operator << (float* ptr)
{
    *this << script_local_var_t(this->m_savedParams.size());
    this->m_savedParams.push_back((int*)ptr);
}

#define operator(ID, TYPE)                                                     \
    void c_script::operator << (TYPE n)                                        \
    {                                                                          \
        unsigned char typeId = ID;                                             \
        append (&typeId, 1);                                                   \
        append (&n, sizeof (TYPE));                                            \
    }

operator(4, char);
operator(1, int);
operator(2, short);
operator(6, float);
operator(2, script_global_var_t);
operator(3, script_local_var_t);