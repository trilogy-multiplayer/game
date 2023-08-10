#pragma once

#include <sdk/sdk_vector.hpp>

class CPlaceable {
protected:
    CPlaceable() {}
public:
    __int64 m_placement;
    __int64* m_matrix;

    virtual ~CPlaceable() {};

    __int64* GetMatrix();

    static void ShutdownMatrixArray();
    static void InitMatrixArray();

    sdk_vec3_t GetRightDirection();
    sdk_vec3_t GetTopDirection();
    sdk_vec3_t GetAtDirection();

    void FreeStaticMatrix();
    void SetPosn(float x, float y, float z);
    void SetPosn(sdk_vec3_t const& posn);
    void SetOrientation(float x, float y, float z);
    void SetHeading(float heading);
    float GetHeading();
    bool IsWithinArea(float x1, float y1, float x2, float y2);
    bool IsWithinArea(float x1, float y1, float z1, float x2, float y2, float z2);
    void RemoveMatrix();
    void AllocateStaticMatrix();
    void AllocateMatrix();
    void SetMatrix(__int64  const& matrix);
};