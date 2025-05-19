#pragma once

#include "common.hpp"
#include "math.hpp"

struct Material {
    Bana::String name;
    vec3 ambient_colour; // Ka
    vec3 diffuse_colour; // Kd
    vec3 specular_colour; // Ks
    f32 specular_exponent; // Ns
    f32 dissolve; // d
    f32 index_of_refraction; // Ni optical density
    // TODO: illum - Ilumination model. I really don't think I need this.
    // TODO: Ke/map_Ke - emissive?
    Bana::String ambient_map_file; // map_Ka
    Bana::String diffuse_map_file; // map_Kd
};

struct Bone {
    Bana::String name;
    i32 parent_index;
    mat4 bind_matrix;
    mat4 inverse_bind_matrix;
};

struct Mesh {
    Bana::String name;
    Bana::Array<i32> vtx_idx;
    Material material;
    // Bana::Array<i32> tex_idx;
    // Bana::Array<i32> norm_idx;
    Bana::Allocator allocator;
};

struct MeshGroup {
    Bana::String name;
    Bana::Array<TexturedVertex> vtx;
    Bana::Array<Mesh> meshes;
    Bana::Allocator allocator;
};

struct SkinningInformation {
    i32 vertex_idx;
    i32 bones[MAX_BONE_INFLUENCE];
    f32 weights[MAX_BONE_INFLUENCE];
};

struct BauArmature {
    Bana::FixedArray<Bone> bones;
    Bana::FixedArray<SkinningInformation> skinning;
};

struct BauAnimation {
    i32 sample_rate;
    Bana::FixedArray<Bana::FixedArray<Xform>> samples;
};

Bana::Optional<BauArmature> load_bau(Bana::String filename, Bana::Allocator allocator = Bana::heap_allocator);
// TODO: TEMPORARY!!! We will need a fixed array of keyframes instead or something like that. For now this just loads a single pose.
Bana::Optional<BauAnimation> load_bau_anim(Bana::String filename, Bana::Allocator allocator = Bana::heap_allocator);
Bana::Optional<Bana::Array<Material>> load_mtl(Bana::String filename, Bana::Allocator allocator = Bana::heap_allocator);
Bana::Optional<MeshGroup> load_wavefront(Bana::String filename, Bana::Allocator allocator = Bana::heap_allocator);

inline void free_mesh(Mesh *m) {
    assert(m);
    free_array(&m->vtx_idx);
    // free_array(&m->tex_idx);
    // free_array(&m->norm_idx);
    free_string(&m->name, m->allocator);
    free_string(&m->material.name, m->allocator);
    free_string(&m->material.ambient_map_file, m->allocator);
    free_string(&m->material.diffuse_map_file, m->allocator);
}

inline void free_mesh_group(MeshGroup *mg) {
    assert(mg);

    for (isize i = 0; i < mg->meshes.size; ++i) {
        free_mesh(&mg->meshes[i]);
    }

    free_array(&mg->vtx);
    free_array(&mg->meshes);
    free_string(&mg->name, mg->allocator);
}
