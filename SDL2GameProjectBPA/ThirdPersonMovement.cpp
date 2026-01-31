#include "ThirdPersonMovement.h"

void ThirdPersonMovement::ApplyMovement(float DeltaTime, DrawScratchSpace* MyScratch)
{
    //Auto tick input:
    LastSafePos = Pos;
    MyScratch->Input->Tick(DeltaTime);

    //USE FOR COLLISION LATER: vec3d last_safe_pos = Pos;
    vec3d forward = MyScratch->Normalize(MyScratch->CameraTargetLoc); //is this always correct if the camera moves? No idea...But I suspect so...

    vec3d up = { 0, 1, 0 };
    vec3d right = MyScratch->Normalize(MyScratch->CrossProduct(up, forward));

    //Flatten ground movement
    forward.y = 0;
    right.y = 0;

    forward = MyScratch->Normalize(forward);
    right = MyScratch->Normalize(right);

    //Apply Movement
    vec3d Move = (right * MyScratch->Input->GetMovementX()) + (forward * MyScratch->Input->GetMovementY());
    Pos = Pos + Move * Speed * DeltaTime;



     //Apply rotation
     Rot = vec3d{ 1.0f + (MyScratch->Input->GetMovementY() * -0.3f),0.0f,3.0f + (MyScratch->Input->GetMovementX() * 0.35f) };

}

void ThirdPersonMovement::ApplyGroundSnap(const Mesh& mesh, DrawScratchSpace* MyScratch, vec3d offsetMe)
{
    Pos = MyScratch->SnapToMesh(Pos, mesh, vec3d{ 0,0,0 }) + offsetMe;
    if (MyScratch->LastSnapToMeshResult == false)
    {
        //No walking off of terrain
        Pos = LastSafePos;
    }
}
