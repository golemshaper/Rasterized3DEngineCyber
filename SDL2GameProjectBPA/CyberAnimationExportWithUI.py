bl_info = {
    "name": "BPACyberEngineTools Extension",
    "author": "Brian",
    "version": (1, 0),
    "blender": (4, 0, 0),
    "location": "View3D > N-Panel > BPACyberEngineTools",
    "description": "Adds extra tools to the BPACyberEngineTools panel",
    "category": "Import-Export",
}

import bpy

# Operator: file browser
class BPACE_OT_SelectAnimPath(bpy.types.Operator):
    bl_idname = "bpacyber.select_path"
    bl_label = "Select Export Path"

    filepath: bpy.props.StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        context.scene.bpace_anim_path = self.filepath
        return {'FINISHED'}

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}


# Operator: some action button
class BPACE_OT_SendToEngine(bpy.types.Operator):
    bl_idname = "bpacyber.extra_action"
    bl_label = "Run Extra Action"

    def execute(self, context):
        print("Anim export action triggered with path:", context.scene.bpace_anim_path)
        export_animation_data(context.scene.bpace_anim_path+"unnamedAnimation.txt")

        return {'FINISHED'}


# UI injection into your existing panel
def extra_buttons(self, context):
    layout = self.layout
    layout.separator()
    layout.label(text="Animation Exporter:")

    # Show the selected path
    layout.prop(context.scene, "bpace_anim_path", text="AnimPath")

    # File browser button
    layout.operator("bpacyber.select_path", icon='FILE_FOLDER')

    # Action button
    layout.operator("bpacyber.extra_action", icon='FILE_TICK')


def register():
    bpy.utils.register_class(BPACE_OT_SelectAnimPath)
    bpy.utils.register_class(BPACE_OT_SendToEngine)

    bpy.types.Scene.bpace_anim_path = bpy.props.StringProperty(name="Anim Path")

    bpy.types.VIEW3D_PT_bpa_cyber_engine_tools.append(extra_buttons)


def unregister():
    bpy.types.VIEW3D_PT_bpa_cyber_engine_tools.remove(extra_buttons)

    del bpy.types.Scene.bpace_anim_path

    bpy.utils.unregister_class(BPACE_OT_SendToEngine)
    bpy.utils.unregister_class(BPACE_OT_SelectAnimPath)


if __name__ == "__main__":
    register()





#----------------------------------------------------------------
import bpy
import os

#FILE ANATOMY:
'''
~~FPS~~
24 
~~OBJ NAMES~~
Sphere 
Cone
~~ ALL VECTORS, NO TPYE ~~
0.000000,0.000000,0.000000
1.000000,1.000000,1.000000
-4.060116,0.852744,3.955886
0.363591,0.363591,0.363591
-0.590768,0.332596,1.663459
0.395553,1.062668,-0.694348
0.496769,0.496769,0.496769
2.903442,0.156097,1.166904
-0.187108,1.096504,-0.351437
~~KEYFRAMES~~
#Object ID, Keyframe, Location, Rotation, Scale
0,1,0,0,1
0,48,2,0,3
1,-17,4,5,6
1,91,7,8,1
'''

# ------------------------------------------------------------
# Unified vector table + lookup
# ------------------------------------------------------------

vector_table = []
vector_lookup = {}

def vec_key(vec):
    return (round(vec.x, 6), round(vec.y, 6), round(vec.z, 6))

def get_vec_id(vec):
    key = vec_key(vec)
    if key not in vector_lookup:
        vector_lookup[key] = len(vector_table)
        vector_table.append(key)
    return vector_lookup[key]

def get_obj_id(obj_name,objs):
    #get the name and a list of objects, and return the index of that name
    index  = -1
    for obj in objs:
        index = index +1
        if obj.name == obj_name:
            return index
    return index

# ------------------------------------------------------------
# Modern Blender 5.x API: safely extract fcurves
# ------------------------------------------------------------
def get_modern_fcurves(action):
    if not action:
        return []

    if not action.layers:
        return []

    layer = action.layers[0]
    if not layer.strips:
        return []

    strip = layer.strips[0]

    if not action.slots:
        return []

    slot = action.slots[0]

    try:
        bag = strip.channelbag(slot)
    except Exception:
        return []

    return bag.fcurves

# ------------------------------------------------------------
# Animation Export Function
# ------------------------------------------------------------
def export_animation_data(filepath, bake=False):
    scene = bpy.context.scene
    fps = scene.render.fps
    depsgraph = bpy.context.evaluated_depsgraph_get()

    objs = [o for o in scene.objects if o.type == "MESH"]

    keyframes = {}

    for obj in objs:
        print(f"\nExporting {obj.name}")

        keyframes[obj.name] = []
        frames = set()

        anim = obj.animation_data
        action = anim.action if anim else None

        fcurves = get_modern_fcurves(action)

        # Collect keyframe frames
        for fc in fcurves:
            for kp in fc.keyframe_points:
                frames.add(int(kp.co.x))

        # Optional baking
        if bake and frames:
            start = min(frames)
            end = max(frames)
            frames = set(range(start, end + 1))

        # Evaluate transforms
        for frame in sorted(frames):
            scene.frame_set(frame)
            obj_eval = obj.evaluated_get(depsgraph)

            pos_id   = get_vec_id(obj_eval.location)
            rot_id   = get_vec_id(obj_eval.rotation_euler)
            scale_id = get_vec_id(obj_eval.scale)

            keyframes[obj.name].append((frame, pos_id, rot_id, scale_id))

    # ------------------------------------------------------------
    # Write file
    # ------------------------------------------------------------
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"{fps}\n")

        # Object names
        for obj in objs:
            f.write(f"{obj.name}\n")

        # Unified vector table
        for vec in vector_table:
            f.write(f"{vec[0]:.6f},{vec[1]:.6f},{vec[2]:.6f}\n")

        # Animation rows: indices into the single table
        for obj in objs:
            for frame, pid, rid, sid in keyframes[obj.name]:
                f.write(f"{get_obj_id(obj.name,objs)},{frame},{pid},{rid},{sid}\n")

    print("\nExport complete:", filepath)

