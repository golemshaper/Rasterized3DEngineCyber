import bpy

# Operator: file browser
class BPACE_OT_SelectPath(bpy.types.Operator):
    bl_idname = "bpacyber.select_path"
    bl_label = "Select Export Path"

    filepath: bpy.props.StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        context.scene.bpace_extra_path = self.filepath
        return {'FINISHED'}

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}


# Operator: some action button
class BPACE_OT_ExtraAction(bpy.types.Operator):
    bl_idname = "bpacyber.extra_action"
    bl_label = "Run Extra Action"

    def execute(self, context):
        print("Extra action triggered with path:", context.scene.bpace_extra_path)
        return {'FINISHED'}


# UI injection into your existing panel
def extra_buttons(self, context):
    layout = self.layout
    layout.separator()
    layout.label(text="Extra Tools (Addon 2)")

    # Show the selected path
    layout.prop(context.scene, "bpace_extra_path", text="Path")

    # File browser button
    layout.operator("bpacyber.select_path", icon='FILE_FOLDER')

    # Action button
    layout.operator("bpacyber.extra_action", icon='CHECKMARK')


def register():
    bpy.utils.register_class(BPACE_OT_SelectPath)
    bpy.utils.register_class(BPACE_OT_ExtraAction)

    bpy.types.Scene.bpace_extra_path = bpy.props.StringProperty(name="Extra Path")

    bpy.types.VIEW3D_PT_bpa_cyber_engine_tools.append(extra_buttons)


def unregister():
    bpy.types.VIEW3D_PT_bpa_cyber_engine_tools.remove(extra_buttons)

    del bpy.types.Scene.bpace_extra_path

    bpy.utils.unregister_class(BPACE_OT_ExtraAction)
    bpy.utils.unregister_class(BPACE_OT_SelectPath)


if __name__ == "__main__":
    register()
