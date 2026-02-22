#include <iostream>
#include <cassert>
#include <string>
#include "../../include/behavioral/mediator.h"

using namespace std;

// ============================================================================
// MEDIATOR PATTERN TEST SUITE
// ============================================================================

void test_person_creation()
{
    cout << "Test: Person creation... ";

    Person p{"Alice"};
    assert(p.name == "Alice");
    assert(!p.is_in_room());
    assert(p.chat_log.empty());

    cout << "PASS\n";
}

void test_chatroom_creation()
{
    cout << "Test: ChatRoom creation... ";

    ChatRoom room;
    assert(room.participant_count() == 0);
    assert(!room.has_participant("Anyone"));

    cout << "PASS\n";
}

void test_person_join_room()
{
    cout << "Test: Person joins room... ";

    ChatRoom room;
    Person alice{"Alice"};

    room.join(&alice);

    assert(alice.is_in_room());
    assert(room.participant_count() == 1);
    assert(room.has_participant("Alice"));

    cout << "PASS\n";
}

void test_multiple_people_join()
{
    cout << "Test: Multiple people join room... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    room.join(&bob);
    room.join(&charlie);

    assert(room.participant_count() == 3);
    assert(room.has_participant("Alice"));
    assert(room.has_participant("Bob"));
    assert(room.has_participant("Charlie"));

    cout << "PASS\n";
}

void test_broadcast_message()
{
    cout << "Test: Broadcast message... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.say("Hello");

    // Bob should receive the message
    assert(bob.chat_log.size() == 1);
    assert(bob.chat_log[0] == "Alice: \"Hello\"");

    // Alice should have Bob's join message but not her own Hello
    assert(alice.chat_log.size() == 1);
    assert(alice.chat_log[0].find("Bob") != string::npos); // "room: Bob joins the chat"

    cout << "PASS\n";
}

void test_multiple_broadcasts()
{
    cout << "Test: Multiple broadcasts... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.say("Message 1");
    bob.say("Message 2");
    alice.say("Message 3");

    // Alice should have join + Bob's message (but not her own)
    assert(alice.chat_log.size() == 2);
    assert(bob.chat_log.size() == 2);

    cout << "PASS\n";
}

void test_private_message()
{
    cout << "Test: Private message... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    room.join(&bob);
    room.join(&charlie);

    alice.private_message("Bob", "Hi Bob");

    // Bob should receive Charlie's join message and the private message
    assert(bob.chat_log.size() == 2);
    assert(bob.chat_log[0].find("Charlie") != string::npos);
    assert(bob.chat_log[1] == "Alice: \"Hi Bob\"");
    // Charlie should not receive the private message
    assert(charlie.chat_log.size() == 0);

    cout << "PASS\n";
}

void test_private_message_to_nonexistent()
{
    cout << "Test: Private message to non-existent person... ";

    ChatRoom room;
    Person alice{"Alice"};

    room.join(&alice);

    // Should not crash, just be ignored
    alice.private_message("NonExistent", "Hello");

    cout << "PASS\n";
}

void test_broadcast_with_joining()
{
    cout << "Test: Broadcast on join... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    assert(alice.chat_log.size() == 0); // No messages yet

    room.join(&bob);
    // Alice should see the join message
    assert(alice.chat_log.size() == 1);
    assert(alice.chat_log[0].find("Bob") != string::npos);

    cout << "PASS\n";
}

void test_person_leave_room()
{
    cout << "Test: Person leaves room... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    assert(room.participant_count() == 2);

    room.leave(&alice);

    assert(room.participant_count() == 1);
    assert(!room.has_participant("Alice"));
    assert(room.has_participant("Bob"));
    assert(!alice.is_in_room());

    cout << "PASS\n";
}

void test_broadcast_after_leave()
{
    cout << "Test: Broadcast after someone leaves... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    room.join(&bob);
    room.join(&charlie);

    // Clear chat logs
    alice.chat_log.clear();
    bob.chat_log.clear();

    room.leave(&alice);

    bob.say("Alice is gone");

    // Charlie should receive the message
    assert(charlie.chat_log.back() == "Bob: \"Alice is gone\"");

    cout << "PASS\n";
}

void test_chat_log_format()
{
    cout << "Test: Chat log formatting... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.say("Test message");

    assert(bob.chat_log.size() == 1);
    assert(bob.chat_log[0] == "Alice: \"Test message\"");

    cout << "PASS\n";
}

void test_person_equality()
{
    cout << "Test: Person equality operators... ";

    Person alice1{"Alice"};
    Person alice2{"Alice"};
    Person bob{"Bob"};

    assert(alice1 == alice2);
    assert(alice1 != bob);
    assert(!(alice1 == bob));

    cout << "PASS\n";
}

