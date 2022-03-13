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
