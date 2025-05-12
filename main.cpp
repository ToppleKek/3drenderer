/*
    Ichigo! A simple, from scratch, minimal dependency game engine for 2D side scrolling games.

    Core module. Rendering, input, state, etc.

    Author:      Braeden Hong
    Last edited: 2024/12/1
*/

#include <cassert>
#include <cstddef>
#include <ctime>

#include "common.hpp"
#include "math.hpp"
#include "asset.hpp"
#include "ichigo.hpp"
#include "mesh.hpp"
// #include "mixer.hpp"

#include <stb_image.h>

// #define STB_RECT_PACK_IMPLEMENTATION
// #include <stb_rect_pack.h>

// #define STB_TRUETYPE_IMPLEMENTATION
// #include <stb_truetype.h>

EMBED("noto.ttf", noto_font)
EMBED("shaders/basic_vert.glsl", basic_vert_src)
EMBED("shaders/basic_texture.glsl", basic_texture_src)
EMBED("shaders/basic_colour.glsl", basic_colour_src)
EMBED("shaders/basic_colour_vert.glsl", basic_colour_vert_src)

EMBED("assets/bookie.png", bookie_png)

#ifdef ICHIGO_DEBUG
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"
static bool show_debug_menu = false;
static ImGuiStyle initial_style;
static ImFontConfig font_config;
#endif

static Ichigo::TextureID bookie_tex_id = 0;

static f32 last_dpi_scale = 1.0f;

static u32 last_window_height = 0;
static u32 last_window_width  = 0;

static GLuint basic_shader_program = 0;
static GLuint basic_colour_shader_program = 0;
static GLuint basic_pos_tex_vao = 0;
static GLuint basic_pos_col_vao = 0;
static GLuint test_geometry_vbo = 0;
static GLuint test_index_vbo    = 0;

static GLuint mococo_geometry_vbo = 0;
static Bana::FixedArray<GLuint> mococo_index_vbos  = {};
static usize skeleton_index_size = 0;
static Bana::FixedArray<i32> mococo_index_sizes = {};
static Bana::FixedArray<Material> mococo_materials = {};
static Bana::FixedArray<Bone> mococo_skeleton = {};
// Global state that is visible in ichigo.hpp
Ichigo::GameState Ichigo::game_state        = {};
f32 Ichigo::Internal::target_frame_time     = 0.016f;
u32 Ichigo::Internal::viewport_width        = 0;
u32 Ichigo::Internal::viewport_height       = 0;
Vec2<u32> Ichigo::Internal::viewport_origin = {};

struct VBOPair {
    GLuint geometry_vbo;
    GLuint index_vbo;
};

static VBOPair mococo_skeleton_debug_vbos;

static void *temp_arena_alloc(usize size) {
    return (void *) PUSH_ARRAY(Ichigo::game_state.transient_storage_arena, u8, size);
}

static void arena_free_nop(void *) {}

static bool arena_realloc_nop(void **, usize) { return false; }

static void *perm_arena_alloc(usize size) {
    return (void *) PUSH_ARRAY(Ichigo::game_state.permanent_storage_arena, u8, size);
}

Bana::Allocator Ichigo::Internal::temp_allocator = {
    temp_arena_alloc,
    arena_free_nop,
    arena_realloc_nop
};

Bana::Allocator Ichigo::Internal::perm_allocator = {
    perm_arena_alloc,
    arena_free_nop,
    arena_realloc_nop
};

// General purpose static string buffer.
static char string_buffer[1024];

static vec3 camera_front = {0.0f, 0.0f, -1.0f};
static vec3 camera_pos   = {0.0f, 0.0f, -3.0f};
static vec3 camera_up    = {0.0f, 1.0f, 0.0f};
static f32 camera_yaw = 45.0f;
static f32 camera_pitch = 10.0f;

static vec3 light_pos            = {1.0f, 1.0f, 1.0f};
static vec3 light_colour         = {1.0f, 1.0f, 1.0f};
static vec3 ambient_light_colour = {0.2f, 0.2f, 0.2f};

static Bana::FixedStringMap<Ichigo::TextureID> texture_maps;

