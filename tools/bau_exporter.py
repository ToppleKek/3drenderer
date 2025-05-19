import bpy
import mathutils
import math
from bpy_extras.io_utils import axis_conversion

## Useful stuff
## Gives us all the bones: for bone in arm.data.bones: print(bone)
#>>> for fcurve in actions.fcurves:
# ...         print("New fcurve")
# ...         for i, keyframe in enumerate(fcurve.keyframe_points):
# ...             print(f"keyframe {i}: {keyframe.co}")

##
# (bone count)  Bc <num_bones>
# (bone def)    Bd <parent_idx> <name>
#               Bbm <bind_matrix>
#               Bib <inverse_bind>
# (bone weight) Bw <vtx_idx> <n> {<bone_idx> <weight>} ... n times

# bpy.ops.script.reload()  REMVOES OLD MENU ITEMS

BAU_VERSION_NUMBER = 1
BAU_ANIM_SAMPLE_RATE = 30

def matrix_to_string(matrix):
    ret = ""
    for row in matrix:
        for e in row:
            ret += f" {e:.6f}"
        ret += "\n"

    return ret[:-1]

def vec3_to_string(v):
    return f"{v.x:.6f} {v.y:.6f} {v.z:.6f}"

def quaternion_to_string(q):
    return f"{q.w:.6f} {q.x:.6f} {q.y:.6f} {q.z:.6f}"

def do_export(context, filepath):
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(f"{BAU_VERSION_NUMBER} # BAU file version number.\n")

        obj = context.selected_objects[0]
        arm = obj.find_armature()

        # Bone count (Bc).
        bone_count = len(arm.data.bones.keys())
        f.write(f"Bc {bone_count}\n")

        axis_convert = axis_conversion(from_forward='-Y', from_up='Z', to_forward='Z', to_up='Y').to_4x4()

        root_found = False
        for bone in arm.data.bones:
            parent = -1
            bind_matrix = None
            bind_matrix_str = ""
            if bone.parent is None:
                if root_found:
                    print("BAU ERROR: Found more than one root node!")
                    return {'CANCELLED'}

                root_found = True
            else:
                parent = arm.data.bones.find(bone.parent.name)

            bind_matrix = axis_convert @ bone.matrix_local
            inverse_bind_matrix = bind_matrix.inverted()

            bind_matrix_str  = matrix_to_string(bind_matrix)
            inverse_bind_str = matrix_to_string(inverse_bind_matrix)

            f.write(f"Bd {parent} {bone.name}\nBbm{bind_matrix_str}\nBib{inverse_bind_str}\n")

        f.write(f"Vc {len(obj.data.vertices)}\n")
        for i, vertex in enumerate(obj.data.vertices):
            f.write(f"Bw {i} {len(vertex.groups)}")
            for group in vertex.groups:
                group_name = obj.vertex_groups[group.group].name
                bone_index = arm.data.bones.find(group_name)
                if bone_index == -1:
                    print("BAU ERROR: No corresponding bone for vertex group {group_name} (group index: {group.group})")
                    return {'CANCELLED'}

                f.write(f" {bone_index} {group.weight:.6f}")
            f.write("\n")

    return {'FINISHED'}

def do_animation_export(context, filepath):
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(f"{BAU_VERSION_NUMBER} # BAU file version number.\n")
        f.write(f"Sr {BAU_ANIM_SAMPLE_RATE}\n")

        obj = context.selected_objects[0]
        arm = obj.find_armature()

        bone_count = len(arm.pose.bones.keys())
        f.write(f"Bc {bone_count}\n")

        axis_convert = axis_conversion(from_forward='-Y', from_up='Z', to_forward='Z', to_up='Y').to_4x4()

        f.write(f"Sc {context.scene.frame_end - context.scene.frame_start}\n")
        for frame in range(context.scene.frame_start, context.scene.frame_end):
            context.scene.frame_set(frame)
            f.write(f"S {frame}\n")
            for i, pose_bone in enumerate(arm.pose.bones):
                matrix = axis_convert @ pose_bone.matrix
                if pose_bone.parent:
                    parent = axis_convert @ pose_bone.parent.matrix
                    matrix = parent.inverted() @ matrix

                loc, rot, scale = matrix.decompose()
                f.write(f"Bx {i} {vec3_to_string(loc)} {quaternion_to_string(rot)} {vec3_to_string(scale)}\n")

    return {'FINISHED'}

# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class ExportBauArmature(Operator, ExportHelper):
    """Export an armature to BAU"""
    bl_idname = "bau_exporter.export"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "BAU BAU"

    # ExportHelper mix-in class uses this.
    filename_ext = ".bau"

    filter_glob: StringProperty(
        default="*.bau",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
#    use_setting: BoolProperty(
#        name="Example Boolean",
#        description="Example Tooltip",
#        default=True,
#    )

#    type: EnumProperty(
#        name="Example Enum",
#        description="Choose between two items",
#        items=(
#            ('OPT_A', "First Option", "Description one"),
#            ('OPT_B', "Second Option", "Description two"),
#        ),
#        default='OPT_A',
#    )

    def execute(self, context):
        return do_export(context, self.filepath)

class ExportBauAnimation(Operator, ExportHelper):
    """Export an animation to BAU"""
    bl_idname = "bau_exporter.export_anim"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "BAU BAU"

    # ExportHelper mix-in class uses this.
    filename_ext = ".bau_anim"

    filter_glob: StringProperty(
        default="*.bau_anim",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
#    use_setting: BoolProperty(
#        name="Example Boolean",
#        description="Example Tooltip",
#        default=True,
#    )

#    type: EnumProperty(
#        name="Example Enum",
#        description="Choose between two items",
#        items=(
#            ('OPT_A', "First Option", "Description one"),
#            ('OPT_B', "Second Option", "Description two"),
#        ),
#        default='OPT_A',
#    )

    def execute(self, context):
        return do_animation_export(context, self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportBauArmature.bl_idname, text="Basic Unified Armature (.bau)")
    self.layout.operator(ExportBauAnimation.bl_idname, text="Basic Unified Armature Animation (.bau_anim)")

# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access).
def register():
    # bpy.ops.script.reload() # nocheckin
    bpy.utils.register_class(ExportBauArmature)
    bpy.utils.register_class(ExportBauAnimation)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.register_class(ExportBauArmature)
    bpy.utils.unregister_class(ExportBauAnimation)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    # get_skinning_weights()
   register()

#    # test call
   # bpy.ops.export_test.some_data('INVOKE_DEFAULT')
