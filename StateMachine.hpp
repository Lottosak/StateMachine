#include <iostream>
#include <tuple>
#include <variant>
#include <functional>

template <typename... States>
class StateMachine
{
private:
    // tuple of machine states
    std::tuple<States...> states;
    // current state
    std::variant<States *...> currentState(&std::get<0>(states));

public:
    StateMachine();
    ~StateMachine();

    // change currentState to state
    template <typename State>
    void transitionTo()
    {
        currentState = &std::get<State>(states);
    }

    // handle event, callback execute on current state
    template <typename Event>
    void handle(const Event &event)
    {
        std::visit([&event](auto statePtr)
                   { statePtr->handle(event).execute(*this); },
                   currentState);
    }
};

// transition struct -> executes machine transition
template <typename State>
struct TransitionTo
{
    template <typename Machine>
    void execute(Machine &machine)
    {
        machine.transitionTo<State>();
    }
};

// donothing struct
struct DoNothing
{
    template <typename Machine>
    void execute(Machine &machine)
    {
    }
};
