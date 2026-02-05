import bpy
import math
import re
#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
                                            #SCENE
#===================================================================================================


# Remove Blender's .001 / .002 suffixes
def clean_name(name: str) -> str:
    return re.sub(r"\.\d+$", "", name)

def export_scene_as_custom():
    out = ""

    # ---------------------------------------
    # Collect positions, rotations, scales
    # ---------------------------------------
    positions = []
    rotations = []
    scales = []

    pos_lookup = {}
    rot_lookup = {}
    scale_lookup = {}

    obj_pos_ids = []
    obj_rot_ids = []
    obj_scale_ids = []

    objs = [o for o in bpy.context.scene.objects if o.type in {"MESH", "EMPTY"}]

    for obj in objs:
        # Position
        loc = obj.location
        pos = (loc.x, loc.z, loc.y)
        if pos not in pos_lookup:
            pos_lookup[pos] = len(positions)
            positions.append(pos)
        obj_pos_ids.append(pos_lookup[pos])

        # Rotation (radians → degrees)
        rot = obj.rotation_euler
        rot_deg = (
            math.degrees(rot.x),
            math.degrees(rot.y),
            math.degrees(rot.z),
        )
        if rot_deg not in rot_lookup:
            rot_lookup[rot_deg] = len(rotations)
            rotations.append(rot_deg)
        obj_rot_ids.append(rot_lookup[rot_deg])

        # Scale
        sc = tuple(obj.scale)
        if sc not in scale_lookup:
            scale_lookup[sc] = len(scales)
            scales.append(sc)
        obj_scale_ids.append(scale_lookup[sc])

    # ---------------------------------------
    # Write Positions
    # ---------------------------------------
    out += "Positions:\n"
    for (x, y, z) in positions:
        out += f" {x:.3f} {y:.3f} {z:.3f}\n"
    out += "\n"

    # ---------------------------------------
    # Write Rotations
    # ---------------------------------------
    out += "Rotations:\n"
    for (rx, ry, rz) in rotations:
        out += f" {rx:.3f} {rz:.3f} {ry:.3f}\n"
    out += "\n"

    # ---------------------------------------
    # Write Scales
    # ---------------------------------------
    out += "Scales:\n"
    for (sx, sy, sz) in scales:
        out += f" {sx:.3f},{sy:.3f},{sz:.3f}\n"
    out += "\n"

    # ---------------------------------------
    # Objects block
    # ---------------------------------------
    out += "Objects:\n"
    #out += "#Model, Texture, PositionID, RotationID, ScaleID, visibility, MiscTags\n"

    for i, obj in enumerate(objs):
        model = clean_name(obj.name)

        # Texture = first material name or "None"
        if obj.type == "MESH" and obj.data.materials:
            texture = obj.data.materials[0].name
        else:
            texture = "None"

        pos_id = obj_pos_ids[i]
        rot_id = obj_rot_ids[i]
        scale_id = obj_scale_ids[i]

        # Visibility
        vis = "true" if obj.visible_get() else "false"

        # Tags
        tags = []
        if "tags" in obj:
            raw = obj["tags"]
            if isinstance(raw, (list, tuple)):
                tags = list(raw)
            else:
                tags = [str(raw)]
        tag_str = ",".join(tags) if tags else ""

        out += f"{model},{texture},{pos_id},{rot_id},{scale_id},{vis},{tag_str}\n"

    out += "\n"

    # ---------------------------------------
    # DialogueTriggers
    # ---------------------------------------
    out += "DialogueTriggers:\n"
    for i, obj in enumerate(objs):
        if "dialogue" in obj:
            dlg = obj["dialogue"]
            out += f" \"{dlg}\",{i}\n"

    # ---------------------------------------
    # Assign to clipboard
    # ---------------------------------------
    bpy.context.window_manager.clipboard = out
    print("Scene exported to clipboard.")
    print(out)
    return out


# Run exporter
export_scene_as_custom()



#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
                                            #MESH
#===================================================================================================
# ---------------------------------------------------------
# 1. Your mesh export function (unchanged)
# ---------------------------------------------------------

