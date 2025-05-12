#pragma once

#include "opengl.hpp"
#include "math.hpp"

namespace Ichigo {
struct KeyState {
    bool down_this_frame;
    bool up_this_frame;
    bool down;
    bool up;
};

enum Keycode {
    IK_UNKNOWN,
    IK_BACKSPACE,
    IK_TAB,
    IK_ENTER,
    IK_LEFT_SHIFT,
    IK_LEFT_CONTROL,
    IK_LEFT_ALT,
    IK_RIGHT_SHIFT,
    IK_RIGHT_CONTROL,
    IK_RIGHT_ALT,
    IK_ESCAPE,
    IK_SPACE,
    IK_PAGE_UP,
    IK_PAGE_DOWN,
    IK_END,
    IK_HOME,
    IK_LEFT,
    IK_UP,
    IK_RIGHT,
    IK_DOWN,
    IK_PRINT_SCREEN,
    IK_INSERT,
    IK_DELETE,
    IK_F1,
    IK_F2,
    IK_F3,
    IK_F4,
    IK_F5,
    IK_F6,
    IK_F7,
    IK_F8,
    IK_F9,
    IK_F10,
    IK_F11,
    IK_F12,

    IK_0 = '0',
    IK_1,
    IK_2,
    IK_3,
    IK_4,
    IK_5,
    IK_6,
    IK_7,
    IK_8,
    IK_9,

    IK_A = 'A',
    IK_B,
    IK_C,
    IK_D,
    IK_E,
    IK_F,
    IK_G,
    IK_H,
    IK_I,
    IK_J,
    IK_K,
    IK_L,
    IK_M,
    IK_N,
    IK_O,
    IK_P,
    IK_Q,
    IK_R,
    IK_S,
    IK_T,
    IK_U,
    IK_V,
    IK_W,
    IK_X,
    IK_Y,
    IK_Z,
    IK_ENUM_COUNT
};

struct Gamepad {
    // Face buttons
    KeyState a;
    KeyState b;
    KeyState x;
    KeyState y;

    // D-pad
    KeyState up;
    KeyState down;
    KeyState left;
    KeyState right;

    // Shoulder buttons
    KeyState lb;
    KeyState rb;

    // Menu buttons
    KeyState start;
    KeyState select;

    // Triggers
    f32 lt;
    f32 rt;

    // Analog sticks
    KeyState stick_left_click;
    KeyState stick_right_click;
    Vec2<f32> stick_left;
    Vec2<f32> stick_right;

    bool connected;
};

struct Mouse {
    Vec2<i32> pos;
    f32 scroll_wheel_delta_this_frame;

    KeyState left_button;
    KeyState middle_button;
    KeyState right_button;
    KeyState button4;
    KeyState button5;
};


// TODO: Perhaps this is not "game state". Rather, "engine state"?
struct GameState {
    Bana::Arena transient_storage_arena;
    Bana::Arena permanent_storage_arena;
};

extern GameState game_state;

// Internal stuff. Generally okay to read from your game, maybe don't write to it unless you know what you're doing.
namespace Internal {
extern OpenGL gl;
extern u32 window_width;
extern u32 window_height;
extern u32 viewport_width;
extern u32 viewport_height;
extern Vec2<u32> viewport_origin;
extern f32 target_frame_time;
extern f32 dpi_scale;
extern Bana::Allocator temp_allocator;
extern Bana::Allocator perm_allocator;

// TODO: Move these out of internal?
extern f32 dt;
extern Ichigo::KeyState keyboard_state[Ichigo::Keycode::IK_ENUM_COUNT];
extern Gamepad gamepad;
extern Mouse mouse;

// The platform layer calls us through these.
void init();
void do_frame();
void deinit();
void fill_sample_buffer(u8 *buffer, usize buffer_size, usize write_cursor_position_delta);

// == Platform specific functions ==
struct PlatformFile;

PlatformFile *platform_open_file_write(const Bana::String path);
void platform_write_entire_file_sync(const char *path, const u8 *data, usize data_size);

// Append to an open file. Generally works like fwrite() from the CRT.
void platform_append_file_sync(PlatformFile *file, const u8 *data, usize data_size);
void platform_close_file(PlatformFile *file);
Bana::Optional<Bana::FixedArray<u8>> platform_read_entire_file_sync(const Bana::String path, Bana::Allocator allocator = Bana::heap_allocator);

bool platform_file_exists(const char *path);
void platform_sleep(f64 t);
f64 platform_get_current_time();
void platform_pause_audio();
void platform_resume_audio();
void set_mouse_pos(i32 x, i32 y); // NOTE: In client window coordinates.
Vec2<i32> get_window_pos();
}
}
