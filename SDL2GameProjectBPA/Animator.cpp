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
    //data holders
   
    std::vector<std::string> csv = SplitByChar("", ',');
    std::vector<std::string> next_csv = SplitByChar("", ',');
    std::vector<std::string> animation_data = SplitByChar(str, '\n');
    std::string sb = "";
    //final animation data container:
    AnimationSource CreatedAnimation;
    Animation* anim = new Animation();
    CreatedAnimation.animation = anim;
    //parse modes
    int mode = 0;
    const int m_fps = 0;
    const int m_objects = 1;
    const int m_vectors = 2;
    const int m_frames = 3;

    for (std::size_t i = 0; i < animation_data.size(); ++i)
    {
        //current data
        sb = animation_data[i];
        csv = SplitByChar(sb, ',');

        //look ahead:
        if (i + 1 < animation_data.size())
        {
            next_csv = SplitByChar(animation_data[i + 1], ',');
        }
        else
        {
            next_csv.clear();
        }

        switch (mode)
        {
            case m_fps:
                csv = SplitByChar(sb, ',');
                mode = m_objects;
                //set data
                CreatedAnimation.fps = std::stoi(sb);;
                sb.clear();//clearing data
                continue;
            case m_objects:
                //build object list to generate ID index.
           
               
                //build name data
                CreatedAnimation.objectNames.push_back(sb);
                CreatedAnimation.objectMap[sb] = CreatedAnimation.objectNames.size() - 1; //minus one, right?

                if (next_csv.size() >= 3)
                {
                    //switch to vectors!
                    mode = m_vectors;
                    continue;
                }

                continue;
            case m_vectors:
                
                //we finished the vector list, since csv has more then 3 elements
                vec3d nVector = vec3d{ std::stof(csv[0]) ,std::stof(csv[1]) ,std::stof(csv[2]) };
                CreatedAnimation.animation->vectors.push_back(nVector);

                if (next_csv.size() > 3)
                {
                    mode = m_frames;
                    sb.clear();
                    continue;
                }

                continue;
            case m_frames:
                //build frames here.
            
                Frame nFrame;
                nFrame.objId = std::stoi(csv[0]);
                nFrame.frame = std::stoi(csv[1]);
                nFrame.locId = std::stoi(csv[2]);
                nFrame.rotId = std::stoi(csv[3]);
                nFrame.scaleId = std::stoi(csv[4]);
                CreatedAnimation.animation->frames.push_back(nFrame);
                sb.clear();//clearing data
                continue;

            default:
                break;
        }
       
    }
    animationSources.push_back(CreatedAnimation);
    
}



//String helpers:
std::vector<std::string> Animator::SplitByChar(const std::string& str, char c)
{
   // std::cout << "SplitByChar called with: [" << str << "]\n";

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

Frame Animator::RawFrameDataByFrameValue(int sourceIndex, int curObj, int curFrame)
{
    //slow probably, do better please...
    int resultIndex = 0;
    for (int i = 0; i < animationSources[sourceIndex].animation->frames.size(); i++)
    {
       Frame f = animationSources[sourceIndex].animation->frames[i];
       if (f.objId != curObj) { continue; }
       if (f.frame >  curFrame) { continue; }

       resultIndex = i;
    }
    return animationSources[sourceIndex].animation->frames[resultIndex];
}

int Animator::GetNextFrameWithAnimationData(int sourceIndex, int curObj, int curFrame)
{
    for (int i = 0; i < animationSources[sourceIndex].animation->frames.size(); i++)
    {
        Frame f = animationSources[sourceIndex].animation->frames[i];
        if (f.objId != curObj) { continue; }
        if (f.frame > curFrame)
        {
            return f.frame;
        }
    }
    return 0;
}

int Animator::GetPrevFrameWithAnimationData(int sourceIndex, int curObj, int curFrame)
{
    for (int i = 0; i < animationSources[sourceIndex].animation->frames.size(); i++)
    {
        Frame f = animationSources[sourceIndex].animation->frames[i];
        if (f.objId != curObj) { continue; }
        if (f.frame < curFrame)
        {
            return f.frame;
        }
    }
    return GetNextFrameWithAnimationData(sourceIndex, curObj, curFrame);
}

float Animator::GetBetweenFrameTime(float currentFrame, float lastKey, float nextKey, float totalFrames)
{
    //kind of hate this one.
    float tGlobal = currentFrame / totalFrames;
    float tStart = lastKey / totalFrames;
    float tEnd = nextKey / totalFrames;

    float t = (tGlobal - tStart) / (tEnd - tStart);
    float result = t;
    //sad mans clamp:
    if (result < 0.0f)result = 0.0f;
    if (result > 1.0f)result = 1.0f;
    return result;
}




std::string Animator::VectorIdToString(Animation anim, int a)
{
    std::string sb =
        std::to_string((int)anim.vectors[a].x) + "," +
        std::to_string((int)anim.vectors[a].y) + "," +
        std::to_string((int)anim.vectors[a].z);

    return sb;
}

std::string Animator::VectorToString( vec3d a)
{
    std::string sb = 
        std::to_string((float)a.x) + "," +
        std::to_string((float)a.y) + "," +
        std::to_string((float)a.z);
    return sb;
}

void Animator::InitializeAnim()
{
    if (animationSources.size()<=0)
    {
        AnimationSource animObj;
        animationSources.push_back(animObj);
    }
}

void Animator::Tick(float DeltaTime)
{
    for (int i = 0; i < animationSources.size(); i++)
    {
        float fps = animationSources[i].fps;

        float currentFPS = 1.0f / DeltaTime;
        float effectiveDT = DeltaTime * (currentFPS / fps);

        float totalTime = animationSources[i].totalTime += effectiveDT;
        animationSources[i].currentFrame = (int)(totalTime * fps);



    }
}