def export_mesh_as_custom(obj):
    mesh = obj.data
    mesh.calc_loop_triangles()

    # 1. Try to find a CORNER color attribute (byte or float)
    color_layer = None
    for attr in mesh.color_attributes:
        if attr.domain == 'CORNER':
            color_layer = attr.data
            break

    WHITE = (255, 255, 255, 255)

    palette = []
    palette_lookup = {}

    def get_palette_index(rgba):
        if rgba not in palette_lookup:
            palette_lookup[rgba] = len(palette)
            palette.append(rgba)
        return palette_lookup[rgba]

    # ---------------------------
    # 2. Vectors (positions)
    # ---------------------------
    verts = [v.co for v in mesh.vertices]
    out = "vectors:\n\n"
    for v in verts:
        out += f" {v.x:.6f} {-v.z:.6f} {v.y:.6f}\n\n"

    # ---------------------------
    # 3. Tris (vertex indices)
    # ---------------------------
    out += "tris:\n\n"
    for tri in mesh.loop_triangles:
        i0, i1, i2 = tri.vertices
        out += f" {i0} {i1} {i2}\n\n"

    # ---------------------------
    # 4. Per-triangle colors → palette indices
    # ---------------------------
    tri_color_indices = []

    for tri in mesh.loop_triangles:
        tri_cols = []

        for loop_index in tri.loops:
            if color_layer:
                c = color_layer[loop_index].color
                r = int(round(c[0] * 255))
                g = int(round(c[1] * 255))
                b = int(round(c[2] * 255))
                a = int(round((c[3] if len(c) > 3 else 1.0) * 255))
                rgba = (r, g, b, a)
            else:
                rgba = WHITE

            idx = get_palette_index(rgba)
            tri_cols.append(idx)

        tri_color_indices.append(tri_cols)

    # ---------------------------
    # 5. UVs
    # ---------------------------
    uv_layer = mesh.uv_layers.active.data if mesh.uv_layers else None

    uv_list = []
    uv_lookup = {}

    def get_uv_index(uv):
        key = (round(uv[0], 6), round(uv[1], 6))
        if key not in uv_lookup:
            uv_lookup[key] = len(uv_list)
            uv_list.append(key)
        return uv_lookup[key]

    tri_uv_indices = []

    for tri in mesh.loop_triangles:
        tri_uvs = []
        for loop_index in tri.loops:
            if uv_layer:
                uv = uv_layer[loop_index].uv
                tri_uvs.append(get_uv_index(uv))
            else:
                tri_uvs.append(0)
        tri_uv_indices.append(tri_uvs)

    out += "vector2s:\n\n"
    for (u, v) in uv_list:
        out += f" {u:.6f} {v:.6f}\n\n"

    out += "uv:\n\n"
    for tri_uvs in tri_uv_indices:
        i0, i1, i2 = tri_uvs
        out += f" {i0} {i1} {i2}\n\n"

    # ---------------------------
    # 6. Palette
    # ---------------------------
    out += "palette:\n\n"
    for (r, g, b, a) in palette:
        out += f" {r} {g} {b} {a}\n\n"

    # ---------------------------
    # 7. Color indices
    # ---------------------------
    out += "color:\n\n"
    for tri_cols in tri_color_indices:
        i0, i1, i2 = tri_cols
        out += f" {i0} {i1} {i2}\n\n"

    bpy.context.window_manager.clipboard = out
    print(out)
    return out

#===================================================================================================
#===================================================================================================
#===================================================================================================
#===================================================================================================
                                        #Path options
#===================================================================================================

def register_properties():
    bpy.types.Scene.bpace_path_a = bpy.props.StringProperty(
        name="ScenePath",
        description="Scene file path",
        subtype='FILE_PATH'
    )

    bpy.types.Scene.bpace_path_b = bpy.props.StringProperty(
        name="ModelPath",
        description="Model file path",
        subtype='FILE_PATH'
    )

def unregister_properties():
    del bpy.types.Scene.bpace_path_a
    del bpy.types.Scene.bpace_path_b

class BPACE_PT_Tools(bpy.types.Panel):
    bl_label = "BPACyberEngineTools"
    bl_idname = "VIEW3D_PT_bpa_cyber_engine_tools"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "BPACyberEngineTools"

    def draw(self, context):
        layout = self.layout
        scene = context.scene

        layout.prop(scene, "bpace_path_a")
        layout.prop(scene, "bpace_path_b")

        layout.separator()

        layout.operator("bpacyber.copy_mesh", icon='COPYDOWN')
        layout.operator("bpacyber.copy_scene", icon='COPYDOWN')
        
