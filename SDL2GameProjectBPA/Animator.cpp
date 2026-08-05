#include "Animator.h"

void Animator::Tick(float DeltaTime)
{
    totalTime += DeltaTime;

}
/*
* Animation example file:
24
Sphere
Cone
0.000000,0.000000,0.000000
1.000000,1.000000,1.000000
-4.060116,0.852744,3.955886
0.363591,0.363591,0.363591
-0.590768,0.332596,1.663459
0.395553,1.062668,-0.694348
0.496769,0.496769,0.496769
2.903442,0.156097,1.166904
-0.187108,1.096504,-0.351437
0,1,0,0,1
0,48,2,0,3
1,-17,4,5,6
1,91,7,8,1
*/
void Animator::LoadAnimationFromString(const std::string& str, const std::string& animName)
{
    std::string sb;
    sb.reserve(1024);

    //final animation data container:
    AnimatedObject CreatedAnimation;

    //name
    CreatedAnimation.animName = animName;

    //parse modes
    int mode = 0;

    const int m_fps = 0; 
    const int m_objects = 1;
    const int m_vectors = 2;
    const int m_frames = 3;
    
    //parse
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        char c = str[i];
        switch (mode)
        {
        case m_fps:
            if (c == '\n')
            {
                int fps = std::stoi(sb);
                sb.clear();//clearing data
            }
            break;

        default:
            break;
        }
        sb += c;
    }
}
