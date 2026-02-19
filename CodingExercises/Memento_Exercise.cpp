#include <iostream>
#include <cassert>

using namespace std;

#include "behavioral/memento.h"

using namespace memento_patterns;

// ============================================================================
// CUSTOM ASSERTION FRAMEWORK
// ============================================================================

int assertion_count = 0;
int assertion_failures = 0;

#define ASSERT_TRUE(condition)                            \
    do                                                    \
    {                                                     \
        assertion_count++;                                \
        if (!(condition))                                 \
        {                                                 \
            cout << "  ✗ FAILED: " << #condition << "\n"; \
            assertion_failures++;                         \
        }                                                 \
        else                                              \
        {                                                 \
            cout << "  ✓";                                \
        }                                                 \
    } while (0)

#define ASSERT_FALSE(condition) ASSERT_TRUE(!(condition))

#define ASSERT_EQ(a, b)                                    \
    do                                                     \
    {                                                      \
        assertion_count++;                                 \
        if ((a) != (b))                                    \
        {                                                  \
            cout << "  ✗ FAILED: " << #a << " == " << #b   \
                 << " (" << (a) << " != " << (b) << ")\n"; \
            assertion_failures++;                          \
        }                                                  \
        else                                               \
        {                                                  \
            cout << "  ✓";                                 \
        }                                                  \
    } while (0)

#define TEST_FUNCTION(name) \
    cout << "\n"            \
         << name << "\n";   \
    assertion_count_before = assertion_count;

int assertion_count_before = 0;

// ============================================================================
// TEST SUITE
// ============================================================================

void test_simple_text_editor_append()
{
    TEST_FUNCTION("Test: Text Editor - Append");

    using namespace simple_memento;

    TextEditor editor;
    editor.append("Hello");

    ASSERT_EQ(editor.get_text(), "Hello");

    editor.append(" World");
    ASSERT_EQ(editor.get_text(), "Hello World");
}

void test_text_editor_delete()
{
    TEST_FUNCTION("Test: Text Editor - Delete");

    using namespace simple_memento;

    TextEditor editor;
    editor.append("Hello World");
    editor.delete_text(0, 4); // Delete "Hello"

    ASSERT_EQ(editor.get_text(), " World");
}

void test_text_memento_save()
{
    TEST_FUNCTION("Test: Text Memento - Save");

    using namespace simple_memento;

    TextEditor editor;
    editor.append("Test state");

    TextMemento memento = editor.save();

    ASSERT_EQ(memento.get_state(), "Test state");
}

void test_text_memento_restore()
{
    TEST_FUNCTION("Test: Text Memento - Restore");

    using namespace simple_memento;

    TextEditor editor;
    editor.append("Original");
    TextMemento memento = editor.save();

    editor.append(" Modified");
    ASSERT_EQ(editor.get_text(), "Original Modified");

    editor.restore(memento);
    ASSERT_EQ(editor.get_text(), "Original");
}

void test_undo_redo_system_basic()
{
    TEST_FUNCTION("Test: Undo/Redo - Basic");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("A");
    history.save_state();

    editor.append("B");
    history.save_state();

    ASSERT_EQ(editor.get_text(), "AB");
    ASSERT_TRUE(history.can_undo());
}

void test_undo_single_step()
{
    TEST_FUNCTION("Test: Undo - Single Step");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("Hello");
    history.save_state();

    editor.append(" World");
    history.save_state();

    history.undo();
    ASSERT_EQ(editor.get_text(), "Hello");
}

void test_undo_multiple_steps()
{
    TEST_FUNCTION("Test: Undo - Multiple Steps");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("A");
    history.save_state();

    editor.append("B");
    history.save_state();

    editor.append("C");
    history.save_state();

    history.undo();
    ASSERT_EQ(editor.get_text(), "AB");

    history.undo();
    ASSERT_EQ(editor.get_text(), "A");
}

void test_redo_functionality()
{
    TEST_FUNCTION("Test: Redo - Functionality");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("First");
    history.save_state();

    editor.append(" Second");
    history.save_state();

    history.undo();
    ASSERT_EQ(editor.get_text(), "First");
    ASSERT_TRUE(history.can_redo());

    history.redo();
    ASSERT_EQ(editor.get_text(), "First Second");
}

