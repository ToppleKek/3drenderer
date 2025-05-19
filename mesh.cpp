#include "mesh.hpp"
#include "ichigo.hpp"

static void init_mesh(Mesh *m) {
    assert(m);
    m->vtx_idx  = Bana::make_array<i32>(1024, m->allocator);
    // m->tex_idx  = Bana::make_array<i32>(1024, m->allocator);
    // m->norm_idx = Bana::make_array<i32>(1024, m->allocator);
}

static isize find_material(const Bana::Array<Material> &mats, const Bana::String &mat) {
    for (isize i = 0; i < mats.size; ++i) {
        if (mats[i].name == mat) return i;
    }

    return -1;
}

static vec3 read_vec3(Bana::BufferReader *br) {
    vec3 ret;

    ret.x = br->read_f32();
    ret.y = br->read_f32();
    ret.z = br->read_f32();

    return ret;
}

static vec4 read_vec4(Bana::BufferReader *br) {
    vec4 ret;

    ret.x = br->read_f32();
    ret.y = br->read_f32();
    ret.z = br->read_f32();
    ret.w = br->read_f32();

    return ret;
}

static Quaternion read_quaternion(Bana::BufferReader *br) {
    Quaternion ret;

    ret.w = br->read_f32();
    ret.x = br->read_f32();
    ret.y = br->read_f32();
    ret.z = br->read_f32();

    return ret;
}

static mat4 read_mat4(Bana::BufferReader *br) {
    mat4 ret;

    ret.a = read_vec4(br);
    ret.b = read_vec4(br);
    ret.c = read_vec4(br);
    ret.d = read_vec4(br);

    return ret;
}


#define VERIFY(STATEMENT) if (!STATEMENT) return {}
Bana::Optional<BauArmature> load_bau(Bana::String filename, Bana::Allocator allocator) {
    auto d = Ichigo::Internal::platform_read_entire_file_sync(filename, Ichigo::Internal::temp_allocator);
    if (!d.has_value) return {};
    auto data = d.value;
    Bana::BufferReader br = {(char *) data.data, (usize) data.size, 0};


    i32 version_number = (i32) br.read_i64();
    assert(version_number == 1);
    br.skip_to_after('\n');

    // Bone has name, parent_index, bind_matrix, and inverse_bind_matrix.
    Bana::String line = br.view_until_after('\n');
    Bana::BufferReader lbr = {(char *) line.data, (usize) line.length, 0};
    VERIFY(lbr.consume('B'));
    VERIFY(lbr.consume('c'));
    VERIFY(lbr.consume(' '));
    i32 bone_count = (i32) lbr.read_i64();

    BauArmature ret = {};
    ret.bones = Bana::make_fixed_array<Bone>(bone_count, allocator);

    for (i32 i = 0; i < bone_count; ++i) {
        Bone b;
        {
            Bana::String line = br.view_until_after('\n');

            line.data[line.length - 1] = '\0';
            Bana::BufferReader lbr = {(char *) line.data, (usize) line.length, 0};
            VERIFY(lbr.consume('B'));
            VERIFY(lbr.consume('d'));
            VERIFY(lbr.consume(' '));

            b.parent_index = (i32) lbr.read_i64();
            assert(lbr.consume(' '));
            b.name = lbr.slice_until('\0', allocator);
        }
        {
            VERIFY(br.consume('B'));
            VERIFY(br.consume('b'));
            VERIFY(br.consume('m'));
            VERIFY(br.consume(' '));

            b.bind_matrix = read_mat4(&br);
            br.skip_to_after('\n');
        }
        {
            VERIFY(br.consume('B'));
            VERIFY(br.consume('i'));
            VERIFY(br.consume('b'));
            VERIFY(br.consume(' '));

            b.inverse_bind_matrix = read_mat4(&br);
            br.skip_to_after('\n');
        }

        ret.bones.append(b);
    }

    VERIFY(br.consume('V'));
    VERIFY(br.consume('c'));
    VERIFY(br.consume(' '));

    i32 vertex_count  = (i32) br.read_i64();
    ret.skinning      = Bana::make_fixed_array<SkinningInformation>(vertex_count, allocator);
    ret.skinning.size = vertex_count;

    VERIFY(br.consume('\n'));

    for (i32 i = 0; i < vertex_count; ++i) {
        auto line = br.view_next_line_with_newline();
        line.data[line.length - 1] = '\0';
        Bana::BufferReader lbr = {(char *) line.data, (usize) line.length, 0};

        VERIFY(lbr.consume('B'));
        VERIFY(lbr.consume('w'));
        VERIFY(lbr.consume(' '));

        i32 vertex_idx = (i32) lbr.read_i64();
        assert(vertex_idx == i); // TODO: Do we really need the vertex index in the format since they are all listed in order? Probably not.

        i32 bone_influence_count = (i32) lbr.read_i64();
        for (i32 j = 0; j < bone_influence_count; ++j) {
            ret.skinning[i].bones[j]   = (i32) lbr.read_i64();
            ret.skinning[i].weights[j] = lbr.read_f32();
        }
    }

    return ret;
}

