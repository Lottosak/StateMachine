//#include <iostream>
//#include "StateMachine.hpp"
#include <iostream>
#include <tuple>
#include <variant>
#include <functional>

template <typename... States>
class StateMachine
{
private:
    std::tuple<States...> states;
    std::variant<States*...> currentState(&std::get<0>(states));
public:
    StateMachine();
    ~StateMachine();

    template <typename State>
    void transitionTo()
    {
        currentState = &std::get<State>(states);
    }

    template <typename Event>
    void handle(const Event& event)
    {
        std::visit([&event] (auto statePtr) { statePtr->handle(event).execute(*this); }, currentState);
    }
};

template<typename State>
struct TransitionTo
{
    template <typename Machine>
    void execute(Machine &machine){
        machine.template transitionTo<State>();
    }
};

struct DoNothing
{
    template <typename Machine>
    void execute(Machine &machine){

    }
};
//test example - patroling
//decision tree - walkA -> lookAround -> walkB
struct WalkState;
struct LookingState;

struct ArriveEvent {};
struct FinishLookingEvent {};

struct WalkState{
    TransitionTo<LookingState> handle(const ArriveEvent&)
    {
        std::cout << "Arrived at destination." << std::endl;
        return {};
    }

    DoNothing handle(const FinishLookingEvent&){
        std::cout << "Already finished looking (in walking state)." << std::endl;
        return {};
    }
};

struct LookingState{
    TransitionTo<WalkState> handle(const FinishLookingEvent&)
    {
        std::cout << "Finished looking around." << std::endl;
        return {};
    }

    DoNothing handle(const ArriveEvent&){
        std::cout << "Already in looking state." << std::endl;
        return {};
    }
};

using PatrolingSM = StateMachine<WalkState,LookingState>;

int main(void){

    //start walking to point
    auto patrolingSM = PatrolingSM();
    //patroling
    patrolingSM.handle(ArriveEvent());
    // start walking to another point
    patrolingSM.handle(FinishLookingEvent());
}