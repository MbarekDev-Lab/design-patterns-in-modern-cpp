#include <iostream>
#include <iomanip>

using namespace std;

#include "behavioral/memento.h"

using namespace memento_patterns;

// ============================================================================
// EXAMPLE 1: Violation - No Undo/Redo
// ============================================================================

void example1_violation()
{
    cout << "\n=== EXAMPLE 1: VIOLATION - No Undo/Redo ===\n";

    using namespace violation;

    cout << "\nText editor without undo/redo:\n";

    TextEditorNoUndo editor;
    editor.append("Hello");
    cout << "After append: " << editor.get_text() << "\n";

    editor.append(" World");
    cout << "After append: " << editor.get_text() << "\n";

    editor.delete_text(0, 4);
    cout << "After delete: " << editor.get_text() << "\n";

    cout << "\nProblem: No way to undo! Changes are permanent.\n";
    cout << "User must manually manage undo/redo if needed.\n";
}

// ============================================================================
// EXAMPLE 2: Simple Text Editor with Undo/Redo
// ============================================================================

void example2_text_editor_undo_redo()
{
    cout << "\n=== EXAMPLE 2: Text Editor with Undo/Redo ===\n";

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    cout << "\nBuilding text:\n";

    editor.append("The quick");
    history.save_state();
    cout << "1. " << editor.get_text() << "\n";

    editor.append(" brown");
    history.save_state();
    cout << "2. " << editor.get_text() << "\n";

    editor.append(" fox");
    history.save_state();
    cout << "3. " << editor.get_text() << "\n";

    editor.append(" jumps");
    history.save_state();
    cout << "4. " << editor.get_text() << "\n";

    cout << "\nUndoing changes:\n";

    if (history.can_undo())
    {
        history.undo();
        cout << "After undo 1: " << editor.get_text() << "\n";
    }

    if (history.can_undo())
    {
        history.undo();
        cout << "After undo 2: " << editor.get_text() << "\n";
    }

    cout << "\nRedoing changes:\n";

    if (history.can_redo())
    {
        history.redo();
        cout << "After redo 1: " << editor.get_text() << "\n";
    }

    cout << "\nBenefit: Full encapsulation of undo/redo without exposing internals\n";
}

// ============================================================================
// EXAMPLE 3: Formatted Text with Range-Based Memento
// ============================================================================

void example3_formatted_text()
{
    cout << "\n=== EXAMPLE 3: Formatted Text with Undo/Redo ===\n";

    using namespace formatted_memento;

    FormattedText text("Welcome to the design patterns");
    FormattingHistory history(text);

    cout << "\nOriginal: " << text.get_text() << "\n";
    cout << "Rendered: " << text.render() << "\n";

    cout << "\nApplying formatting:\n";

    text.get_range(0, 6).capitalize = true; // "Welcome"
    history.save();
    cout << "After capitalize 'Welcome': " << text.render() << "\n";

    text.get_range(11, 14).capitalize = true; // "the"
    history.save();
    cout << "After capitalize 'the': " << text.render() << "\n";

    text.get_range(16, 22).bold = true; // "design"
    history.save();
    cout << "After bold 'design': " << text.render() << " (marked bold)\n";

    cout << "\nUndoing formatting:\n";

    if (history.can_undo())
    {
        history.undo();
        cout << "Undo 1: " << text.render() << "\n";
    }

    if (history.can_undo())
    {
        history.undo();
        cout << "Undo 2: " << text.render() << "\n";
    }

    cout << "\nRedoing formatting:\n";

    if (history.can_redo())
    {
        history.redo();
        cout << "Redo 1: " << text.render() << "\n";
    }
}

// ============================================================================
// EXAMPLE 4: Game Save Points & Checkpoints
// ============================================================================

void example4_game_checkpoints()
{
    cout << "\n=== EXAMPLE 4: Game with Checkpoints ===\n";

    using namespace game_memento;

    GameSession session;
    GameCharacter &player = session.get_character();

    cout << "\nStarting game:\n";
    cout << "Level: " << player.get_level() << ", Health: " << player.get_health()
         << ", Score: " << player.get_score() << "\n";

    session.create_checkpoint();
    cout << "Checkpoint 1 created\n";

    cout << "\nPlaying game:\n";
    player.move(10, 20);
    player.collect_item(100);
    player.level_up();
    cout << "After playing: Level " << player.get_level()
         << ", Score " << player.get_score()
         << ", Pos (" << player.get_x() << ", " << player.get_y() << ")\n";

    session.create_checkpoint();
    cout << "Checkpoint 2 created\n";

    cout << "\nContinuing game:\n";
    player.take_damage(30);
    player.fire();
    player.fire();
    cout << "After damage: Health " << player.get_health()
         << ", Ammo " << player.get_ammo() << "\n";

    session.create_checkpoint();
    cout << "Checkpoint 3 created\n";

    cout << "\nLoading checkpoint 2 (losing recent progress):\n";
    session.load_checkpoint(1);
    cout << "Level: " << player.get_level() << ", Health: " << player.get_health()
         << ", Score: " << player.get_score() << ", Ammo: " << player.get_ammo()
         << ", Pos (" << player.get_x() << ", " << player.get_y() << ")\n";

    cout << "\nLoading checkpoint 1 (far rollback):\n";
    session.load_checkpoint(0);
    cout << "Level: " << player.get_level() << ", Health: " << player.get_health()
         << ", Score: " << player.get_score() << ", Pos (" << player.get_x()
         << ", " << player.get_y() << ")\n";
}