Bana::Optional<BauAnimation> load_bau_anim(Bana::String filename, Bana::Allocator allocator) {
    auto d = Ichigo::Internal::platform_read_entire_file_sync(filename, Ichigo::Internal::temp_allocator);
    if (!d.has_value) return {};
    auto data = d.value;
    Bana::BufferReader br = {(char *) data.data, (usize) data.size, 0};


    i32 version_number = (i32) br.read_i64();
    assert(version_number == 1);
    br.skip_to_after('\n');

    br.consume('S');
    br.consume('r');
    br.consume(' ');
    i32 sample_rate = (i32) br.read_i64();
    br.consume('\n');

    br.consume('B');
    br.consume('c');
    br.consume(' ');
    i32 bone_count = (i32) br.read_i64();
    br.consume('\n');

    br.consume('S');
    br.consume('c');
    br.consume(' ');
    i32 sample_count = (i32) br.read_i64();
    br.consume('\n');

    BauAnimation animation = {};
    animation.sample_rate = sample_rate;

    auto samples = Bana::make_fixed_array<Bana::FixedArray<Xform>>(sample_count, allocator);
    for (i32 sample = 0; sample < sample_count; ++sample) {
        auto this_sample_xforms = Bana::make_fixed_array<Xform>(bone_count, allocator);

        // TODO: We don't really need the sample number. Who cares? Remove it from the format perhaps?
        br.consume('S');
        br.consume(' ');
        br.skip_to_after('\n');

        for (i32 i = 0; i < bone_count; ++i) {
            assert(br.has_more_data());
            Bana::String line      = br.view_next_line_with_newline();
            // FIXME: MAJOR GARBAGE ALARM!!!
            line[line.length - 1]  = 0;
            Bana::BufferReader lbr = {line.data, line.length, 0};
            Xform xform            = {};

            lbr.consume('B');
            lbr.consume('x');

            i32 bone_idx = (i32) lbr.read_i64();
            // TODO: Not sure if we really even need the bone index in the file format.
            assert(bone_idx == i);
            xform.translation = read_vec3(&lbr);
            xform.rotation    = read_quaternion(&lbr);
            xform.scale       = read_vec3(&lbr);

            this_sample_xforms.append(xform);
        }

        samples.append(this_sample_xforms);
    }

    animation.samples = samples;
    return animation;
}

