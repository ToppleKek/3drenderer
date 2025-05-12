import bpy
import mathutils

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

def matrix_to_string(matrix):
    ret = ""
    for row in matrix:
        for e in row:
            ret += f" {e:.6f}"
        ret += "\n"

    return ret[:-1]

def do_export(context, filepath):
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(f"{BAU_VERSION_NUMBER} # BAU file version number.\n")

        obj = context.selected_objects[0]
        arm = obj.find_armature()

        # Bone count (Bc).
        bone_count = len(arm.data.bones.keys())
        f.write(f"Bc {bone_count}\n")

        bone_map = {}
        for i, bone in enumerate(arm.data.bones):
            bone_map[bone.name] = i

        axis_convert = mathutils.Matrix((
            (1,0,0,0),
            (0,0,-1,0),
            (0,1,0,0),
            (0,0,0,1),
        ))

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
                # bind_matrix = mathutils.Matrix.Translation(bone.head) @ bone.matrix.to_4x4()
                bind_matrix = bone.matrix_local.copy()
            else:
                parent = bone_map[bone.parent.name]
                # bind_matrix = mathutils.Matrix.Translation(bone.head) @ bone.matrix.to_4x4()
                bind_matrix = bone.parent.matrix_local.inverted() @ bone.matrix_local

            inverse_bind_matrix = bind_matrix.inverted()
            # inverse_bind_matrix = axis_convert @ inverse_bind_matrix
            # bind_matrix = axis_convert @ bind_matrix

            bind_matrix_str  = matrix_to_string(bind_matrix)
            inverse_bind_str = matrix_to_string(inverse_bind_matrix)

            f.write(f"Bd {parent} {bone.name}\nBbm{bind_matrix_str}\nBib{inverse_bind_str}\n")

        # group_to_bone = {i: group.name for i, group in enumerate(obj.vertex_groups)}

        for i, vertex in enumerate(obj.data.vertices):
            f.write(f"Bw {i} {len(vertex.groups)}")
            for group in vertex.groups:
                f.write(f" {group.group} {group.weight:.6f}")
                # print(f"group: {group_to_bone[group.group]} weight: {group.weight}")
            f.write("\n")


    return {'FINISHED'}

def do_animation_export(context, filepath):
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(f"{BAU_VERSION_NUMBER} # BAU file version number.\n")

        obj = context.selected_objects[0]
        arm = obj.find_armature()

        for i, pose_bone in enumerate(arm.pose.bones):
            loc = None
            rot = None
            scale = None

            if pose_bone.parent is None:
                loc, rot, scale = pose_bone.matrix.decompose()
            else:
                loc, rot, scale = (pose_bone.parent.matrix.inverted() @ pose_bone.matrix).decompose()
            f.write(f"Bx {i} {loc} {rot} {scale}\n")

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
