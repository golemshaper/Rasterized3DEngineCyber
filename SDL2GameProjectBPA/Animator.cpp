#include "Animator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


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
void Animator::LoadAnimationFromFile(const std::string path, const std::string& animName)
{
    std::ifstream file(path);
    std::string str;
    std::string file_contents;

    if (!file.is_open()) {
        file_contents = "F:" + path;
    }

    while (std::getline(file, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    }
    LoadAnimationFromString(file_contents,animName);
}
void Animator::LoadAnimationFromString(const std::string& str, const std::string& animName)
{
    std::string sb;

    sb.reserve(1024);

    //final animation data container:
    AnimatedObject CreatedAnimation;
    //vectors container:
    vec3d* vectors;
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
        std::vector<std::string>  csv = SplitByChar(sb, ',');
        char c = str[i];
        switch (mode)
        {
        case m_fps:
            if (c == '\n')
            {
                int fps = std::stoi(sb);
                mode = m_objects;
                //set data
                CreatedAnimation.fps = fps;
                sb.clear();//clearing data
                continue;
            }
            break;
        case m_objects:
            //build object list to generate ID index.
            if (c == '\n') 
            {
                if (csv.size() >= 3)
                {
                    //switch to vectors!
                    CreatedAnimation.fps = m_vectors;
                    continue;
                }
                //build name data
                CreatedAnimation.objectNames.push_back(sb);
                CreatedAnimation.objectMap[sb] = CreatedAnimation.objectNames.size() -1; //minus one, right?

                sb.clear();//clearing data
                continue;
            }

            break;
        case m_vectors:
            if (csv.size() > 3)
            {
                //we finished the vector list, since csv has more then 3 elements
                mode = m_frames;
                continue;
            }
            break;
        case m_frames:
            //build frames here.
            if (c == '\n')
            {
                Frame nFrame;
                nFrame.objId = std::stoi(csv[0]);
                nFrame.frame = std::stoi(csv[1]);
                nFrame.locId = std::stoi(csv[2]);
                nFrame.rotId = std::stoi(csv[3]);
                nFrame.scaleId = std::stoi(csv[4]);

                sb.clear();//clearing data
                continue;
            }
            break;

        default:
            break;
        }
        sb += c;
    }
    //todo: Add to the list of animated objects 
}


//String helpers:
std::vector<std::string> Animator::SplitByChar(const std::string& str, char c)
{
    std::vector<std::string> strings;
    std::istringstream f(str);
    std::string s;
    while (std::getline(f, s, c)) {
        strings.push_back(s);
    }
    return strings;
}
std::string Animator::trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;

    return s.substr(start, end - start);
}

vec3d Animator::InterpolatedVectorByID(Animation anim, int a, int b, float c)
{
    vec3d va = anim.vectors[a];
    vec3d vb = anim.vectors[b];
    vec3d result = va + (vb - va) * c; //lerp
    return result;
}

void Animator::InitializeAnim()
{
    if (animatedObjects.size()<=0)
    {
        AnimatedObject animObj;
        animatedObjects.push_back(animObj);
    }
}

void Animator::Tick(float DeltaTime)
{
    for (int i = 0; i < animatedObjects.size(); i++)
    {
        float fps = animatedObjects[i].fps;

        float currentFPS = 1.0f / DeltaTime;
        float effectiveDT = DeltaTime * (currentFPS / fps);

        float totalTime = animatedObjects[i].totalTime += effectiveDT;
        animatedObjects[i].currentFrame = (int)(totalTime * fps);



    }
}
