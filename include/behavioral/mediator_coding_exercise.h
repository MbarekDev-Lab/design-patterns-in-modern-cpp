#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Forward declaration
struct Participant;

// ============================================================================
// Mediator - Central coordinator for participants
// ============================================================================
class Mediator
{
public:
    vector<Participant *> participants;

    void broadcast(Participant *origin, int value);

    int get_participant_count() const
    {
        return participants.size();
    }

    void clear()
    {
        participants.clear();
    }
};

// ============================================================================
// Participant - Broadcasts values and receives broadcasts
// ============================================================================
struct Participant
{
    int value = 0;
    Mediator &mediator;

    Participant(Mediator &mediator) : mediator(mediator)
    {
        mediator.participants.push_back(this);
    }

    void say(int val)
    {
        mediator.broadcast(this, val);
    }

    void receive(int val)
    {
        value += val;
    }

    int get_value() const
    {
        return value;
    }

    void set_value(int val)
    {
        value = val;
    }
};

// ============================================================================
// Mediator implementation (after Participant is complete)
// ============================================================================
inline void Mediator::broadcast(Participant *origin, int value)
{
    for (auto participant : participants)
    {
        if (participant != origin)
        {
            participant->receive(value);
        }
    }
}
