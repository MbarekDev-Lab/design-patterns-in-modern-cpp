#include <iostream>
#include <string>
#include "../../include/behavioral/mediator.h"

using namespace std;

// ============================================================================
// MEDIATOR PATTERN EXAMPLES: Chat Room Communication
// ============================================================================

// Example 1: Basic chat room with two people
void example_basic_chat()
{
    cout << "\n=== Example 1: Basic Two-Person Chat ===\n";

    ChatRoom room;

    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.say("Hello everyone!");
    bob.say("Hi Alice!");
}

// Example 2: Multiple participants joining
void example_multi_person_chat()
{
    cout << "\n=== Example 2: Multi-Person Chat Room ===\n";

    ChatRoom room;

    Person john{"John"};
    Person jane{"Jane"};
    Person simon{"Simon"};

    cout << "\n--- John joins ---\n";
    room.join(&john);

    cout << "\n--- Jane joins ---\n";
    room.join(&jane);

    cout << "\n--- John says something ---\n";
    john.say("Hi everyone!");

    cout << "\n--- Simon joins ---\n";
    room.join(&simon);

    cout << "\n--- Simon says something ---\n";
    simon.say("Hey folks, just arrived!");

    cout << "\n--- Jane responds ---\n";
    jane.say("Welcome Simon!");
}

// Example 3: Private messages
void example_private_messages()
{
    cout << "\n=== Example 3: Private Messages ===\n";

    ChatRoom room;

    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    room.join(&bob);
    room.join(&charlie);

    cout << "\n--- Alice broadcasts ---\n";
    alice.say("Hello everyone!");

    cout << "\n--- Alice sends private message to Bob ---\n";
    alice.private_message("Bob", "Hi Bob, just wanted to say hi!");

    cout << "\n--- Bob sends private message to Charlie ---\n";
    bob.private_message("Charlie", "Have you seen Alice's message?");
}

// Example 4: People joining and leaving
void example_join_and_leave()
{
    cout << "\n=== Example 4: People Joining and Leaving ===\n";

    ChatRoom room;

    Person user1{"User1"};
    Person user2{"User2"};
    Person user3{"User3"};

    cout << "\n--- Users 1 and 2 join ---\n";
    room.join(&user1);
    room.join(&user2);

    cout << "\n--- User1 says something ---\n";
    user1.say("Hello!");

    cout << "\n--- User3 joins ---\n";
    room.join(&user3);

    cout << "\n--- User2 leaves ---\n";
    room.leave(&user2);

    cout << "\n--- User3 says something ---\n";
    user3.say("Only us now!");
}

// Example 5: Chat room statistics
void example_chat_stats()
{
    cout << "\n=== Example 5: Chat Room Statistics ===\n";

    ChatRoom room;

    Person adam{"Adam"};
    Person eve{"Eve"};
    Person charlie{"Charlie"};

    cout << "Participants before: " << room.participant_count() << "\n";

    room.join(&adam);
    cout << "Participants after Adam joins: " << room.participant_count() << "\n";

    room.join(&eve);
    cout << "Participants after Eve joins: " << room.participant_count() << "\n";

    room.join(&charlie);
    cout << "Participants after Charlie joins: " << room.participant_count() << "\n";

    adam.say("Let's chat!");
    eve.say("Sure!");

    cout << "\nRoommates:\n";
    cout << "- Has Adam: " << (room.has_participant("Adam") ? "yes" : "no") << "\n";
    cout << "- Has Eve: " << (room.has_participant("Eve") ? "yes" : "no") << "\n";
    cout << "- Has Unknown: " << (room.has_participant("Unknown") ? "yes" : "no") << "\n";
}

// Example 6: Chat history
void example_chat_history()
{
    cout << "\n=== Example 6: Chat History ===\n";

    ChatRoom room;

    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.say("Message 1");
    bob.say("Message 2");
    alice.say("Message 3");
    bob.private_message("Alice", "Private for Alice");

    cout << "\n=== Alice's Chat Log ===\n";
    for (const auto &msg : alice.get_chat_log())
    {
        cout << "  " << msg << "\n";
    }

    cout << "\n=== Bob's Chat Log ===\n";
    for (const auto &msg : bob.get_chat_log())
    {
        cout << "  " << msg << "\n";
    }
}

// Example 7: Multiple independent chat rooms
void example_multiple_rooms()
{
    cout << "\n=== Example 7: Multiple Independent Chat Rooms ===\n";

    ChatRoom room1;
    ChatRoom room2;

    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};
    Person diana{"Diana"};

    cout << "\n--- Room 1: Alice and Bob ---\n";
    room1.join(&alice);
    room1.join(&bob);
    alice.say("Hello from Room 1");

    cout << "\n--- Room 2: Charlie and Diana ---\n";
    room2.join(&charlie);
    room2.join(&diana);
    charlie.say("Hello from Room 2");

    cout << "\n--- Room 1 activity ---\n";
    bob.say("This is still Room 1");

    cout << "\n--- Room 2 activity ---\n";
    diana.say("This is still Room 2");
}

// Example 8: Complex conversation
void example_complex_conversation()
{
    cout << "\n=== Example 8: Complex Conversation ===\n";

    ChatRoom office;

    Person manager{"Manager"};
    Person dev1{"Developer1"};
    Person dev2{"Developer2"};
    Person qa{"QA"};

    office.join(&manager);
    office.join(&dev1);
    office.join(&dev2);
    office.join(&qa);

    cout << "\n--- Manager addresses everyone ---\n";
    manager.say("Team, let's discuss the latest build");

    cout << "\n--- Dev1 responds ---\n";
    dev1.say("I finished the new feature");

    cout << "\n--- Dev2 responds ---\n";
    dev2.say("I'm working on the bug fixes");

    cout << "\n--- Manager sends private message to QA ---\n";
    manager.private_message("QA", "Can you test dev1's feature?");

    cout << "\n--- QA responds to Manager ---\n";
    qa.private_message("Manager", "Will do, need 2 hours");

    cout << "\n--- Dev1 broadcasts completion ---\n";
    dev1.say("Feature is ready for testing!");
}

int main()
{
    cout << "========================================\n";
    cout << "  MEDIATOR PATTERN EXAMPLES\n";
    cout << "  Chat Room Communication\n";
    cout << "========================================\n";

    example_basic_chat();
    example_multi_person_chat();
    example_private_messages();
    example_join_and_leave();
    example_chat_stats();
    example_chat_history();
    example_multiple_rooms();
    example_complex_conversation();

    cout << "\n========================================\n";

    return 0;
}