// Render one frame.
static void frame_render() {
#ifdef ICHIGO_DEBUG
    ImGui::Render();
#endif

    if (Ichigo::Internal::window_width == 0 || Ichigo::Internal::window_height == 0) {
        return;
    }

    Ichigo::Internal::gl.glViewport(0, 0, Ichigo::Internal::window_width, Ichigo::Internal::window_height);
    Ichigo::Internal::gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Ichigo::Internal::gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static f32 shit = 0.0f;
    mat4 model  = m4identity() * xrot3d(shit);
    // mat4 model1 = translate3d({1.0f, 0.0f, -1.0f}) * yrot3d(shit);
    mat4 model1 = translate3d({1.0f, 0.0f, -1.0f}) * yrot3d(shit);
    // mat4 model2 = translate3d({3.0f, 0.0f, -3.0f});
    // mat4 model2 = translate3d({2.0f, 0.0f, 0.0f}) * xrot3d(-90.0f);
    mat4 model2 = translate3d({2.0f, 0.0f, 0.0f}) * xrot3d(-90.0f);
    // mat4 model2 = translate3d({-1.0f, 0.0f, 1.0f}) * zrot3d(shit);
    shit += 50.0f * Ichigo::Internal::dt;

    if (shit >= 360.0f) {
        shit -= 360.0f;
    }
    // mat4 view  = translate3d({0.0f, 0.0f, -3.0f});
    // vec3 mimizu = (camera_pos + camera_front);
    // vec4 target = yrot3d(shit2) * xrot3d(shit2) * vec4{mimizu.x, mimizu.y, mimizu.z, 1.0f};
    // vec3 happy  = {target.x, target.y, target.z};

    f32 yr = deg2rad(camera_yaw);
    f32 pr = deg2rad(camera_pitch);

    camera_front.x = cos(yr) * cos(pr);
    camera_front.y = sin(pr);
    camera_front.z = sin(yr) * cos(pr);

    camera_front = normalize(camera_front);

    mat4 view  = look_at(camera_pos, camera_pos + camera_front, {0.0f, 1.0f, 0.0f});
    // mat4 view  = look_at(camera_pos, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    // mat4 view = m4identity();
    // view.a.y = 5.0f;
    // view.a.z = 3.0f;

    mat4 proj = perspective(45.0f, (f32) Ichigo::Internal::window_width / (f32) Ichigo::Internal::window_height, 0.01f, 100.0f);

    // mat4 proj  = infinite_perspective(0, Ichigo::Internal::window_width, 0, Ichigo::Internal::window_height, 0.1f);
    // auto fucking_proj = glm::perspective(glm::radians(45.0f), (f32) Ichigo::Internal::window_width / (f32) Ichigo::Internal::window_height, 0.1f, 100.0f);
    // auto fucking_proj = glm::ortho(0.0f, 800.0f, 0.0f, (f32) 600.0f, 0.1f, 100.0f);

    Ichigo::Internal::gl.glUseProgram(basic_shader_program);
    Ichigo::Internal::gl.glBindTexture(GL_TEXTURE_2D, Ichigo::Internal::textures[bookie_tex_id].id);
    Ichigo::Internal::gl.glBindVertexArray(basic_pos_tex_vao);

    GLint model_loc          = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "model");
    // GLint normal_model_loc   = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "normal_model");
    GLint view_loc           = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "view");
    GLint proj_loc           = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "proj");
    GLint tex_loc            = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "entity_texture");
    GLint ambient_light_loc  = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "ambient_light");
    GLint test_light_pos_loc = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "test_light_pos");
    GLint test_light_col_loc = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "test_light_colour");
    GLint specular_exp_loc   = Ichigo::Internal::gl.glGetUniformLocation(basic_shader_program, "specular_exponent");
    // mat4 normal_model = transpose(inverse(model));

    Ichigo::Internal::gl.glUniform1i(tex_loc, 0);
    Ichigo::Internal::gl.glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &model);
    // Ichigo::Internal::gl.glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &normal_model);
    Ichigo::Internal::gl.glUniformMatrix4fv(view_loc, 1, GL_TRUE, (GLfloat *) &view);
    Ichigo::Internal::gl.glUniformMatrix4fv(proj_loc, 1, GL_TRUE, (GLfloat *) &proj);
    Ichigo::Internal::gl.glUniform3fv(ambient_light_loc, 1, (f32 *) &ambient_light_colour);
    Ichigo::Internal::gl.glUniform3fv(test_light_pos_loc, 1, (f32 *) &light_pos);
    Ichigo::Internal::gl.glUniform3fv(test_light_col_loc, 1, (f32 *) &light_colour);
    Ichigo::Internal::gl.glUniform1f(specular_exp_loc, 32.0f);

    Ichigo::Internal::gl.glBindVertexBuffer(0, test_geometry_vbo, 0, sizeof(TexturedVertex));
    Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test_index_vbo);

    Ichigo::Internal::gl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    Ichigo::Internal::gl.glBindVertexBuffer(0, mococo_geometry_vbo, 0, sizeof(TexturedVertex));
    Ichigo::Internal::gl.glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &model1);

    for (i32 i = 0; i < mococo_index_vbos.size; ++i) {
        auto t = texture_maps.get(mococo_materials[i].diffuse_map_file);
        if (t.has_value) {
            Ichigo::Internal::gl.glBindTexture(GL_TEXTURE_2D, Ichigo::Internal::textures[*t.value].id);
        } else {
            assert(false);
        }

        Ichigo::Internal::gl.glUniform1f(specular_exp_loc, mococo_materials[i].specular_exponent);
        Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mococo_index_vbos.data[i]);
        // ICHIGO_INFO("Number of indices: %d", mococo_index_sizes.data[i]);
        Ichigo::Internal::gl.glDrawElements(GL_TRIANGLES, mococo_index_sizes.data[i], GL_UNSIGNED_INT, 0);
    }

    GL_CALL(glBindVertexArray(basic_pos_col_vao));
    GL_CALL(glUseProgram(basic_colour_shader_program));

    model_loc          = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "model");
    view_loc           = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "view");
    proj_loc           = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "proj");
    tex_loc            = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "entity_texture");
    ambient_light_loc  = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "ambient_light");
    test_light_pos_loc = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "test_light_pos");
    test_light_col_loc = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "test_light_colour");
    specular_exp_loc   = Ichigo::Internal::gl.glGetUniformLocation(basic_colour_shader_program, "specular_exponent");

    GL_CALL(glUniform1i(tex_loc, 0));
    GL_CALL(glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &model2));
    GL_CALL(glUniformMatrix4fv(view_loc, 1, GL_TRUE, (GLfloat *) &view));
    GL_CALL(glUniformMatrix4fv(proj_loc, 1, GL_TRUE, (GLfloat *) &proj));
    GL_CALL(glUniform3fv(ambient_light_loc, 1, (f32 *) &ambient_light_colour));
    GL_CALL(glUniform3fv(test_light_pos_loc, 1, (f32 *) &light_pos));
    GL_CALL(glUniform3fv(test_light_col_loc, 1, (f32 *) &light_colour));
    GL_CALL(glUniform1f(specular_exp_loc, 32.0f));

    GL_CALL(glBindVertexBuffer(0, mococo_skeleton_debug_vbos.geometry_vbo, 0, sizeof(ColouredVertex)));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mococo_skeleton_debug_vbos.index_vbo));
    GL_CALL(glDrawElements(GL_TRIANGLES, skeleton_index_size, GL_UNSIGNED_INT, 0));

    // Ichigo::Internal::gl.glBindVertexArray(basic_pos_tex_vao);
    // Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mococo_index_vbos.data[0]);
    // Ichigo::Internal::gl.glDrawElements(GL_TRIANGLES, mococo_index_sizes.data[0], GL_UNSIGNED_INT, 0);

    // Ichigo::Internal::gl.glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &model1);
    // Ichigo::Internal::gl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Ichigo::Internal::gl.glUniformMatrix4fv(model_loc, 1, GL_TRUE, (GLfloat *) &model2);
    // Ichigo::Internal::gl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Reset uniforms
