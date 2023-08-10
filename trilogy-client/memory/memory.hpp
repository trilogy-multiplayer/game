#pragma once

#ifndef TRILOGY_MEMORY_H
#define TRILOGY_MEMORY_H

#include "common.hpp"

#include "module.hpp"
#include "patterns.hpp"

#include "scripting/scripting.hpp"

#include <sdk/sdk.hpp>
#include <networking/packets/packet_player_sync_data.hpp>

class c_memory : public c_singleton<c_memory> {
private:
	using sdk_find_player_ped_t = int64_t(*)(int32_t player_id);
public:

    class CPad {
    public:
        CGtaControls NewState;
        CGtaControls OldState;
        int16_t            SteeringLeftRightBuffer[10];
        int32_t            DrunkDrivingBufferUsed;
        CGtaControls PCTempKeyState;
        CGtaControls PCTempJoyState;
        CGtaControls PCTempMouseState;
        char             Phase;
        int16_t            Mode;
        int16_t            ShakeDur;

        union {
            struct {
                uint16_t bCamera : 1;
                uint16_t unk2 : 1;
                uint16_t bPlayerAwaitsInGarage : 1;
                uint16_t bPlayerOnInteriorTransition : 1;
                uint16_t unk3 : 1;                        // 0x10 unused
                uint16_t bPlayerSafe : 1;
                uint16_t bPlayerTalksOnPhone : 1;         // bPlayerSafeForPhoneCall?
                uint16_t bPlayerSafeForCutscene : 1;
                uint16_t bPlayerSkipsToDestination : 1;   // bPlayerSafeForDestination?
            };
            uint16_t DisablePlayerControls;
        };

        char     ShakeFreq;
        char     bHornHistory[5];
        char     iCurrHornHistory;
        char     JustOutOfFrontEnd;
        bool     bApplyBrakes;
        bool     bDisablePlayerEnterCar;
        bool     bDisablePlayerDuck;
        bool     bDisablePlayerFireWeapon;
        bool     bDisablePlayerFireWeaponWithL1;
        bool     bDisablePlayerCycleWeapon;
        bool     bDisablePlayerJump;
        bool     bDisablePlayerDisplayVitalStats;
        uint32_t   LastTimeTouched;
        int32_t    AverageWeapon;
        int32_t    AverageEntries;
        float    NoShakeBeforeThis;
        char     NoShakeFreq;
        char    _pad131[3];
    };


	sdk_find_player_ped_t o_sdk_find_player_ped;
	sdk_find_player_ped_t sdk_find_player_ped;
	int8_t* sdk_player_in_focus;

    float* time_step;

    int32_t* sdk_ped_pool_padding;
	int64_t* sdk_ped_pool;
    CPad* sdk_player_pad;

	void initialize();
};

#endif
