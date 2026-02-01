import bpy

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


# ---------------------------------------------------------
# 2. Operator that calls your exporter
# ---------------------------------------------------------

class BPACE_OT_CopyMesh(bpy.types.Operator):
    bl_idname = "bpacyber.copy_mesh"
    bl_label = "Copy Mesh to Clipboard"
    bl_description = "Exports the active mesh to your custom format and copies it to the clipboard"

    def execute(self, context):
        obj = context.active_object
        if not obj or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object first")
            return {'CANCELLED'}

        export_mesh_as_custom(obj)
        self.report({'INFO'}, "Mesh copied to clipboard")
        return {'FINISHED'}


# ---------------------------------------------------------
# 3. Panel in the 3D Viewport (N-panel)
# ---------------------------------------------------------

class BPACE_PT_Tools(bpy.types.Panel):
    bl_label = "BPACyberEngineTools"
    bl_idname = "VIEW3D_PT_bpa_cyber_engine_tools"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "BPACyberEngineTools"

    def draw(self, context):
        layout = self.layout
        layout.operator("bpacyber.copy_mesh", icon='COPYDOWN')


# ---------------------------------------------------------
# 4. Registration
# ---------------------------------------------------------

classes = (
    BPACE_OT_CopyMesh,
    BPACE_PT_Tools,
)

def register():
    for c in classes:
        bpy.utils.register_class(c)

def unregister():
    for c in reversed(classes):
        bpy.utils.unregister_class(c)

if __name__ == "__main__":
    register()