#ifdef ICHIGO_DEBUG
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    Ichigo::Internal::gl.glFinish();
}

// Simulation and logic for one frame.
void Ichigo::Internal::do_frame() {
    if (Ichigo::Internal::dt > 0.1f) {
        Ichigo::Internal::dt = 0.1f;
    }

    RESET_ARENA(Ichigo::game_state.transient_storage_arena);

    if (Ichigo::Internal::window_height != last_window_height || Ichigo::Internal::window_width != last_window_width) {
        last_window_height = Ichigo::Internal::window_height;
        last_window_width  = Ichigo::Internal::window_width;
    }

    if (dpi_scale != last_dpi_scale) {
        ICHIGO_INFO("scaling to scale=%f", dpi_scale);
#ifdef ICHIGO_DEBUG
        auto io = ImGui::GetIO();
        ImGui_ImplOpenGL3_DestroyFontsTexture();
        io.Fonts->Clear();
        io.Fonts->AddFontFromMemoryTTF((void *) noto_font, noto_font_len, static_cast<i32>(18 * dpi_scale), &font_config, io.Fonts->GetGlyphRangesJapanese());
        io.Fonts->Build();
        ImGui_ImplOpenGL3_CreateFontsTexture();
        // Scale all Dear ImGui sizes based on the inital style
        std::memcpy(&ImGui::GetStyle(), &initial_style, sizeof(initial_style));
        ImGui::GetStyle().ScaleAllSizes(dpi_scale);
#endif
        last_dpi_scale = dpi_scale;
    }

#ifdef ICHIGO_DEBUG
    // Toggle the debug menu when F3 is pressed.
    if (Ichigo::Internal::keyboard_state[Ichigo::IK_F3].down_this_frame) {
        show_debug_menu = !show_debug_menu;
    }

#define CAMERA_SPEED 2.0f

    if (Ichigo::Internal::keyboard_state[IK_W].down) {
        camera_pos += CAMERA_SPEED * Ichigo::Internal::dt * camera_front;
    }

    if (Ichigo::Internal::keyboard_state[IK_S].down) {
        camera_pos -= CAMERA_SPEED * Ichigo::Internal::dt * camera_front;
    }

    if (Ichigo::Internal::keyboard_state[IK_A].down) {
        camera_pos -= normalize(cross(camera_front, camera_up)) * CAMERA_SPEED * Ichigo::Internal::dt;
    }

    if (Ichigo::Internal::keyboard_state[IK_D].down) {
        camera_pos += normalize(cross(camera_front, camera_up)) * CAMERA_SPEED * Ichigo::Internal::dt;
    }

    static bool cursor_locked = false;
    if (Ichigo::Internal::keyboard_state[IK_ESCAPE].down_this_frame) {
        cursor_locked = !cursor_locked;
    }

    if (cursor_locked) {
        Vec2<i32> c = {(i32) Ichigo::Internal::window_width / 2, (i32) Ichigo::Internal::window_height / 2};

        i32 dx = (Ichigo::Internal::mouse.pos.x) - c.x;
        i32 dy = c.y - (Ichigo::Internal::mouse.pos.y);

        camera_pitch = clamp(camera_pitch + dy * 0.1f, -89.0f, 89.0f);
        camera_yaw   = camera_yaw + dx * 0.05f;


        if (camera_yaw >= 180.0f)  camera_yaw = -180.0f;
        if (camera_yaw <  -180.0f) camera_yaw = 180.0f;

        Ichigo::Internal::set_mouse_pos(c.x, c.y);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // Render the debug menu.
    if (show_debug_menu) {
        ImGui::SetNextWindowPos({0.0f, 0.0f});
        ImGui::SetNextWindowSize({Ichigo::Internal::window_width * 0.2f, (f32) Ichigo::Internal::window_height});
        ImGui::Begin("いちご！", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // The engine of choice for strawberry loving idols!
        ImGui::Text("苺の力で...! がんばりまー");
        if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
            // ImGui::SeparatorText("Performance");
            ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
            ImGui::SliderFloat("Target SPF", &Ichigo::Internal::target_frame_time, 0.0f, 0.1f, "%fs");
            ImGui::Text("Resolution: %ux%u (%ux%u)", window_width, window_height, Ichigo::Internal::viewport_width, Ichigo::Internal::viewport_height);

            if (ImGui::Button("120fps"))
                Ichigo::Internal::target_frame_time = 0.0083f;
            if (ImGui::Button("60fps"))
                Ichigo::Internal::target_frame_time = 0.016f;
            if (ImGui::Button("30fps"))
                Ichigo::Internal::target_frame_time = 0.033f;
        }

        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("yaw: %f\npitch: %f", camera_yaw, camera_pitch);
            ImGui::DragFloat3("Camera pos", (f32 *) &camera_pos, 0.1f);
        }

        if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Light pos", (f32 *) &light_pos, 0.1f);
            ImGui::ColorPicker3("Light colour", (f32 *) &light_colour);
            ImGui::ColorPicker3("Ambient colour", (f32 *) &ambient_light_colour);
        }

        if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Position: %u,%u", Ichigo::Internal::mouse.pos.x, Ichigo::Internal::mouse.pos.y);
            ImGui::Text("Left button: %u Middle button: %u Right Button: %u", Ichigo::Internal::mouse.left_button.down, Ichigo::Internal::mouse.middle_button.down, Ichigo::Internal::mouse.right_button.down);
            ImGui::Text("Button 4: %u Button 5: %u", Ichigo::Internal::mouse.button4.down, Ichigo::Internal::mouse.button5.down);
        }


        ImGui::End();
    }

    ImGui::EndFrame();
