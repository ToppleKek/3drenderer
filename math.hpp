/*
    Ichigo! A simple, from scratch, minimal dependency game engine for 2D side scrolling games.

    Math structures and functions.

    Author:      Braeden Hong
    Last edited: 2024/12/2
*/

#pragma once
#include "common.hpp"
#include <cmath>

#define MATH_PI 3.14159265

template<typename T>
inline T clamp(T value, T min, T max);

inline f32 deg2rad(f32 deg) {
    return deg * (MATH_PI / 180.0f);
}

template<typename T>
struct Vec2 {
    union {
        struct {
            T x;
            T y;
        };

        struct {
            T r;
            T g;
        };
    };

    Vec2<T> &operator*=(T s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2<T> &operator+=(const Vec2<T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2<T> operator+(const Vec2<T> &rhs) const {
        return { x + rhs.x, y + rhs.y };
    }

    Vec2<T> operator*(const T rhs) const {
        return { x * rhs, y * rhs };
    }

    Vec2<T> operator/(const T rhs) const {
        return { x / rhs, y / rhs };
    }

    Vec2<T> operator*(const Vec2<T> rhs) const {
        return { x * rhs.x, y * rhs.y };
    }

    Vec2<T> operator-(const Vec2<T> &rhs) const {
        return { x - rhs.x, y - rhs.y };
    }

    T length() {
        return sqrt(x * x + y * y);
    }

    T lengthsq() {
        return x * x + y * y;
    }

    void clamp(T min, T max) {
        x = ::clamp(x, min, max);
        y = ::clamp(y, min, max);
    }
};

template<typename T>
Vec2<T> operator*(const T lhs, const Vec2<T> &rhs) {
    return { rhs.x * lhs, rhs.y * lhs };
}

template<typename T>
struct Vec3 {
    union {
        struct {
            T x;
            T y;
            T z;
        };

        struct {
            T r;
            T g;
            T b;
        };
    };

    Vec3<T> &operator*=(T s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vec3<T> &operator+=(const Vec3<T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3<T> &operator-=(const Vec3<T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3<T> operator+(const Vec3<T> &rhs) {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    Vec3<T> operator*(const T rhs) {
        return { x * rhs, y * rhs, z * rhs };
    }

    Vec3<T> operator-(const Vec3<T> &rhs) {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    Vec3<T> operator/(const T rhs) const {
        return { x / rhs, y / rhs, z / rhs };
    }


    T length() {
        return sqrt(x * x + y * y + z * z);
    }

    void clamp(T min, T max) {
        x = ::clamp(x, min, max);
        y = ::clamp(y, min, max);
        z = ::clamp(z, min, max);
    }
};

template<typename T>
Vec3<T> operator*(const T lhs, const Vec3<T> &rhs) {
    return { rhs.x * lhs, rhs.y * lhs, rhs.z * lhs };
}

template<typename T>
struct Vec4 {
    union {
        struct {
            T x;
            T y;
            T z;
            T w;
        };

        struct {
            T r;
            T g;
            T b;
            T a;
        };
    };

    Vec4<T> &operator*=(T s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }
};

template<typename T>
bool operator==(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename T>
bool operator==(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template<typename T>
bool operator==(const Vec4<T> &lhs, const Vec4<T> &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template<typename T>
bool operator!=(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator!=(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator!=(const Vec4<T> &lhs, const Vec4<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
inline Vec2<T> vector_cast(const Vec2<U> &v) {
    return {
        (T) v.x,
        (T) v.y,
    };
}

template<typename T, typename U>
inline Vec3<T> vector_cast(const Vec3<U> &v) {
    return {
        (T) v.x,
        (T) v.y,
        (T) v.z,
    };
}

template<typename T, typename U>
inline Vec4<T> vector_cast(const Vec4<U> &v) {
    return {
        (T) v.x,
        (T) v.y,
        (T) v.z,
        (T) v.w,
    };
}

template<typename T>
struct Mat4 {
    Vec4<T> a;
    Vec4<T> b;
    Vec4<T> c;
    Vec4<T> d;


    Mat4<T> operator*(const Mat4<T> rhs) {
        return {
            {dot(a, {rhs.a.x, rhs.b.x, rhs.c.x, rhs.d.x}), dot(a, {rhs.a.y, rhs.b.y, rhs.c.y, rhs.d.y}), dot(a, {rhs.a.z, rhs.b.z, rhs.c.z, rhs.d.z}), dot(a, {rhs.a.w, rhs.b.w, rhs.c.w, rhs.d.w})},
            {dot(b, {rhs.a.x, rhs.b.x, rhs.c.x, rhs.d.x}), dot(b, {rhs.a.y, rhs.b.y, rhs.c.y, rhs.d.y}), dot(b, {rhs.a.z, rhs.b.z, rhs.c.z, rhs.d.z}), dot(b, {rhs.a.w, rhs.b.w, rhs.c.w, rhs.d.w})},
            {dot(c, {rhs.a.x, rhs.b.x, rhs.c.x, rhs.d.x}), dot(c, {rhs.a.y, rhs.b.y, rhs.c.y, rhs.d.y}), dot(c, {rhs.a.z, rhs.b.z, rhs.c.z, rhs.d.z}), dot(c, {rhs.a.w, rhs.b.w, rhs.c.w, rhs.d.w})},
            {dot(d, {rhs.a.x, rhs.b.x, rhs.c.x, rhs.d.x}), dot(d, {rhs.a.y, rhs.b.y, rhs.c.y, rhs.d.y}), dot(d, {rhs.a.z, rhs.b.z, rhs.c.z, rhs.d.z}), dot(d, {rhs.a.w, rhs.b.w, rhs.c.w, rhs.d.w})}
        };
    }

    Vec4<T> operator*(const Vec4<T> rhs) {
        return {
            dot(a, rhs), dot(b, rhs), dot(c, rhs), dot(d, rhs),
        };
    }
};

using vec2 = Vec2<f32>;
using vec3 = Vec3<f32>;
using vec4 = Vec4<f32>;
using mat4 = Mat4<f32>;

struct Quaternion {
    f32 w;
    f32 x;
    f32 y;
    f32 z;

    Quaternion conjugated() {
        return {w, -x, -y, -z};
    }

    mat4 as_rotation_mat4() {
        return {
            {w*w + x*x - y*y - z*z, 2*x*y - 2*w*z, 2*x*z + 2*w*y, 0.0f},
            {2*x*y + 2*w*z, w*w - x*x + y*y - z*z, 2*y*z - 2*w*x, 0.0f},
            {2*x*z - 2*w*y, 2*y*z + 2*w*x, w*w - x*x - y*y + z*z, 0.0f},
            {0.0f,          0.0f,          0.0f,                  1.0f},
        };
    }
};

inline Quaternion qrot(f32 theta, vec3 axis) {
    f32 half  = deg2rad(theta / 2);
    f32 shalf = sin(half);
    return {cos(half), axis.x * shalf, axis.y * shalf, axis.z * shalf};
}

template <typename T>
struct Rect {
    Vec2<T> pos;
    T w;
    T h;
};

// A textured vertex contains both a position and UV coordinates.
// TODO: Maybe this is stupid and we should just have one type of vertex. It would probably simplify the shader code.
struct TexturedVertex {
    vec3 pos;
    vec2 tex;
    vec3 normal;
};

struct __attribute__((packed)) ColouredVertex {
    vec3 pos;
    vec3 colour;
    vec3 normal;
};

using Vertex = Vec3<f32>;

// Used by some functions to define both a rectangle to be drawn (draw_rect) and a rectangle region in the texture to generate uv coordinates from (texture_rect)
struct TexturedRect {
    Rect<f32> draw_rect;
    Rect<u32> texture_rect;
};

constexpr const Mat4<f32> M4_IDENTITY_F32 = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
};

constexpr const Vec4<f32> COLOUR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

template<typename T>
inline Mat4<T> transpose(Mat4<T> m) {
    return {
        {m.a.x, m.b.x, m.c.x, m.d.x},
        {m.a.y, m.b.y, m.c.y, m.d.y},
        {m.a.z, m.b.z, m.c.z, m.d.z},
        {m.a.w, m.b.w, m.c.w, m.d.w},
    };
}

// template<typename T>
// inline Mat4<T> inverse(Mat4<T> m) {

// }

inline Mat4<f32> m4identity() {
    return M4_IDENTITY_F32;
}

inline Mat4<f32> translate2d(Vec2<f32> t) {
    return {
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };
}

inline Mat4<f32> translate3d(Vec3<f32> t) {
    return {
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, t.z},
        {0, 0, 0, 1},
    };
}

inline Mat4<f32> scale2d(Vec2<f32> s) {
    return {
        {s.x, 0,   0, 0},
        {0,   s.y, 0, 0},
        {0,   0,   1, 0},
        {0,   0,   0, 1},
    };
}

inline Mat4<f32> scale3d(Vec3<f32> s) {
    return {
        {s.x, 0,   0,   0},
        {0,   s.y, 0,   0},
        {0,   0,   s.z, 0},
        {0,   0,   0,   1},
    };
}

inline Mat4<f32> rotation2d(f32 deg) {
    f32 rad = deg * (MATH_PI / 180.0f);
    return {
        {std::cos(rad), -std::sin(rad), 0, 0},
        {std::sin(rad), std::cos(rad),  0, 0},
        {0,             0,              1, 0},
        {0,             0,              0, 1},
    };
}

inline Mat4<f32> xrot3d(f32 deg) {
    f32 rad = deg * (MATH_PI / 180.0f);
    return {
        {1, 0,        0,         0},
        {0, cos(rad), -sin(rad), 0},
        {0, sin(rad), cos(rad),  0}, // FIXME: Is this -cos wrong?
        {0, 0,        0,         1},
    };
}

inline Mat4<f32> yrot3d(f32 deg) {
    f32 rad = deg * (MATH_PI / 180.0f);
    return {
        {cos(rad),  0, sin(rad), 0},
        {0,         1, 0,        0},
        {-sin(rad), 0, cos(rad), 0},
        {0,         0, 0,        1},
    };
}

inline Mat4<f32> zrot3d(f32 deg) {
    f32 rad = deg * (MATH_PI / 180.0f);
    return {
        {cos(rad), -sin(rad), 0, 0},
        {sin(rad), cos(rad),  0, 0},
        {0,        0,         1, 0},
        {0,        0,         0, 1},
    };
}

inline Vec2<f32> get_translation2d(Mat4<f32> m) {
    return {m.a.w, m.b.w};
}

struct Xform {
    vec3 translation;
    Quaternion rotation;
    vec3 scale;
};

inline mat4 xform_to_mat4(Xform xform) {
    return translate3d(xform.translation) * xform.rotation.as_rotation_mat4() * scale3d(xform.scale);
    // return xform.rotation.as_rotation_mat4();
}

template<typename T>
inline T dot(Vec2<T> lhs, Vec2<T> rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T>
inline T dot(Vec3<T> lhs, Vec3<T> rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
inline T dot(Vec4<T> lhs, Vec4<T> rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template<typename T>
inline Vec3<T> cross(Vec3<T> lhs, Vec3<T> rhs) {
    return Vec3<T>{lhs.y * rhs.z - rhs.y * lhs.z, lhs.z * rhs.x - rhs.z * lhs.x, lhs.x * rhs.y - rhs.x * lhs.y};
}

template<typename T>
inline Vec3<T> normalize(Vec3<T> v) {
    return v / v.length();
}

inline mat4 perspective(f32 fov, f32 aspect, f32 zn, f32 zf) {
    f32 tanhalf = tan((fov * (MATH_PI / 180.0f)) / 2.0f);

    return {
        {1.0f / (aspect * tanhalf), 0, 0, 0},
        {0, 1.0f / tanhalf, 0, 0},
        {0, 0, -(zf + zn) / (zf - zn), -(2.0f * zf * zn) / (zf - zn)},
        {0, 0, -1.0f, 0},
    };
}

inline mat4 look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = normalize(cross(s, f));

    return {
        {s.x, s.y, s.z, -dot(s, eye)},
        {u.x, u.y, u.z, -dot(u, eye)},
        {-f.x, -f.y, -f.z, dot(f, eye)},
        {0.0f, 0.0f, 0.0f, 1.0f},
        // {right.x, cam_up.x, forward.x, -dot(right, position)},
        // {right.y, cam_up.y, forward.y, -dot(cam_up, position)},
        // {right.z, cam_up.z, forward.z, dot(forward, position)},
        // {0.0f, 0.0f, 0.0f, 1.0f},
    };
}

inline bool rectangles_intersect(Rect<f32> rect1, Rect<f32> rect2) {
    return ((rect1.pos.x >= rect2.pos.x && rect1.pos.x <= rect2.pos.x + rect2.w) || (rect2.pos.x >= rect1.pos.x && rect2.pos.x <= rect1.pos.x + rect1.w)) &&
           ((rect1.pos.y >= rect2.pos.y && rect1.pos.y <= rect2.pos.y + rect2.h) || (rect2.pos.y >= rect1.pos.y && rect2.pos.y <= rect1.pos.y + rect1.h));
}

inline bool rectangle_contains_point(Rect<f32> rect, Vec2<f32> point) {
    return ((rect.pos.x <= point.x && rect.pos.x + rect.w >= point.x) && (rect.pos.y <= point.y && rect.pos.y + rect.h >= point.y));
}

inline f32 pixels_to_metres(f32 pixels) {
    return pixels / PIXELS_PER_METRE;
}

inline f32 safe_ratio_1(f32 dividend, f32 divisor) {
    if (divisor == 0)
        return 1.0f;

    return dividend / divisor;
}

inline f32 safe_ratio_0(f32 dividend, f32 divisor) {
    if (divisor == 0)
        return 0.0f;

    return dividend / divisor;
}

template<typename T>
inline T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

inline f32 ichigo_lerp(f32 a, f32 t, f32 b) {
    return a + t * (b - a);
}

inline f32 bezier(f32 p0, f32 p1, f32 t, f32 p2, f32 p3) {
    return ((1 - t) * (1 - t) * (1 - t) * p0) + (t * t * t * p3) + (3 * t * t * (1 - t) * p2) + (3 * (1 - t) * (1 - t) * t * p1);
}

inline f32 move_towards(f32 current, f32 end, f32 step) {
    if (current < end) {
        return clamp(current + step, current, end);
    } else {
        return clamp(current - step, end, current);
    }
}

inline f32 signof(f32 value) {
    return value < 0.0f ? -1.0f : 1.0f;
}

inline f32 rand_01_f32() {
    return (f32) std::rand() / (f32) RAND_MAX;
}

inline f32 rand_range_f32(f32 low, f32 high) {
    return ichigo_lerp(low, rand_01_f32(), high);
}
