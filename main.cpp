#include <iostream>
#include "StateMachine.hpp"

// test example - patroling
// use case - walk -> arrive -> patrol -> stop patroling -> walk
struct WalkState;
struct PatrolingState;

struct ArriveEvent
{
};
struct FinishPatrolingEvent
{
};

// Walking state -> when arrive event transition to patroling otherwise do nothing
struct WalkState
{
    TransitionTo<PatrolingState> handle(const ArriveEvent &)
    {
        std::cout << "Arrived at destination." << std::endl;
        return {};
    }

    DoNothing handle(const FinishPatrolingEvent &)
    {
        std::cout << "Already finished looking (in walking state)." << std::endl;
        return {};
    }
};

// Patroling state -> when finished patroling then start walking else do nothing
struct PatrolingState
{
    TransitionTo<WalkState> handle(const FinishPatrolingEvent &)
    {
        std::cout << "Finished looking around." << std::endl;
        return {};
    }

    DoNothing handle(const ArriveEvent &)
    {
        std::cout << "Already in looking state." << std::endl;
        return {};
    }
};

using PatrolingSM = StateMachine<WalkState, PatrolingState>;

int main(void)
{

    // start walking to point
    auto patrolingSM = PatrolingSM();
    // patroling
    patrolingSM.handle(ArriveEvent());
    // start walking to another point
    patrolingSM.handle(FinishPatrolingEvent());
}