#===================================================================================================
#===================================================================================================
                                        # PANEL INTERFACE
#===================================================================================================
# ---------------------------------------------------------
# . Operator that calls mesh exporter
# ---------------------------------------------------------

class BPACE_OT_CopyMesh(bpy.types.Operator):
    bl_idname = "bpacyber.copy_mesh"
    bl_label = "Copy Meshes to Folder"
    bl_description = "Exports selected meshes to Cyber Athena format and saves them to the ModelPath folder"

    def execute(self, context):
        scene = context.scene
        directory = scene.bpace_path_b  # ModelPath

        if not directory:
            self.report({'ERROR'}, "ModelPath not set")
            return {'CANCELLED'}

        import os
        exported_names = set()
        count = 0

        for obj in context.selected_objects:
            if obj.type != 'MESH':
                continue

            # Clean the name
            clean = bpy.path.clean_name(obj.name)

            # Skip duplicates
            if clean in exported_names:
                continue
            exported_names.add(clean)

            # Export text
            text = export_mesh_as_custom(obj)

            # Build full file path
            filename = clean + ".txt"
            full_path = os.path.join(directory, filename)

            # Write file
            try:
                with open(full_path, "w", encoding="utf-8") as f:
                    f.write(text)
                count += 1
            except Exception as e:
                self.report({'ERROR'}, f"Failed to save {filename}: {e}")

        # Clipboard behavior: copy active mesh (your original behavior)
        active = context.active_object
        if active and active.type == 'MESH':
            text = export_mesh_as_custom(active)
            bpy.context.window_manager.clipboard = text

        self.report({'INFO'}, f"Exported {count} mesh files")
        return {'FINISHED'}
# ---------------------------------------------------------
# . Operator that calls scene exporter
# ---------------------------------------------------------

class BPACE_OT_CopyScene(bpy.types.Operator):
    bl_idname = "bpacyber.copy_scene"
    bl_label = "Copy Scene to Folder"
    bl_description = "Exports the active scene to Cyber Athena format and copies it to the clipboard"

    def execute(self, context):
        scene = context.scene

        # 1. Get exported text
        text = export_scene_as_custom()

        # 2. Get the directory the user selected
        directory = scene.bpace_path_a

        if directory:
            import os

            # 3. Build filename from the .blend file name
            blend_path = bpy.data.filepath
            if blend_path:
                base = os.path.splitext(os.path.basename(blend_path))[0]
                filename = base + ".txt"
            else:
                filename = "untitled_scene.txt"

            # 4. Combine directory + filename
            full_path = os.path.join(directory, filename)

            # 5. Write the file
            try:
                with open(full_path, "w", encoding="utf-8") as f:
                    f.write(text)
                self.report({'INFO'}, f"Scene saved to {full_path}")
            except Exception as e:
                self.report({'ERROR'}, f"Failed to save file: {e}")
        else:
            self.report({'WARNING'}, "ScenePath not set — only copied to clipboard")

        return {'FINISHED'}

# ---------------------------------------------------------
# . Panel in the 3D Viewport (N-panel)
# ---------------------------------------------------------

class BPACE_PT_Tools(bpy.types.Panel):
    bl_label = "BPACyberEngineTools"
    bl_idname = "VIEW3D_PT_bpa_cyber_engine_tools"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "BPACyberEngineTools"

    def draw(self, context):
        layout = self.layout
        scene = context.scene

        layout.prop(scene, "bpace_path_a")
        layout.prop(scene, "bpace_path_b")

        layout.operator("bpacyber.copy_mesh", icon='COPYDOWN')
        layout.operator("bpacyber.copy_scene", icon='COPYDOWN')


# ---------------------------------------------------------
# . Registration
# ---------------------------------------------------------

classes = (
    BPACE_OT_CopyMesh,
    BPACE_OT_CopyScene,
    BPACE_PT_Tools,
)

def register():
    for c in classes:
        bpy.utils.register_class(c)
        register_properties()

def unregister():
    unregister_properties()
    for c in reversed(classes):
        bpy.utils.unregister_class(c)

if __name__ == "__main__":
    register()
    
#===================================================================================================