void test_redo_clears_on_new_action()
{
    TEST_FUNCTION("Test: Redo - Clears on New Action");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("A");
    history.save_state();

    editor.append("B");
    history.save_state();

    history.undo();
    ASSERT_TRUE(history.can_redo());

    editor.append("C");
    history.save_state();

    ASSERT_FALSE(history.can_redo()); // Redo stack cleared
}

void test_formatted_text_range()
{
    TEST_FUNCTION("Test: Formatted Text - Range");

    using namespace formatted_memento;

    FormattedText text("Hello");
    TextRange &range = text.get_range(0, 4);

    range.capitalize = true;
    ASSERT_EQ(range.start, 0);
    ASSERT_EQ(range.end, 4);
    ASSERT_TRUE(range.capitalize);
}

void test_formatted_text_render()
{
    TEST_FUNCTION("Test: Formatted Text - Render");

    using namespace formatted_memento;

    FormattedText text("Hello World");
    text.get_range(0, 4).capitalize = true;

    string rendered = text.render();
    ASSERT_EQ(rendered[0], 'H');
}

void test_formatted_text_memento()
{
    TEST_FUNCTION("Test: Formatted Text - Memento");

    using namespace formatted_memento;

    FormattedText text("Test");
    text.get_range(0, 3).capitalize = true;

    FormattedTextMemento memento = text.save();

    text.clear();
    ASSERT_EQ(text.get_formatting().size(), 0);

    text.restore(memento);
    ASSERT_EQ(text.get_formatting().size(), 1);
}

void test_game_character_basic()
{
    TEST_FUNCTION("Test: Game Character - Basic");

    using namespace game_memento;

    GameCharacter player;

    ASSERT_EQ(player.get_health(), 100);
    ASSERT_EQ(player.get_score(), 0);
    ASSERT_EQ(player.get_level(), 1);
}

void test_game_character_actions()
{
    TEST_FUNCTION("Test: Game Character - Actions");

    using namespace game_memento;

    GameCharacter player;

    player.take_damage(20);
    ASSERT_EQ(player.get_health(), 80);

    player.collect_item(50);
    ASSERT_EQ(player.get_score(), 50);

    player.fire();
    ASSERT_EQ(player.get_ammo(), 29);
}

void test_game_checkpoint_save()
{
    TEST_FUNCTION("Test: Game Checkpoint - Save");

    using namespace game_memento;

    GameCharacter player;
    player.move(10, 20);
    player.collect_item(100);

    GameStateMemento checkpoint = player.save_checkpoint();

    ASSERT_EQ(checkpoint.x, 10);
    ASSERT_EQ(checkpoint.y, 20);
    ASSERT_EQ(checkpoint.score, 100);
}

void test_game_checkpoint_load()
{
    TEST_FUNCTION("Test: Game Checkpoint - Load");

    using namespace game_memento;

    GameCharacter player;
    player.move(10, 20);
    player.collect_item(100);
    GameStateMemento checkpoint = player.save_checkpoint();

    player.move(50, 50);
    player.take_damage(30);

    player.load_checkpoint(checkpoint);

    ASSERT_EQ(player.get_x(), 10);
    ASSERT_EQ(player.get_y(), 20);
    ASSERT_EQ(player.get_score(), 100);
    ASSERT_EQ(player.get_health(), 100); // Restored
}

void test_game_session_checkpoints()
{
    TEST_FUNCTION("Test: Game Session - Checkpoints");

    using namespace game_memento;

    GameSession session;
    GameCharacter &player = session.get_character();

    ASSERT_EQ(session.checkpoint_count(), 1); // Initial checkpoint

    player.collect_item(50);
    player.level_up();
    session.create_checkpoint();

    ASSERT_EQ(session.checkpoint_count(), 2);

    player.move(10, 10);
    session.create_checkpoint();

    ASSERT_EQ(session.checkpoint_count(), 3);
}