Bana::Optional<Bana::Array<Material>> load_mtl(Bana::String filename, Bana::Allocator allocator) {
    auto d = Ichigo::Internal::platform_read_entire_file_sync(filename, Ichigo::Internal::temp_allocator);
    if (!d.has_value) return {};
    auto data = d.value;
    Bana::BufferReader br = {(char *) data.data, (usize) data.size, 0};

    auto ret = Bana::make_array<Material>(32, allocator);

    bool first_mat = true;
    Material current_mat = {};
    while (br.has_more_data()) {
        Bana::String line = br.view_until_after('\n');
        if (line[0] == '#') continue;

        line.data[line.length - 1] = '\0';
        Bana::BufferReader lbr = {(char *) line.data, (usize) line.length, 0};
        Bana::String cmd = lbr.view_until(' ');
        lbr.consume(' ');

        if (cmd == "newmtl") {
            if (first_mat) {
                current_mat.name = lbr.slice_until('\0', allocator);
                first_mat = false;
                continue;
            }

            ret.append(current_mat);
            current_mat = {};
            current_mat.name = lbr.slice_until('\0', allocator);
        } else if (cmd == "Ns") {
            current_mat.specular_exponent = std::atof(lbr.current_ptr());
        } else if (cmd == "Ka") {
            current_mat.ambient_colour = read_vec3(&lbr);
        } else if (cmd == "Kd") {
            current_mat.diffuse_colour = read_vec3(&lbr);
        } else if (cmd == "Ks") {
            current_mat.specular_colour = read_vec3(&lbr);
        } else if (cmd == "Ke") {
            ICHIGO_INFO("TODO: MTL Ke/map_Ke emissive");
            // current_mat.specular_colour = read_vec3(&lbr);
        } else if (cmd == "Ni") {
            current_mat.index_of_refraction = std::atof(lbr.current_ptr());
        } else if (cmd == "d") {
            current_mat.dissolve = std::atof(lbr.current_ptr());
        } else if (cmd == "illum") {
            ICHIGO_INFO("TODO: MTL illum");
        } else if (cmd == "map_Ka") {
            assert(!current_mat.ambient_map_file.data); // If there are multiple of these statements, it is an error.
            current_mat.ambient_map_file = lbr.slice_until('\0', allocator);
        } else if (cmd == "map_Kd") {
            assert(!current_mat.diffuse_map_file.data);
            current_mat.diffuse_map_file = lbr.slice_until('\0', allocator);
        }
    }

    ret.append(current_mat);
    return ret;
}

