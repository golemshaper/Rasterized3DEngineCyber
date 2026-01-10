#pragma once
#include <map>
#include <functional>


class StateMachine
{
    std::map<int, std::function<void()>> TickMap;
    std::map<int, std::function<void()>> EnterMap;
    std::map<int, std::function<void()>> ExitMap;
public:
    /*
    * HOW TO MAP:
    *       class Enemy
            {
            public:
                StateMachine sm;

                void Idle()  { std::cout << "Enemy idle...\n"; }
                void Attack(){ std::cout << "Enemy attack...\n"; }

                Enemy() {
                    sm.MapState(0, [this](){ Idle(); });
                    sm.MapState(1, [this](){ Attack(); });
                }
            };
    */
    int CurrentState = -1;
    float TimeInState = 0.0f;
    float StateDeltaTime = 0.0f;
    void MapState(int StateID, std::function<void()> OnUpdate);
    void MapState(int StateID, std::function<void()> OnUpdate, std::function<void()> OnEnter);
    void MapState(int StateID, std::function<void()> OnUpdate, std::function<void()> OnEnter, std::function<void()> OnExit);
    void SetState(int NState);
    void Tick(float DeltaTime);
};