#endif

    if (Ichigo::Internal::window_height != 0 && Ichigo::Internal::window_width != 0) {
        frame_render();
    }
}

// Compile a GLSL shader.
static void compile_shader(u32 shader_id, const GLchar *shader_source, i32 shader_source_len) {
    Ichigo::Internal::gl.glShaderSource(shader_id, 1, &shader_source, &shader_source_len);
    Ichigo::Internal::gl.glCompileShader(shader_id);

    i32 success;
    Ichigo::Internal::gl.glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        Ichigo::Internal::gl.glGetShaderInfoLog(shader_id, sizeof(string_buffer), nullptr, string_buffer);
        ICHIGO_ERROR("Shader compilation failed:\n%s", string_buffer);
        std::exit(1);
    }
}

// Link an OpenGL shader program.
static GLuint link_program(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    GLuint program_id = Ichigo::Internal::gl.glCreateProgram();
    Ichigo::Internal::gl.glAttachShader(program_id, vertex_shader_id);
    Ichigo::Internal::gl.glAttachShader(program_id, fragment_shader_id);
    Ichigo::Internal::gl.glLinkProgram(program_id);

    i32 success;
    Ichigo::Internal::gl.glGetProgramiv(program_id, GL_LINK_STATUS, &success);

    if (!success) {
        Ichigo::Internal::gl.glGetProgramInfoLog(program_id, sizeof(string_buffer), nullptr, string_buffer);
        ICHIGO_ERROR("Shader program link failed:\n%s", string_buffer);
        std::exit(1);
    }

    return program_id;
}