Bana::Optional<MeshGroup> load_wavefront(Bana::String filename, Bana::Allocator allocator) {
    auto d = Ichigo::Internal::platform_read_entire_file_sync(filename, Ichigo::Internal::temp_allocator);
    if (!d.has_value) return {};
    auto data = d.value;
    Bana::BufferReader br = {(char *) data.data, (usize) data.size, 0};

    Bana::Array<vec3> norm = Bana::make_array<vec3>(1024, allocator);
    Bana::Array<vec2> tex  = Bana::make_array<vec2>(1024, allocator);
    Bana::Array<Material> mats = {};

    MeshGroup ret = {};
    ret.allocator = allocator;
    // ret.vtx_idx   = Bana::make_array<vec3>(1024, allocator);
    ret.vtx       = Bana::make_array<TexturedVertex>(1024, allocator);
    ret.meshes    = Bana::make_array<Mesh>(64, allocator);

    bool first_mesh = true;
    Mesh current_mesh = {};
    current_mesh.allocator = allocator;

    while (br.has_more_data()) {
        Bana::String line = br.view_until_after('\n');
        if (line[0] == '#') continue;

        Bana::BufferReader lbr = {(char *) line.data, (usize) line.length, 0};
        Bana::String cmd = lbr.view_until(' ');
        lbr.consume(' ');

        if (cmd == "mtllib") {
            Bana::String mtl_filename = lbr.view_until('\n');
            i32 slash_idx = 0;
            for (usize i = 0; i < filename.length; ++i) {
                if (filename[i] == '/' || filename[i] == '\\') slash_idx = i;
            }

            Bana::String base_path = filename;
            base_path.length = slash_idx + 1;
            MAKE_STACK_STRING(mtl_path, base_path.length + mtl_filename.length + 1);
            Bana::string_concat(mtl_path, base_path);
            Bana::string_concat(mtl_path, mtl_filename);

            auto mtl = load_mtl(mtl_path, allocator);
            if (!mtl.has_value) {
                ICHIGO_ERROR("Failed to load MTL file: %.*s", PFBS(mtl_path));
                continue;
            }

            mats = mtl.value;
            ICHIGO_INFO("Loaded %lld materials from MTL file: %.*s", mats.size, PFBS(mtl_path));
        } else if (cmd == "o") {
            ret.name = lbr.slice_until('\n', ret.allocator); // NOTE: The string is not actually 0 terminated, we just want to slice until the end of the line.
        } else if (cmd == "v") {
            // FIXME: MAJOR GARBAGE ALARM
            // atof expects the string to be 0 terminated... Stomp the newline char with a 0 to satisfy this.
            line.data[line.length - 1] = '\0';
            TexturedVertex v;
            v.pos.x = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            v.pos.y = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            v.pos.z = std::atof(lbr.current_ptr());

            v.tex = {};
            v.normal = {};

            ret.vtx.append(v);
        } else if (cmd == "vn") {
            // FIXME: MAJOR GARBAGE ALARM
            // atof expects the string to be 0 terminated... Stomp the newline char with a 0 to satisfy this.
            line.data[line.length - 1] = '\0';
            vec3 vn;
            vn.x = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            vn.y = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            vn.z = std::atof(lbr.current_ptr());

            norm.append(vn);
        } else if (cmd == "vt") {
            // FIXME: MAJOR GARBAGE ALARM
            // atof expects the string to be 0 terminated... Stomp the newline char with a 0 to satisfy this.
            line.data[line.length - 1] = '\0';
            vec2 vt;
            vt.x = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            vt.y = std::atof(lbr.current_ptr());
            lbr.skip_to_after(' ');

            tex.append(vt);
        } else if (cmd == "s") {
            ICHIGO_INFO("TODO: 's' (smoothing group)");
        } else if (cmd == "usemtl") {
            Bana::String mesh_name = lbr.slice_until('\n', ret.allocator); // NOTE: The string is not actually 0 terminated, we just want to slice until the end of the line.

            isize idx_of_mat = find_material(mats, mesh_name);
            if (idx_of_mat == -1) {
                ICHIGO_ERROR("Unable to find matching material for mesh: %.*s", PFBS(mesh_name));
            }

            if (first_mesh) {
                init_mesh(&current_mesh);
                if (idx_of_mat != -1) current_mesh.material = mats[idx_of_mat];
                current_mesh.name = mesh_name;
                first_mesh = false;
                continue;
            }

            ret.meshes.append(current_mesh);
            init_mesh(&current_mesh);
            if (idx_of_mat != -1) current_mesh.material = mats[idx_of_mat];
            current_mesh.name = mesh_name;
        } else if (cmd == "f") {
            assert(current_mesh.vtx_idx.capacity != 0);
            // FIXME: MAJOR GARBAGE ALARM
            // atoi expects the string to be 0 terminated... Stomp the newline char with a 0 to satisfy this.
            line.data[line.length - 1] = '\0';

            for (i32 i = 0; i < 3; ++i) {
                // ICHIGO_INFO("lbr currentptr: %s", lbr.current_ptr());
                i32 vtx_idx = std::atoi(lbr.current_ptr());
                lbr.skip_to_after('/');
                i32 tex_idx = std::atoi(lbr.current_ptr());
                lbr.skip_to_after('/');
                i32 norm_idx = std::atoi(lbr.current_ptr());
                lbr.skip_to_after(' ');

                ret.vtx[vtx_idx - 1].tex    = tex[tex_idx - 1];
                ret.vtx[vtx_idx - 1].normal = norm[norm_idx - 1];
                // ICHIGO_INFO("vtx_idx=%d ret.vtx[vtx_idx].tex=%f,%f", vtx_idx, ret.vtx[vtx_idx].tex.x, ret.vtx[vtx_idx].tex.y);
                current_mesh.vtx_idx.append(vtx_idx - 1);
            }

            // current_mesh.tex_idx.append(tex);
            // current_mesh.norm_idx.append(norm);
        }
    }

    free_array(&norm);
    free_array(&tex);
    free_array(&mats); // NOTE: We copy the material data into the respective mesh so we can free this array.

    return ret;
}
#undef VERIFY