// ============================================================================
// EXAMPLE 5: Application Configuration Snapshots
// ============================================================================

void example5_config_snapshots()
{
    cout << "\n=== EXAMPLE 5: Application Configuration Snapshots ===\n";

    using namespace config_memento;

    Application app;
    ConfigurationManager config_mgr(app);

    cout << "\nDefault config:\n";
    cout << app.to_string_debug() << "\n";

    cout << "\nModifying settings:\n";
    app.set_theme("dark");
    app.set_dark_mode(true);
    config_mgr.save_state();
    cout << "After change 1: " << app.to_string_debug() << "\n";

    app.set_font_size(14);
    config_mgr.save_state();
    cout << "After change 2: " << app.to_string_debug() << "\n";

    app.set_volume(75);
    config_mgr.save_state();
    cout << "After change 3: " << app.to_string_debug() << "\n";

    cout << "\nUndoing configuration changes:\n";

    if (config_mgr.can_undo())
    {
        config_mgr.undo();
        cout << "After undo 1: " << app.to_string_debug() << "\n";
    }

    if (config_mgr.can_undo())
    {
        config_mgr.undo();
        cout << "After undo 2: " << app.to_string_debug() << "\n";
    }

    if (config_mgr.can_undo())
    {
        config_mgr.undo();
        cout << "After undo 3: " << app.to_string_debug() << "\n";
    }
}

// ============================================================================
// EXAMPLE 6: Database Transaction with Rollback
// ============================================================================

void example6_database_transaction()
{
    cout << "\n=== EXAMPLE 6: Database Transaction with Rollback ===\n";

    using namespace transaction_memento;

    Database db;
    Transaction tx(db);

    cout << "\nInitial database:\n";
    cout << "Records: " << db.record_count() << "\n";

    cout << "\nPerforming transactions:\n";

    db.insert(DatabaseRecord(1, "Alice", "User data 1"));
    db.insert(DatabaseRecord(2, "Bob", "User data 2"));
    cout << "After insert: " << db.record_count() << " records\n";

    tx.create_savepoint();
    cout << "Savepoint 1 created\n";

    db.insert(DatabaseRecord(3, "Charlie", "User data 3"));
    cout << "After insert: " << db.record_count() << " records\n";

    tx.create_savepoint();
    cout << "Savepoint 2 created\n";

    db.update(1, "Updated data for Alice");
    db.delete_record(2);
    cout << "After update/delete: " << db.record_count() << " records\n";

    cout << "\nRolling back to savepoint 2:\n";
    tx.rollback();
    cout << "After rollback: " << db.record_count() << " records\n";

    cout << "\nRolling back to savepoint 1:\n";
    tx.rollback();
    cout << "After rollback: " << db.record_count() << " records\n";
}

// ============================================================================
// EXAMPLE 7: Multi-Step Undo/Redo
// ============================================================================

void example7_multi_step_history()
{
    cout << "\n=== EXAMPLE 7: Multi-Step Undo/Redo ===\n";

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    cout << "\nBuilding a sentence step by step:\n";

    vector<string> states = {"The", "The quick", "The quick brown",
                             "The quick brown fox", "The quick brown fox jumps",
                             "The quick brown fox jumps over"};

    for (size_t i = 0; i < states.size(); ++i)
    {
        if (i > 0)
        {
            editor.append(" " + states[i].substr(states[i - 1].length() + 1));
        }
        else
        {
            editor.append(states[i]);
        }
        history.save_state();
        cout << i + 1 << ". " << editor.get_text() << "\n";
    }

    cout << "\nStep-by-step undo:\n";
    int step = 1;
    while (history.can_undo())
    {
        history.undo();
        cout << step++ << ". " << editor.get_text() << "\n";
    }

    cout << "\nStep-by-step redo:\n";
    step = 1;
    while (history.can_redo())
    {
        history.redo();
        cout << step++ << ". " << editor.get_text() << "\n";
    }
}

// ============================================================================
// EXAMPLE 8: Comparison - With vs Without Memento
// ============================================================================

