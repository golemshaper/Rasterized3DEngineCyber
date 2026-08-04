import bpy
import os

#TODO: Replace the object string name with an object index!

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
                f.write(f"{obj.name},{frame},{pid},{rid},{sid}\n")

    print("\nExport complete:", filepath)

# TEST
export_animation_data("C:/Users/Brian/Desktop/anim_test.txt")