void test_config_application_settings()
{
    TEST_FUNCTION("Test: Config - Application Settings");

    using namespace config_memento;

    Application app;

    app.set_theme("dark");
    app.set_font_size(14);
    app.set_dark_mode(true);

    const auto &config = app.get_config();
    ASSERT_EQ(config.theme, "dark");
    ASSERT_EQ(config.font_size, 14);
    ASSERT_TRUE(config.dark_mode);
}

void test_config_save_restore()
{
    TEST_FUNCTION("Test: Config - Save and Restore");

    using namespace config_memento;

    Application app;
    app.set_theme("dark");
    app.set_font_size(14);

    ApplicationConfig saved = app.save_config();

    app.set_theme("light");
    app.set_font_size(12);

    app.restore_config(saved);

    ASSERT_EQ(app.get_config().theme, "dark");
    ASSERT_EQ(app.get_config().font_size, 14);
}

void test_config_manager_undo()
{
    TEST_FUNCTION("Test: Config Manager - Undo");

    using namespace config_memento;

    Application app;
    ConfigurationManager mgr(app);

    app.set_theme("dark");
    mgr.save_state();

    app.set_font_size(16);
    mgr.save_state();

    ASSERT_EQ(app.get_config().font_size, 16);
    ASSERT_TRUE(mgr.can_undo());

    mgr.undo();
    ASSERT_EQ(app.get_config().font_size, 12); // Default
}

void test_database_basic_operations()
{
    TEST_FUNCTION("Test: Database - Basic Operations");

    using namespace transaction_memento;

    Database db;

    db.insert(DatabaseRecord(1, "Alice", "Data"));
    ASSERT_EQ(db.record_count(), 1);

    db.insert(DatabaseRecord(2, "Bob", "Data"));
    ASSERT_EQ(db.record_count(), 2);
}

void test_database_update()
{
    TEST_FUNCTION("Test: Database - Update");

    using namespace transaction_memento;

    Database db;
    db.insert(DatabaseRecord(1, "Alice", "Original"));

    db.update(1, "Updated");

    const auto &records = db.get_records();
    ASSERT_EQ(records[0].data, "Updated");
}

void test_database_delete()
{
    TEST_FUNCTION("Test: Database - Delete");

    using namespace transaction_memento;

    Database db;
    db.insert(DatabaseRecord(1, "Alice", "Data"));
    db.insert(DatabaseRecord(2, "Bob", "Data"));

    ASSERT_EQ(db.record_count(), 2);

    db.delete_record(1);
    ASSERT_EQ(db.record_count(), 1);
}

void test_database_transaction_begin()
{
    TEST_FUNCTION("Test: Database Transaction - Begin");

    using namespace transaction_memento;

    Database db;
    db.insert(DatabaseRecord(1, "Alice", "Data"));

    Transaction tx(db);
    ASSERT_TRUE(tx.has_savepoint());

    db.insert(DatabaseRecord(2, "Bob", "Data"));
    tx.create_savepoint();

    ASSERT_EQ(db.record_count(), 2);
}

void test_database_transaction_rollback()
{
    TEST_FUNCTION("Test: Database Transaction - Rollback");

    using namespace transaction_memento;

    Database db;
    db.insert(DatabaseRecord(1, "Alice", "Data"));

    Transaction tx(db);
    tx.create_savepoint();

    db.insert(DatabaseRecord(2, "Bob", "Data"));
    db.insert(DatabaseRecord(3, "Charlie", "Data"));

    ASSERT_EQ(db.record_count(), 3);

    tx.rollback();
    ASSERT_EQ(db.record_count(), 1);
}

void test_memento_immutability()
{
    TEST_FUNCTION("Test: Memento - Immutability");

    using namespace simple_memento;

    TextEditor editor;
    editor.append("State");

    TextMemento memento1 = editor.save();
    string state1 = memento1.get_state();

    editor.append(" Modified");
    editor.restore(memento1);

    string state2 = memento1.get_state();

    // Memento state unchanged
    ASSERT_EQ(state1, state2);
    ASSERT_EQ(state2, "State");
}