void test_multiple_rooms_independent()
{
    cout << "Test: Multiple independent rooms... ";

    ChatRoom room1;
    ChatRoom room2;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room1.join(&alice);
    room2.join(&bob);

    alice.say("Message in room 1");

    // Bob should not receive Alice's message
    assert(bob.chat_log.size() == 0);

    cout << "PASS\n";
}

void test_get_chat_log()
{
    cout << "Test: Get chat log... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    bob.say("Message 1");
    bob.say("Message 2");

    auto log = alice.get_chat_log();
    assert(log.size() == 3);
    assert(log[0].find("Bob") != string::npos); // "room: Bob joins the chat"
    assert(log[1] == "Bob: \"Message 1\"");
    assert(log[2] == "Bob: \"Message 2\"");

    cout << "PASS\n";
}

void test_room_participant_check()
{
    cout << "Test: Room participant check... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    assert(!room.has_participant("Alice"));
    assert(!room.has_participant("Bob"));

    room.join(&alice);
    assert(room.has_participant("Alice"));
    assert(!room.has_participant("Bob"));

    room.join(&bob);
    assert(room.has_participant("Alice"));
    assert(room.has_participant("Bob"));

    room.leave(&alice);
    assert(!room.has_participant("Alice"));
    assert(room.has_participant("Bob"));

    cout << "PASS\n";
}

void test_join_broadcast_message()
{
    cout << "Test: Join broadcast message content... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    auto msg = alice.chat_log[0];
    assert(msg.find("Bob") != string::npos);
    assert(msg.find("joins") != string::npos);

    cout << "PASS\n";
}

void test_leave_broadcast_message()
{
    cout << "Test: Leave broadcast message content... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};

    room.join(&alice);
    room.join(&bob);

    alice.chat_log.clear();
    room.leave(&bob);

    auto msg = alice.chat_log[0];
    assert(msg.find("Bob") != string::npos);
    assert(msg.find("leaves") != string::npos);

    cout << "PASS\n";
}

void test_three_person_broadcast()
{
    cout << "Test: Three-person broadcast... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    room.join(&bob);
    room.join(&charlie);

    alice.chat_log.clear();
    bob.chat_log.clear();
    charlie.chat_log.clear();

    alice.say("Hello everyone!");

    // Bob and Charlie should receive the message, Alice should not
    assert(alice.chat_log.size() == 0);
    assert(bob.chat_log.size() == 1);
    assert(charlie.chat_log.size() == 1);

    cout << "PASS\n";
}

void test_person_in_room_status()
{
    cout << "Test: Person in-room status... ";

    ChatRoom room;
    Person alice{"Alice"};

    assert(!alice.is_in_room());

    room.join(&alice);
    assert(alice.is_in_room());

    room.leave(&alice);
    assert(!alice.is_in_room());

    cout << "PASS\n";
}

void test_empty_room_broadcast()
{
    cout << "Test: Broadcast in empty room... ";

    ChatRoom room;
    Person alice{"Alice"};

    // Broadcasting in room with just one person should not crash
    room.join(&alice);
    alice.say("Talking to myself");

    assert(alice.chat_log.size() == 0); // No one else to receive it

    cout << "PASS\n";
}

void test_sequential_operations()
{
    cout << "Test: Sequential operations... ";

    ChatRoom room;
    Person alice{"Alice"};
    Person bob{"Bob"};
    Person charlie{"Charlie"};

    room.join(&alice);
    alice.say("First message");

    room.join(&bob);
    bob.say("Second message");

    room.join(&charlie);
    charlie.say("Third message");

    assert(room.participant_count() == 3);
    assert(alice.chat_log.size() >= 1);
    assert(bob.chat_log.size() >= 2);

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  MEDIATOR PATTERN TEST SUITE\n";
    cout << "========================================\n\n";

    test_person_creation();
    test_chatroom_creation();
    test_person_join_room();
    test_multiple_people_join();
    test_broadcast_message();
    test_multiple_broadcasts();
    test_private_message();
    test_private_message_to_nonexistent();
    test_broadcast_with_joining();
    test_person_leave_room();
    test_broadcast_after_leave();
    test_chat_log_format();
    test_person_equality();
    test_multiple_rooms_independent();
    test_get_chat_log();
    test_room_participant_check();
    test_join_broadcast_message();
    test_leave_broadcast_message();
    test_three_person_broadcast();
    test_person_in_room_status();
    test_empty_room_broadcast();
    test_sequential_operations();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (22 tests)\n";
    cout << "========================================\n";

    return 0;
}
