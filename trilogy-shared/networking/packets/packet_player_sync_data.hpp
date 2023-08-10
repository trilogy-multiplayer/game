#pragma once
#include <cstring> 
#include <cstdint>
#include <vendor/librg/zpl/zpl_math.h>

class CGtaControls
{
public:
	int16_t LeftStickX; // move/steer left (-128?)/right (+128)
	int16_t LeftStickY; // move back(+128)/forwards(-128?)
	int16_t RightStickX; // numpad 6(+128)/numpad 4(-128?)
	int16_t RightStickY;

	int16_t LeftShoulder1;
	int16_t LeftShoulder2;
	int16_t RightShoulder1; // target / hand brake
	int16_t RightShoulder2;

	int16_t DPadUp; // radio change up           Next radio station / Call gang forward/Recruit gang member
	int16_t DPadDown; // radio change down       Previous radio station / Gang stay back/Release gang (hold)
	int16_t DPadLeft; //                         Skip trip/Action / Negative talk reply
	int16_t DPadRight; //                        Next user MP3 track / Positive talk reply

	int16_t Start;                             //Pause
	int16_t Select;                            //Camera modes

	int16_t ButtonSquare; // jump / reverse      Break/Reverse / Jump/Climb
	int16_t ButtonTriangle; // get in/out        Exit vehicle / Enter veihcle
	int16_t ButtonCross; // sprint / accelerate  Accelerate / Sprint/Swim
	int16_t ButtonCircle; // fire                Fire weapon

	int16_t ShockButtonL;
	int16_t ShockButtonR; // look behind

	int16_t m_bChatIndicated;
	int16_t m_bPedWalk;
	int16_t m_bVehicleMouseLook;
	int16_t m_bRadioTrackSkip;
};

struct CCompressedControls
{
	struct
	{
		uint8_t bAnalogLeft : 1;
		uint8_t bAnalogRight : 1;
		uint8_t bAnalogUp : 1;
		uint8_t bAnalogDown : 1;
		//uint8_t bDuck : 1;
		uint8_t bMelee : 1;
		uint8_t bSprint : 1;
		uint8_t bJump : 1;
		uint8_t bWalk : 1;
		uint8_t bAim : 1;
		uint8_t bEnterVeh : 1;
	} onFootKeys;

	void ToOnfootGtaControls(CGtaControls* pGtaControls)
	{
		memset(pGtaControls, 0, sizeof(CGtaControls)); // we are already zeroing all members so we don't have to check for zero conditions

		if (onFootKeys.bAnalogLeft)         pGtaControls->LeftStickX = -128;
		else if (onFootKeys.bAnalogRight)   pGtaControls->LeftStickX = 128;

		if (onFootKeys.bAnalogUp)           pGtaControls->LeftStickY = -128;
		else if (onFootKeys.bAnalogDown)    pGtaControls->LeftStickY = 128;

		if (onFootKeys.bMelee)  pGtaControls->ButtonCircle = 128;
		if (onFootKeys.bAim)    pGtaControls->RightShoulder1 = 128;
		if (onFootKeys.bSprint) pGtaControls->ButtonCross = 128;
		if (onFootKeys.bJump)   pGtaControls->ButtonSquare = 128;
		if (onFootKeys.bWalk)   pGtaControls->m_bPedWalk = 128;
		if (onFootKeys.bEnterVeh)   pGtaControls->ButtonTriangle = 128;
	}

	void FromOnfootGtaControls(CGtaControls* pGtaControls)
	{
		memset(this, 0, sizeof(CCompressedControls));
		if (pGtaControls->LeftStickX < 0)   onFootKeys.bAnalogLeft = true;
		if (pGtaControls->LeftStickX > 0)   onFootKeys.bAnalogRight = true;
		if (pGtaControls->LeftStickY < 0)   onFootKeys.bAnalogUp = true;
		if (pGtaControls->LeftStickY > 0)   onFootKeys.bAnalogDown = true;

		//if (pGtaControls->ShockButtonL > 0) onFootKeys.bDuck = true;
		if (pGtaControls->ButtonCircle > 0) onFootKeys.bMelee = true;
		if (pGtaControls->ButtonCross > 0) {
			onFootKeys.bSprint = true;
			//c_log::Info("bSprint");
		}
		if (pGtaControls->ButtonSquare > 0) {
			onFootKeys.bJump = true;
			//c_log::Info("bJump");
		}
		if (pGtaControls->m_bPedWalk > 0)   onFootKeys.bWalk = true;
		if (pGtaControls->RightShoulder1 > 0) onFootKeys.bAim = true;
		if (pGtaControls->ButtonTriangle > 0) onFootKeys.bEnterVeh = true;
	}
};

class packet_player_sync_data {
public:
	float current_rotation;
	float current_heading;

	zplm_vec3_t move_speed;

	CCompressedControls controls;

	uint8_t weapon_id;

	uint8_t health;
	uint8_t armour;
};