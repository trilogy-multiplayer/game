#pragma once

#include <cstdint>

struct vec3_t {
    float x, y, z;

    vec3_t(float x, float y, float z) :
        x(x), y(y), z(z)
    {}
};

struct sdk_running_script
{
public:
    sdk_running_script* m_pNext;
    sdk_running_script* m_pPrev;
    char            m_szName[8];
    unsigned char* m_pBaseIP;
    unsigned char* m_pCurrentIP;
    unsigned char* m_apStack[8];
    unsigned short  m_nSP;

private:
    char _pad3A[2];

public:
    int  m_aLocalVars[32];
    int  m_anTimers[2];
    bool m_bIsActive;
    bool m_bCondResult;
    bool m_bUseMissionCleanup;
    bool m_bIsExternal;
    bool m_bTextBlockOverride;

private:
    char _padC9[3];

public:
    int            m_nWakeTime;
    unsigned short m_nLogicalOp;
    bool           m_bNotFlag;
    bool           m_bWastedBustedCheck;
    bool           m_bWastedOrBusted;

private:
    char _padD5[3];

public:
    unsigned char* m_pSceneSkipIP;
    bool           m_bIsMission;

private:
    char _padDD[3];

public:
    char* GetPointerToScriptVariable(int a2);
    void  ReadTextLabelFromScript(char* text, char length);
    void  CollectParameters(short num);
    bool  CheckName(const char* name);
    void  ProcessCommands1526to1537(int opcode);
    char  ProcessCommands0to99(int opcode);
    void  UpdateCompareFlag(char flag);
    int   EndThread();
    char  process_one_command(uint32_t table_offset);

    void initialize();

    static void SetCharCoordinates(__int64* ped, vec3_t pos, bool bWarpGang,
        bool bOffset);
};