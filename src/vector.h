#pragma once

class vector3 {
public:
    vector3() : e{ 0,0,0 } {}
    vector3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    vector3 operator-() const { return vector3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    //float& operator[](int i) { return e[i]; }

    vector3& operator+=(const vector3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vector3& operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vector3& operator/=(const float t) {
        return *this *= 1 / t;
    }

    float length() const {
        return e[0] + e[1] + e[2];
    }

    float length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }


public:
    float e[3];
};
class vector3Short {
public:
    vector3Short() : e{ 0,0,0 } {}
    vector3Short(short e0, short e1, short e2) : e{ e0, e1, e2 } {}

    int x() const { return e[0]; }
    int y() const { return e[1]; }
    int z() const { return e[2]; }

    vector3Short operator-() const { return vector3Short(-e[0], -e[1], -e[2]); }
    short operator[](short i) const { return e[i]; }
    //float& operator[](int i) { return e[i]; }

    vector3Short& operator+=(const vector3Short& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vector3Short& operator*=(const short t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vector3Short& operator/=(const short t) {
        return *this *= 1 / t;
    }

    int length() const {
        return e[0] + e[1] + e[2];
    }

    int length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }


public:
    int e[3];
};

class vector4 {
public:
    vector4() : e{ 0,0,0,0} {}
    vector4(float e0, float e1, float e2, float e3) : e{ e0, e1, e2, e3 } {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    float w() const { return e[3]; }

    vector4 operator-() const { return vector4(-e[0], -e[1], -e[2], -e[3]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    vector4& operator+=(const vector4& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        e[3] += v.e[3];
        return *this;
    }

    vector4& operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        e[3] *= t;
        return *this;
    }

    vector4& operator/=(const float t) {
        return *this *= 1 / t;
    }

    float length() const {
        return e[0] + e[1] + e[2];
    }

    float length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

public:
    float e[4];
};