void example8_comparison()
{
    cout << "\n=== EXAMPLE 8: With vs Without Memento ===\n";

    cout << "\nWITHOUT MEMENTO:\n";
    cout << "  - Client must manually track all state changes\n";
    cout << "  - State management is scattered throughout code\n";
    cout << "  - No encapsulation of undo/redo logic\n";
    cout << "  - Easy to forget to save state\n";
    cout << "  - Tight coupling between object and history\n";

    cout << "\nWITH MEMENTO:\n";
    cout << "  - Originator handles state snapshot creation\n";
    cout << "  - Caretaker manages history stack\n";
    cout << "  - Clean separation of concerns\n";
    cout << "  - Automatic state management\n";
    cout << "  - Works with any object without modification\n";

    cout << "\nMEMENTO CHARACTERISTICS:\n";
    cout << "  - Immutable snapshots preserve exact state\n";
    cout << "  - Full state captured (not just differences)\n";
    cout << "  - Memory trade-off for functionality\n";
    cout << "  - Thread-safe if handled correctly\n";
    cout << "  - Works across sessions (save/load)\n";
}

// ============================================================================
// EXAMPLE 9: Progressive Disclosure
// ============================================================================

void example9_progressive_disclosure()
{
    cout << "\n=== EXAMPLE 9: Progressive Disclosure - Memento Usage ===\n";

    using namespace simple_memento;

    cout << "\nBEGINNER: Basic undo/redo\n";

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("Hello");
    history.save_state();

    editor.append(" World");
    history.save_state();

    cout << "Text: " << editor.get_text() << "\n";

    history.undo();
    cout << "After undo: " << editor.get_text() << "\n";

    cout << "\nADVANCED: Custom caretaker with multiple undo histories\n";

    using namespace formatted_memento;

    FormattedText text("Design Patterns");
    FormattingHistory fmt_history(text);

    text.get_range(0, 5).capitalize = true;
    fmt_history.save();

    text.get_range(7, 14).bold = true;
    fmt_history.save();

    cout << "Formatting applied\n";
    cout << "Can undo: " << (fmt_history.can_undo() ? "YES" : "NO") << "\n";
    cout << "Can redo: " << (fmt_history.can_redo() ? "YES" : "NO") << "\n";

    fmt_history.undo();
    fmt_history.undo();

    cout << "After undo all: " << text.render() << "\n";
    cout << "Can undo: " << (fmt_history.can_undo() ? "YES" : "NO") << "\n";
    cout << "Can redo: " << (fmt_history.can_redo() ? "YES" : "NO") << "\n";
}

// ============================================================================

int main()
{
    cout << string(70, '=') << "\n";
    cout << "MEMENTO PATTERN - COMPREHENSIVE EXAMPLES\n";
    cout << string(70, '=') << "\n";

    example1_violation();
    example2_text_editor_undo_redo();
    example3_formatted_text();
    example4_game_checkpoints();
    example5_config_snapshots();
    example6_database_transaction();
    example7_multi_step_history();
    example8_comparison();
    example9_progressive_disclosure();

    cout << "\n"
         << string(70, '=') << "\n";
    cout << "MEMENTO PATTERN - KEY TAKEAWAYS\n";
    cout << string(70, '=') << "\n";

    cout << "\n1. USE CASES:\n";
    cout << "   - Undo/redo functionality\n";
    cout << "   - Save game checkpoints\n";
    cout << "   - Database transaction rollback\n";
    cout << "   - Configuration snapshots\n";
    cout << "   - Collaborative editing (conflict resolution)\n";

    cout << "\n2. COMPONENTS:\n";
    cout << "   - Originator: Creates and restores mementos\n";
    cout << "   - Memento: Immutable state snapshot\n";
    cout << "   - Caretaker: Manages history (undo/redo stacks)\n";

    cout << "\n3. BENEFITS:\n";
    cout << "   - Preserves encapsulation\n";
    cout << "   - Clean separation of concerns\n";
    cout << "   - Automatic state versioning\n";
    cout << "   - Works with any object type\n";

    cout << "\n4. TRADE-OFFS:\n";
    cout << "   - Memory overhead from snapshots\n";
    cout << "   - Large objects = large mementos\n";
    cout << "   - Memento lifecycle management\n";
    cout << "   - Serialization complexity\n";

    cout << "\n5. REAL-WORLD:\n";
    cout << "   - Text editors (Word, VS Code, Vim)\n";
    cout << "   - Game engines (save points, checkpoints)\n";
    cout << "   - Database systems (transactions, rollback)\n";
    cout << "   - Photo editors (history panel)\n";
    cout << "   - IDE debuggers (state snapshots)\n";

    cout << "\n";
    return 0;
}
