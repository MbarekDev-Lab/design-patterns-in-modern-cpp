#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// ============================================================================
// MEDIATOR PATTERN: Chat Room Communication System
// ============================================================================
// The Mediator pattern defines an object that encapsulates how a set of objects
// interact. This creates a one-to-many dependency where instead of objects
// communicating directly, they communicate through a central mediator.
//
// Key Components:
// - Mediator (ChatRoom): Central hub that manages communication
// - Colleague (Person): Objects that communicate through the mediator
// ============================================================================

// Forward declaration
struct Person;

// ============================================================================
// MEDIATOR: ChatRoom (central communication hub)
// ============================================================================
struct ChatRoom
{
    vector<Person *> people;

    // ========================================================================
    // Broadcast a message to all participants except sender
    // ========================================================================
    void broadcast(const string &origin, const string &message);

    // ========================================================================
    // Send a private message to a specific participant
    // ========================================================================
    void message(const string &origin,
                 const string &who,
                 const string &message);

    // ========================================================================
    // Add a new participant to the chat room
    // ========================================================================
    void join(Person *p);

    // ========================================================================
    // Remove a participant from the chat room
    // ========================================================================
    void leave(Person *p);

    // ========================================================================
    // Get number of participants
    // ========================================================================
    int participant_count() const
    {
        return people.size();
    }

    // ========================================================================
    // Check if a person is in the room
    // ========================================================================
    bool has_participant(const string &name) const;
};

// ============================================================================
// COLLEAGUE: Person (communicates through ChatRoom mediator)
// ============================================================================
struct Person
{
    string name;
    ChatRoom *room;
    vector<string> chat_log;

    explicit Person(const string &name)
        : name(name), room(nullptr)
    {
    }

    // ========================================================================
    // Send a message to all participants in the room
    // ========================================================================
    void say(const string &message) const
    {
        if (room)
            room->broadcast(name, message);
    }

    // ========================================================================
    // Send a private message to a specific participant
    // ========================================================================
    void private_message(const string &who, const string &message) const
    {
        if (room)
            room->message(name, who, message);
    }

    // ========================================================================
    // Receive a message (called by ChatRoom mediator)
    // ========================================================================
    void receive(const string &origin, const string &message)
    {
        string formatted_msg = origin + ": \"" + message + "\"";
        cout << "[" << name << "'s chat] " << formatted_msg << "\n";
        chat_log.push_back(formatted_msg);
    }

    // ========================================================================
    // Get chat history
    // ========================================================================
    vector<string> get_chat_log() const
    {
        return chat_log;
    }

    // ========================================================================
    // Check if person is in a room
    // ========================================================================
    bool is_in_room() const
    {
        return room != nullptr;
    }

    // ========================================================================
    // Equality operators
    // ========================================================================
    bool operator==(const Person &other) const
    {
        return name == other.name;
    }

    bool operator!=(const Person &other) const
    {
        return !(other == *this);
    }
};

// ============================================================================
// ChatRoom inline implementations (defined after Person struct)
// ============================================================================

inline void ChatRoom::broadcast(const string &origin, const string &message)
{
    for (auto p : people)
    {
        if (p->name != origin)
            p->receive(origin, message);
    }
}

inline void ChatRoom::message(const string &origin,
                              const string &who,
                              const string &message)
{
    auto target = find_if(
        begin(people),
        end(people),
        [&](const Person *p)
        {
            return p->name == who;
        });

    if (target != end(people))
    {
        (*target)->receive(origin, message);
    }
}

inline void ChatRoom::join(Person *p)
{
    string join_msg = p->name + " joins the chat";
    broadcast("room", join_msg);
    p->room = this;
    people.push_back(p);
}

inline void ChatRoom::leave(Person *p)
{
    people.erase(remove(begin(people), end(people), p), end(people));
    string leave_msg = p->name + " leaves the chat";
    broadcast("room", leave_msg);
    p->room = nullptr;
}

inline bool ChatRoom::has_participant(const string &name) const
{
    return find_if(begin(people), end(people),
                   [&](const Person *p)
                   {
                       return p->name == name;
                   }) != end(people);
}