void test_memento_independence()
{
    TEST_FUNCTION("Test: Memento - Independence");

    using namespace simple_memento;

    TextEditor editor1, editor2;
    editor1.append("Editor1");
    editor2.append("Editor2");

    TextMemento mem1 = editor1.save();
    TextMemento mem2 = editor2.save();

    ASSERT_EQ(mem1.get_state(), "Editor1");
    ASSERT_EQ(mem2.get_state(), "Editor2");
}

void test_undo_redo_bidirectional()
{
    TEST_FUNCTION("Test: Undo/Redo - Bidirectional");

    using namespace simple_memento;

    TextEditor editor;
    UndoRedoSystem history(editor);

    editor.append("Step1");
    history.save_state();
    editor.append("Step2");
    history.save_state();
    editor.append("Step3");
    history.save_state();

    // Undo twice
    history.undo();
    history.undo();
    ASSERT_EQ(editor.get_text(), "Step1");

    // Redo once
    history.redo();
    ASSERT_EQ(editor.get_text(), "Step1Step2");

    // Undo once
    history.undo();
    ASSERT_EQ(editor.get_text(), "Step1");
}

// ============================================================================
// TEST RUNNER AND SUMMARY
// ============================================================================

int main()
{
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║          MEMENTO PATTERN - TEST SUITE                     ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\nRunning tests...\n";

    // Simple text editor tests
    test_simple_text_editor_append();
    test_text_editor_delete();
    test_text_memento_save();
    test_text_memento_restore();
    test_undo_redo_system_basic();
    test_undo_single_step();
    test_undo_multiple_steps();
    test_redo_functionality();
    test_redo_clears_on_new_action();
    test_undo_redo_bidirectional();

    // Formatted text tests
    test_formatted_text_range();
    test_formatted_text_render();
    test_formatted_text_memento();

    // Game character tests
    test_game_character_basic();
    test_game_character_actions();
    test_game_checkpoint_save();
    test_game_checkpoint_load();
    test_game_session_checkpoints();

    // Configuration tests
    test_config_application_settings();
    test_config_save_restore();
    test_config_manager_undo();

    // Database tests
    test_database_basic_operations();
    test_database_update();
    test_database_delete();
    test_database_transaction_begin();
    test_database_transaction_rollback();

    // General memento tests
    test_memento_immutability();
    test_memento_independence();

    // Summary
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "TEST SUMMARY\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "┌───────────────────────────────────────────────────────────┐\n";
    cout << "│ Results                                                   │\n";
    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "│ Passed:       " << (assertion_count - assertion_failures) << " ✓\n";
    cout << "│ Failed:       " << assertion_failures << " ✗\n";

    if (assertion_failures == 0)
    {
        cout << "│\n│ Status: ALL TESTS PASSED ✓\n";
    }
    else
    {
        cout << "│\n│ Status: SOME TESTS FAILED ✗\n";
    }

    cout << "└───────────────────────────────────────────────────────────┘\n";

    cout << "\nKEY LEARNING POINTS\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "1. CORE COMPONENTS:\n";
    cout << "   - Originator: Object whose state is captured\n";
    cout << "   - Memento: Immutable snapshot of state\n";
    cout << "   - Caretaker: Manages history (undo/redo stack)\n\n";

    cout << "2. STATE MANAGEMENT:\n";
    cout << "   - Full snapshots preserve exact state\n";
    cout << "   - Encapsulation maintained (no external access)\n";
    cout << "   - Independent memento objects\n\n";

    cout << "3. UNDO/REDO PATTERN:\n";
    cout << "   - Undo stack holds previous states\n";
    cout << "   - Redo stack holds undone states\n";
    cout << "   - New action clears redo stack\n\n";

    cout << "4. REAL-WORLD APPLICATIONS:\n";
    cout << "   - Text editors (undo/redo)\n";
    cout << "   - Game engines (checkpoints)\n";
    cout << "   - Database systems (transactions)\n";
    cout << "   - Configuration managers\n";
    cout << "   - Photo editors (history)\n\n";

    return assertion_failures == 0 ? 0 : 1;
}
