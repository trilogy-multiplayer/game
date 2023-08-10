/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include <sdk/sdk_entity.hpp>

enum ePhysicalFlags {
    PHYSICAL_b01 = 0x1,
    PHYSICAL_APPLY_GRAVITY = 0x2,
    PHYSICAL_DISABLE_COLLISION_FORCE = 0x4,
    PHYSICAL_COLLIDABLE = 0x8,
    PHYSICAL_DISABLE_TURN_FORCE = 0x10,
    PHYSICAL_DISABLE_MOVE_FORCE = 0x20,
    PHYSICAL_INFINITE_MASS = 0x40,
    PHYSICAL_DISABLE_Z = 0x80,

    PHYSICAL_SUBMERGED_IN_WATER = 0x100,
    PHYSICAL_ON_SOLID_SURFACE = 0x200,
    PHYSICAL_BROKEN = 0x400,
    PHYSICAL_b12 = 0x800,
    PHYSICAL_b13 = 0x1000,
    PHYSICAL_DONT_APPLY_SPEED = 0x2000,
    PHYSICAL_b15 = 0x4000,
    PHYSICAL_b16 = 0x8000,

    PHYSICAL_17 = 0x10000,
    PHYSICAL_18 = 0x20000,
    PHYSICAL_BULLETPROOF = 0x40000,
    PHYSICAL_FIREPROOF = 0x80000,
    PHYSICAL_COLLISIONPROOF = 0x100000,
    PHYSICAL_MEELEPROOF = 0x200000,
    PHYSICAL_INVULNERABLE = 0x400000,
    PHYSICAL_EXPLOSIONPROOF = 0x800000,

    PHYSICAL_25 = 0x1000000,
    PHYSICAL_ATTACHEDTOENTITY = 0x2000000,
    PHYSICAL_27 = 0x4000000,
    PHYSICAL_TOUCHINGWATER = 0x8000000,
    PHYSICAL_CANBECOLLIDEDWITH = 0x10000000,
    PHYSICAL_DESTROYED = 0x20000000,
    PHYSICAL_31 = 0x40000000,
    PHYSICAL_32 = 0x80000000,
};

enum eEntityAltCollision : uint16_t {
    ALT_ENITY_COL_DEFAULT = 0,
    ALT_ENITY_COL_OBJECT,
    ALT_ENITY_COL_VEHICLE,
    ALT_ENITY_COL_BIKE_WRECKED,
    ALT_ENITY_COL_BOAT,
};

class CPhysical : public CEntity {
public:
    CPhysical();
    ~CPhysical() override;
public:
    float  field_38;
    uint32_t m_nLastCollisionTime;
    union {
        struct {
            uint32_t bMakeMassTwiceAsBig : 1;
            uint32_t bApplyGravity : 1;
            uint32_t bDisableCollisionForce : 1;
            uint32_t bCollidable : 1;
            uint32_t bDisableTurnForce : 1;
            uint32_t bDisableMoveForce : 1;
            uint32_t bInfiniteMass : 1;
            uint32_t bDisableZ : 1;

            uint32_t bSubmergedInWater : 1;
            uint32_t bOnSolidSurface : 1;
            uint32_t bBroken : 1;
            uint32_t bProcessCollisionEvenIfStationary : 1; // ref @ 0x6F5CF0
            uint32_t b13 : 1;                               // only used for peds
            uint32_t bDontApplySpeed : 1;
            uint32_t b15 : 1;
            uint32_t bProcessingShift : 1;

            uint32_t b17 : 1;
            uint32_t bDisableSimpleCollision : 1; // ref @ CPhysical::ProcessCollision
            uint32_t bBulletProof : 1;
            uint32_t bFireProof : 1;
            uint32_t bCollisionProof : 1;
            uint32_t bMeeleProof : 1;
            uint32_t bInvulnerable : 1;
            uint32_t bExplosionProof : 1;

            uint32_t bDontCollideWithFlyers : 1;
            uint32_t bAttachedToEntity : 1;
            uint32_t bAddMovingCollisionSpeed : 1;
            uint32_t bTouchingWater : 1;
            uint32_t bCanBeCollidedWith : 1;
            uint32_t bDestroyed : 1;
            uint32_t b31 : 1;
            uint32_t b32 : 1;
        } physicalFlags;
        uint32_t m_nPhysicalFlags;
    };
    sdk_vec3_t             m_vecMoveSpeed;
    sdk_vec3_t             m_vecTurnSpeed;
    sdk_vec3_t             m_vecFrictionMoveSpeed;
    sdk_vec3_t             m_vecFrictionTurnSpeed;
    sdk_vec3_t             m_vecForce;
    sdk_vec3_t             m_vecTorque;
    float               m_fMass;
    float               m_fTurnMass;
    float               m_fVelocityFrequency;
    float               m_fAirResistance;
    float               m_fElasticity;
    float               m_fBuoyancyConstant;
    sdk_vec3_t             m_vecCentreOfMass;
    __int64      m_pCollisionList;
    __int64* m_pMovingList;
    uint8_t               m_nFakePhysics;
    uint8_t               m_nNumEntitiesCollided;
    uint8_t               m_nContactSurface;
    char                field_BB;
    CEntity* m_apCollidedEntities[6];
    float               m_fMovingSpeed; // ref @ CTheScripts::IsVehicleStopped
    float               m_fDamageIntensity;
    CEntity* m_pDamageEntity;
    sdk_vec3_t             m_vecLastCollisionImpactVelocity;
    sdk_vec3_t             m_vecLastCollisionPosn;
    uint16_t              m_nPieceType;
    int16_t               field_FA;
    CPhysical* m_pAttachedTo;
    sdk_vec3_t             m_vecAttachOffset;
    sdk_vec3_t             m_vecAttachedEntityRotation;
    __int64         m_qAttachedEntityRotation;
    CEntity* m_pEntityIgnoredCollision;
    float               m_fContactSurfaceBrightness;
    float               m_fDynamicLighting;
    __int64* m_pShadowData;

    static float& DAMPING_LIMIT_IN_FRAME;
    static float& DAMPING_LIMIT_OF_SPRING_FORCE;
    static float& PHYSICAL_SHIFT_SPEED_DAMP;
    static float& SOFTCOL_SPEED_MULT;
    static float& SOFTCOL_SPEED_MULT2;
    static float& SOFTCOL_DEPTH_MIN;
    static float& SOFTCOL_DEPTH_MULT;
    static float& SOFTCOL_CARLINE_SPEED_MULT;
    static float& TEST_ADD_AMBIENT_LIGHT_FRAC;
    static float& HIGHSPEED_ELASTICITY_MULT_COPCAR;
    static sdk_vec3_t& fxDirection;
};