static mat4 get_parent_xform(Bana::FixedArray<Bone> &skeleton, Bana::FixedArray<Bana::Optional<mat4>> &final_xforms, Bone b) {
    if (final_xforms[b.parent_index].has_value) return final_xforms[b.parent_index].value;
    if (skeleton[b.parent_index].parent_index == -1) {
        // TODO: We could just set it here, but if you call this function you should probably already have the root xform set.
        assert(false && "You must set the root xform before calling this function.");
        // final_xforms[b.parent_index] =
    }

    // final_xforms[b.parent_index] = skeleton[b.parent_index].inverse_bind_matrix * get_parent_xform(skeleton, final_xforms, skeleton[b.parent_index]);
    final_xforms[b.parent_index] = skeleton[b.parent_index].bind_matrix * get_parent_xform(skeleton, final_xforms, skeleton[b.parent_index]);
    return final_xforms[b.parent_index].value;
}

void Ichigo::Internal::init() {
    BEGIN_TIMED_BLOCK(engine_init);

    std::srand(time(nullptr));

    // Load images flipped. OpenGL coordinates are upside down :)
    stbi_set_flip_vertically_on_load(true);

    // TODO: Platform specific allocations? VirtualAlloc() on win32
    Ichigo::game_state.transient_storage_arena.pointer  = 0;
    Ichigo::game_state.transient_storage_arena.capacity = MEGABYTES(64);
    Ichigo::game_state.transient_storage_arena.data     = (u8 *) malloc(Ichigo::game_state.transient_storage_arena.capacity);

    Ichigo::game_state.permanent_storage_arena.pointer  = 0;
    Ichigo::game_state.permanent_storage_arena.capacity = MEGABYTES(256);
    Ichigo::game_state.permanent_storage_arena.data     = (u8 *) malloc(Ichigo::game_state.permanent_storage_arena.capacity);

    Ichigo::Internal::textures     = Bana::make_array<Ichigo::Texture>();
    Ichigo::Internal::audio_assets = Bana::make_array<Ichigo::Audio>();
    texture_maps                   = Bana::make_fixed_string_map<Ichigo::TextureID>(128);

#ifdef ICHIGO_DEBUG
    font_config.FontDataOwnedByAtlas = false;
    font_config.OversampleH = 2;
    font_config.OversampleV = 2;
    font_config.RasterizerMultiply = 1.5f;

    // Init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init();
    initial_style = ImGui::GetStyle();

    // Fonts
    auto io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF((void *) noto_font, noto_font_len, 18, &font_config, io.Fonts->GetGlyphRangesJapanese());
#endif

    ICHIGO_INFO("GL_VERSION=%s", Ichigo::Internal::gl.glGetString(GL_VERSION));

    Ichigo::Internal::gl.glEnable(GL_BLEND);
    Ichigo::Internal::gl.glEnable(GL_DEPTH_TEST);
    Ichigo::Internal::gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Ichigo::Internal::gl.glDepthFunc(GL_LESS);

    GLuint basic_vertex_shader        = Ichigo::Internal::gl.glCreateShader(GL_VERTEX_SHADER);
    GLuint basic_texture_shader       = Ichigo::Internal::gl.glCreateShader(GL_FRAGMENT_SHADER);
    GLuint basic_colour_vertex_shader = Ichigo::Internal::gl.glCreateShader(GL_VERTEX_SHADER);
    GLuint basic_colour_shader        = Ichigo::Internal::gl.glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(basic_vertex_shader,  (const GLchar *) basic_vert_src, basic_vert_src_len);
    ICHIGO_INFO("basic_vertex_shader compiled successfully.");
    compile_shader(basic_texture_shader, (const GLchar *) basic_texture_src, basic_texture_src_len);
    ICHIGO_INFO("basic_texture_shader compiled successfully.");

    compile_shader(basic_colour_vertex_shader, (const GLchar *) basic_colour_vert_src, basic_colour_vert_src_len);
    compile_shader(basic_colour_shader, (const GLchar *) basic_colour_src, basic_colour_src_len);

    basic_shader_program = link_program(basic_vertex_shader, basic_texture_shader);
    basic_colour_shader_program = link_program(basic_colour_vertex_shader, basic_colour_shader);

    Ichigo::Internal::gl.glDeleteShader(basic_vertex_shader);
    Ichigo::Internal::gl.glDeleteShader(basic_texture_shader);

    GL_CALL(glGenVertexArrays(1, &basic_pos_tex_vao));
    GL_CALL(glGenVertexArrays(1, &basic_pos_col_vao));
    GL_CALL(glBindVertexArray(basic_pos_tex_vao));

    GL_CALL(glVertexAttribBinding(0, 0));
    GL_CALL(glVertexAttribBinding(1, 0));
    GL_CALL(glVertexAttribBinding(2, 0));
    GL_CALL(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0));
    GL_CALL(glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(TexturedVertex, tex)));
    GL_CALL(glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(TexturedVertex, normal)));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glEnableVertexAttribArray(2));

    GL_CALL(glBindVertexArray(basic_pos_col_vao));

    GL_CALL(glVertexAttribBinding(0, 0));
    GL_CALL(glVertexAttribBinding(1, 0));
    GL_CALL(glVertexAttribBinding(2, 0));
    GL_CALL(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0));
    GL_CALL(glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(ColouredVertex, colour)));
    GL_CALL(glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(ColouredVertex, normal)));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glEnableVertexAttribArray(2));

    Ichigo::Internal::gl.glGenBuffers(1, &test_geometry_vbo);
    Ichigo::Internal::gl.glGenBuffers(1, &test_index_vbo);
    Ichigo::Internal::gl.glBindBuffer(GL_ARRAY_BUFFER, test_geometry_vbo);
    Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test_index_vbo);

    TexturedVertex vertices[] = {
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}}, // top left
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}}, // top right
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, // bottom left
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, // bottom right
    };

    u32 rectangle_indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    Ichigo::Internal::gl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    Ichigo::Internal::gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), &rectangle_indices, GL_STATIC_DRAW);

    auto skel = load_bau(Bana::temp_string("../assets/mococo.bau"), Internal::perm_allocator);
    assert(skel.has_value);
    mococo_skeleton = skel.value;

    ICHIGO_INFO("Loaded skeleton with %lld bones.", mococo_skeleton.size);
    // for (i32 i = 0; i < skeleton.size; ++i) {
    //     ICHIGO_INFO("Bone: %.*s", PFBS(skeleton[i].name));
    // }

    // calculate_xforms_and_upload_vertex_data(); // Or something

    Bana::FixedArray<Bana::Optional<mat4>> final_xforms = Bana::make_fixed_array<Bana::Optional<mat4>>(mococo_skeleton.size, Internal::perm_allocator);
    final_xforms.size = mococo_skeleton.size;
    std::memset(final_xforms.data, 0, final_xforms.size * sizeof(Bana::Optional<mat4>));

    for (i32 i = 0; i < mococo_skeleton.size; ++i) {
        if (mococo_skeleton[i].parent_index == -1) {
            // final_xforms[i] = mococo_skeleton[i].inverse_bind_matrix;
            final_xforms[i] = mococo_skeleton[i].bind_matrix;
        } else if (!final_xforms[i].has_value) {
            // final_xforms[i] = get_parent_xform(mococo_skeleton, final_xforms, mococo_skeleton[i]) * mococo_skeleton[i].inverse_bind_matrix;
            final_xforms[i] = get_parent_xform(mococo_skeleton, final_xforms, mococo_skeleton[i]) * mococo_skeleton[i].bind_matrix;
        }
    }

    Ichigo::Internal::gl.glGenBuffers(1, (GLuint *) &mococo_skeleton_debug_vbos.geometry_vbo);
    Ichigo::Internal::gl.glGenBuffers(1, (GLuint *) &mococo_skeleton_debug_vbos.index_vbo);
    Ichigo::Internal::gl.glBindBuffer(GL_ARRAY_BUFFER, mococo_skeleton_debug_vbos.geometry_vbo);
    Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mococo_skeleton_debug_vbos.index_vbo);

    Bana::FixedArray<ColouredVertex> debug_skeleton_verts = Bana::make_fixed_array<ColouredVertex>(mococo_skeleton.size * 4, Internal::temp_allocator);
    Bana::FixedArray<u32>            debug_skeleton_idx   = Bana::make_fixed_array<u32>(mococo_skeleton.size * 6, Internal::temp_allocator);

    for (i32 i = 0; i < final_xforms.size; ++i) {
    // for (i32 i = 0; i < 2; ++i) {
        u32 in[] = {
            (u32) debug_skeleton_verts.size,     (u32) debug_skeleton_verts.size + 1, (u32) debug_skeleton_verts.size + 2,
            (u32) debug_skeleton_verts.size + 1, (u32) debug_skeleton_verts.size + 2, (u32) debug_skeleton_verts.size + 3
        };

        debug_skeleton_idx.append(in, ARRAY_LEN(in));

        vec4 p = final_xforms[i].value * vec4{0.0f, 0.0f, 0.0f, 1.0f};
        vec4 p1 = final_xforms[i].value * vec4{0.02f, 0.0f, 0.0f, 1.0f};
        vec4 p2 = final_xforms[i].value * vec4{0.0f, 0.02f, 0.0f, 1.0f};
        vec4 p3 = final_xforms[i].value * vec4{0.02f, 0.02f, 0.0f, 1.0f};

        ICHIGO_INFO("Bone %d (%.*s) final_xform pos in armature space is %f,%f,%f", i, PFBS(mococo_skeleton[i].name), p.x, p.y, p.z);

        vec3 colour;
        if (mococo_skeleton[i].name == "全ての親") {
            colour = {1.0f, 0.0f, 0.0f};
        } else if (mococo_skeleton[i].name == "目先.L" || mococo_skeleton[i].name == "目先.R") {
            colour = {1.0f, 1.0f, 0.0f};
        } else if (mococo_skeleton[i].name == "紐_00.L" || mococo_skeleton[i].name == "紐_00.R") {
            colour = {0.0f, 0.0f, 1.0f};
        } else if (mococo_skeleton[i].name == "首鎖_00_08") {
            colour = {0.0f, 1.0f, 0.0f};
        } else {
            colour = {1.0f, 1.0f, 1.0f};
        }
        ColouredVertex v;
        // vec3 colour = {rand_01_f32(), rand_01_f32(), rand_01_f32()};
        v.pos = {p.x, p.y, p.z};
        v.colour = colour;
        v.normal = {0.0f, 0.0f, 0.0f};
        debug_skeleton_verts.append(v);

        v.pos = {p1.x, p1.y, p1.z};
        v.colour = colour;
        v.normal = {0.0f, 0.0f, 0.0f};
        debug_skeleton_verts.append(v);

        v.pos = {p2.x, p2.y, p2.z};
        v.colour = colour;
        v.normal = {0.0f, 0.0f, 0.0f};
        debug_skeleton_verts.append(v);

        v.pos = {p3.x, p3.y, p3.z};
        v.colour = colour;
        v.normal = {0.0f, 0.0f, 0.0f};
        debug_skeleton_verts.append(v);
    }

    Ichigo::Internal::gl.glBufferData(GL_ARRAY_BUFFER, debug_skeleton_verts.size * sizeof(ColouredVertex), debug_skeleton_verts.data, GL_STATIC_DRAW);
    Ichigo::Internal::gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, debug_skeleton_idx.size * sizeof(u32), debug_skeleton_idx.data, GL_STATIC_DRAW);

    skeleton_index_size = debug_skeleton_idx.size;
    auto mc = load_wavefront(Bana::temp_string("../assets/mococo.obj"));
    assert(mc.has_value);

    MeshGroup mococo = mc.value;
    ICHIGO_INFO("Loaded mesh group: %.*s", PFBS(mococo.name));

    Ichigo::Internal::gl.glGenBuffers(1, &mococo_geometry_vbo);
    Ichigo::Internal::gl.glBindBuffer(GL_ARRAY_BUFFER, mococo_geometry_vbo);

    Ichigo::Internal::gl.glBufferData(GL_ARRAY_BUFFER, mococo.vtx.size * sizeof(TexturedVertex), mococo.vtx.data, GL_STATIC_DRAW);

    mococo_index_vbos  = Bana::make_fixed_array<GLuint>(mococo.meshes.size);
    mococo_index_sizes = Bana::make_fixed_array<i32>(mococo.meshes.size);
    mococo_index_vbos.size = mococo.meshes.size;
    mococo_index_sizes.size = mococo.meshes.size;
    Ichigo::Internal::gl.glGenBuffers(mococo_index_vbos.capacity, mococo_index_vbos.data);

    MAKE_STACK_STRING(temp_filename_string, 256);
    mococo_materials = Bana::make_fixed_array<Material>(mococo.meshes.size, Internal::perm_allocator);

    for (isize i = 0; i < mococo.meshes.size; ++i) {
        auto &mesh = mococo.meshes[i];
        mococo_materials.append(mesh.material);
        if (mesh.material.name.length > 0)             mococo_materials[i].name             = Bana::make_string(mesh.material.name, Internal::perm_allocator);
        if (mesh.material.diffuse_map_file.length > 0) mococo_materials[i].diffuse_map_file = Bana::make_string(mesh.material.diffuse_map_file, Internal::perm_allocator);
        if (mesh.material.ambient_map_file.length > 0) mococo_materials[i].ambient_map_file = Bana::make_string(mesh.material.ambient_map_file, Internal::perm_allocator);

        if (mesh.material.diffuse_map_file.length > 0) {
            temp_filename_string.length = 0;
            Bana::string_concat(temp_filename_string, "../assets/");
            Bana::string_concat(temp_filename_string, mesh.material.diffuse_map_file);
            auto d = Ichigo::Internal::platform_read_entire_file_sync(temp_filename_string);
            if (d.has_value) {
                if (!texture_maps.get(mococo_materials[i].diffuse_map_file).has_value) {
                    texture_maps.put(mococo_materials[i].diffuse_map_file, Ichigo::load_texture(d.value.data, d.value.size));
                    ICHIGO_INFO("Loaded diffuse texture map: %.*s", PFBS(mococo_materials[i].diffuse_map_file));
                } else {
                    ICHIGO_INFO("Skipping already loaded texture map: %.*s", PFBS(mococo_materials[i].diffuse_map_file));
                }
            } else {
                ICHIGO_ERROR("Could not open texture: %.*s", PFBS(temp_filename_string));
            }
        }

        Ichigo::Internal::gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mococo_index_vbos.data[i]);
        Ichigo::Internal::gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.vtx_idx.size * sizeof(i32), mesh.vtx_idx.data, GL_STATIC_DRAW);
        mococo_index_sizes.data[i] = mesh.vtx_idx.size;
    }

    ICHIGO_INFO("Loaded %lld textures.", texture_maps.size);

    free_mesh_group(&mococo);

    bookie_tex_id = Ichigo::load_texture(bookie_png, bookie_png_len);

    END_TIMED_BLOCK(engine_init);
}

// This exists, maybe to save the game when you close it?
// But like, we don't really have to do anything else. The operating system is just going to clean up everything all at once when we exit.
void Ichigo::Internal::deinit() {}

void Ichigo::Internal::fill_sample_buffer(u8 *buffer, usize buffer_size, [[maybe_unused]] usize write_cursor_position_delta) {
    std::memset(buffer, 0, buffer_size);
    // Ichigo::Mixer::mix_into_buffer((AudioFrame2ChI16LE *) buffer, buffer_size, write_cursor_position_delta);
}
