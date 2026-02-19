#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <stack>

using namespace std;

/*
 * MEMENTO PATTERN
 *
 * INTENT:
 * Capture and externalizes an object's internal state without violating
 * encapsulation, allowing the object to be restored to this state later.
 *
 * KEY CONCEPTS:
 * - Memento: Captures object state (immutable snapshot)
 * - Originator: Creates memento, restores from memento
 * - Caretaker: Stores mementos, manages undo/redo stack
 * - State preservation: Full state snapshot at moment of capture
 * - Time travel: Ability to revert to any previous state
 *
 * BENEFITS:
 * - Undo/redo functionality without exposing internal state
 * - Clean separation of concerns
 * - State history preservation
 * - Easy state rollback
 * - Works with any object type
 *
 * TRADE-OFFS:
 * - Memory overhead for storing states
 * - Large objects create large mementos
 * - Must manage memento lifecycle
 * - Threading complexity with mementos
 *
 * WHEN TO USE:
 * - Undo/redo functionality required
 * - State snapshots needed
 * - Transaction rollback
 * - Checkpointing systems
 * - Save game functionality
 * - Collaborative editing (conflict resolution)
 */

namespace memento_patterns
{

    // ============================================================================
    // VIOLATION: No Undo/Redo - Direct State Mutation
    // ============================================================================

    namespace violation
    {

        class TextEditorNoUndo
        {
        private:
            string text;

        public:
            TextEditorNoUndo() = default;

            void append(const string &s)
            {
                text += s;
            }

            void delete_text(int start, int end)
            {
                text.erase(start, end - start + 1);
            }

            void clear()
            {
                text.clear();
            }

            const string &get_text() const
            {
                return text;
            }

            // PROBLEM: No way to undo changes!
            // User must manually track changes or use external undo system
            // Violates encapsulation if client manages state history
        };

    } // namespace violation

    // ============================================================================
    // SOLUTION 1: Simple Text Editor with Memento
    // ============================================================================

    namespace simple_memento
    {

        // Memento: Captures state of text
        class TextMemento
        {
        private:
            string text;

        public:
            explicit TextMemento(const string &t) : text(t) {}

            string get_state() const
            {
                return text;
            }
        };

        // Originator: Text editor that creates mementos
        class TextEditor
        {
        private:
            string text;

        public:
            TextEditor() = default;

            void append(const string &s)
            {
                text += s;
            }

            void delete_text(int start, int end)
            {
                if (start >= 0 && end < (int)text.length())
                {
                    text.erase(start, end - start + 1);
                }
            }

            void clear()
            {
                text.clear();
            }

            const string &get_text() const
            {
                return text;
            }

            // Create memento capturing current state
            TextMemento save() const
            {
                return TextMemento(text);
            }

            // Restore from memento
            void restore(const TextMemento &memento)
            {
                text = memento.get_state();
            }
        };

        // Caretaker: Manages memento history
        class UndoRedoSystem
        {
        private:
            stack<TextMemento> undo_stack;
            stack<TextMemento> redo_stack;
            TextEditor &editor;

        public:
            explicit UndoRedoSystem(TextEditor &e) : editor(e)
            {
                // Save initial state
                undo_stack.push(editor.save());
            }

            void save_state()
            {
                undo_stack.push(editor.save());
                // Clear redo stack when new action taken
                while (!redo_stack.empty())
                {
                    redo_stack.pop();
                }
            }

            void undo()
            {
                if (undo_stack.size() > 1)
                {
                    redo_stack.push(undo_stack.top());
                    undo_stack.pop();
                    editor.restore(undo_stack.top());
                }
            }

            void redo()
            {
                if (!redo_stack.empty())
                {
                    undo_stack.push(redo_stack.top());
                    editor.restore(redo_stack.top());
                    redo_stack.pop();
                }
            }

            bool can_undo() const
            {
                return undo_stack.size() > 1;
            }

            bool can_redo() const
            {
                return !redo_stack.empty();
            }
        };

    } // namespace simple_memento

    // ============================================================================
    // SOLUTION 2: Formatted Text with Range-Based Memento
    // ============================================================================

    namespace formatted_memento
    {

        // Intrinsic state: represents a formatting range
        struct TextRange
        {
            int start, end;
            bool capitalize, bold, italic, underline;

            TextRange(int s, int e)
                : start(s), end(e), capitalize(false), bold(false),
                  italic(false), underline(false) {}

            bool covers(int position) const
            {
                return position >= start && position <= end;
            }
        };

        // Memento: Captures formatted text state
        class FormattedTextMemento
        {
        private:
            string text;
            vector<TextRange> formatting;

        public:
            FormattedTextMemento(const string &t, const vector<TextRange> &f)
                : text(t), formatting(f) {}

            const string &get_text() const { return text; }
            const vector<TextRange> &get_formatting() const { return formatting; }
        };

        // Originator: Formatted text with ranges
        class FormattedText
        {
        private:
            string plain_text;
            vector<TextRange> formatting;

        public:
            explicit FormattedText(const string &text) : plain_text(text) {}

            TextRange &get_range(int start, int end)
            {
                formatting.emplace_back(start, end);
                return formatting.back();
            }

            const vector<TextRange> &get_formatting() const
            {
                return formatting;
            }

            const string &get_text() const
            {
                return plain_text;
            }

            // Create memento
            FormattedTextMemento save() const
            {
                return FormattedTextMemento(plain_text, formatting);
            }

            // Restore from memento
            void restore(const FormattedTextMemento &memento)
            {
                plain_text = memento.get_text();
                formatting = memento.get_formatting();
            }

            string render() const
            {
                string result;
                for (size_t i = 0; i < plain_text.length(); i++)
                {
                    char c = plain_text[i];
                    for (const auto &rng : formatting)
                    {
                        if (rng.covers(i))
                        {
                            if (rng.capitalize)
                                c = toupper(c);
                            // Note: bold/italic/underline would need special rendering
                        }
                    }
                    result += c;
                }
                return result;
            }

            void clear()
            {
                formatting.clear();
            }
        };

        // Caretaker: Manages formatting history
        class FormattingHistory
        {
        private:
            stack<FormattedTextMemento> undo_stack;
            stack<FormattedTextMemento> redo_stack;
            FormattedText &text;

        public:
            explicit FormattingHistory(FormattedText &t) : text(t)
            {
                undo_stack.push(text.save());
            }

            void save()
            {
                undo_stack.push(text.save());
                // Clear redo when new action
                while (!redo_stack.empty())
                {
                    redo_stack.pop();
                }
            }

            void undo()
            {
                if (undo_stack.size() > 1)
                {
                    redo_stack.push(undo_stack.top());
                    undo_stack.pop();
                    text.restore(undo_stack.top());
                }
            }

            void redo()
            {
                if (!redo_stack.empty())
                {
                    undo_stack.push(redo_stack.top());
                    text.restore(redo_stack.top());
                    redo_stack.pop();
                }
            }

            bool can_undo() const { return undo_stack.size() > 1; }
            bool can_redo() const { return !redo_stack.empty(); }
        };

    } // namespace formatted_memento

    // ============================================================================
    // SOLUTION 3: Game Save State Memento
    // ============================================================================

    namespace game_memento
    {

        // Game state snapshot
        struct GameStateMemento
        {
            int level, score, health, ammo;
            float x, y; // Player position

            GameStateMemento(int l, int s, int h, int a, float px, float py)
                : level(l), score(s), health(h), ammo(a), x(px), y(py) {}
        };

        // Game character (originator)
        class GameCharacter
        {
        private:
            int level, score, health, ammo;
            float x, y;

        public:
            GameCharacter() : level(1), score(0), health(100), ammo(30), x(0), y(0) {}

            void take_damage(int amount) { health -= amount; }
            void fire()
            {
                if (ammo > 0)
                    ammo--;
            }
            void collect_item(int value) { score += value; }
            void move(float dx, float dy)
            {
                x += dx;
                y += dy;
            }
            void level_up() { level++; }

            int get_health() const { return health; }
            int get_score() const { return score; }
            int get_level() const { return level; }
            int get_ammo() const { return ammo; }
            float get_x() const { return x; }
            float get_y() const { return y; }

            GameStateMemento save_checkpoint() const
            {
                return GameStateMemento(level, score, health, ammo, x, y);
            }

            void load_checkpoint(const GameStateMemento &checkpoint)
            {
                level = checkpoint.level;
                score = checkpoint.score;
                health = checkpoint.health;
                ammo = checkpoint.ammo;
                x = checkpoint.x;
                y = checkpoint.y;
            }
        };

        // Game session (caretaker)
        class GameSession
        {
        private:
            GameCharacter character;
            vector<GameStateMemento> checkpoints;
            int current_checkpoint = 0;

        public:
            GameSession()
            {
                checkpoints.push_back(character.save_checkpoint());
            }

            GameCharacter &get_character() { return character; }

            void create_checkpoint()
            {
                checkpoints.push_back(character.save_checkpoint());
                current_checkpoint = checkpoints.size() - 1;
            }

            void load_checkpoint(int index)
            {
                if (index >= 0 && index < (int)checkpoints.size())
                {
                    character.load_checkpoint(checkpoints[index]);
                    current_checkpoint = index;
                }
            }

            int checkpoint_count() const { return checkpoints.size(); }
            int get_current_checkpoint() const { return current_checkpoint; }
        };

    } // namespace game_memento

    // ============================================================================
    // SOLUTION 4: Configuration Snapshot
    // ============================================================================

    namespace config_memento
    {

        struct ApplicationConfig
        {
            string theme;
            int font_size;
            bool dark_mode;
            int volume;

            ApplicationConfig(const string &t = "default", int fs = 12,
                              bool dm = false, int v = 50)
                : theme(t), font_size(fs), dark_mode(dm), volume(v) {}
        };

        class Application
        {
        private:
            ApplicationConfig config;

        public:
            void set_theme(const string &t) { config.theme = t; }
            void set_font_size(int fs) { config.font_size = fs; }
            void set_dark_mode(bool dm) { config.dark_mode = dm; }
            void set_volume(int v) { config.volume = v; }

            const ApplicationConfig &get_config() const { return config; }
            string to_string_debug() const
            {
                ostringstream oss;
                oss << "Theme: " << config.theme << ", Font: " << config.font_size
                    << ", Dark: " << (config.dark_mode ? "yes" : "no")
                    << ", Volume: " << config.volume;
                return oss.str();
            }

            ApplicationConfig save_config() const
            {
                return config;
            }

            void restore_config(const ApplicationConfig &cfg)
            {
                config = cfg;
            }
        };

        class ConfigurationManager
        {
        private:
            Application &app;
            stack<ApplicationConfig> history;

        public:
            explicit ConfigurationManager(Application &a) : app(a)
            {
                history.push(app.get_config());
            }

            void save_state()
            {
                history.push(app.get_config());
            }

            void undo()
            {
                if (history.size() > 1)
                {
                    history.pop();
                    app.restore_config(history.top());
                }
            }

            bool can_undo() const { return history.size() > 1; }
        };

    } // namespace config_memento

    // ============================================================================
    // SOLUTION 5: Database Transaction (Transaction Memento)
    // ============================================================================

    namespace transaction_memento
    {

        struct DatabaseRecord
        {
            int id;
            string name;
            string data;

            DatabaseRecord(int i = 0, const string &n = "", const string &d = "")
                : id(i), name(n), data(d) {}
        };

        struct TransactionMemento
        {
            vector<DatabaseRecord> snapshot;

            explicit TransactionMemento(const vector<DatabaseRecord> &s) : snapshot(s) {}
        };

        class Database
        {
        private:
            vector<DatabaseRecord> records;

        public:
            void insert(const DatabaseRecord &record)
            {
                records.push_back(record);
            }

            void update(int id, const string &new_data)
            {
                for (auto &r : records)
                {
                    if (r.id == id)
                    {
                        r.data = new_data;
                        return;
                    }
                }
            }

            void delete_record(int id)
            {
                records.erase(
                    remove_if(records.begin(), records.end(),
                              [id](const DatabaseRecord &r)
                              { return r.id == id; }),
                    records.end());
            }

            const vector<DatabaseRecord> &get_records() const
            {
                return records;
            }

            TransactionMemento begin_transaction() const
            {
                return TransactionMemento(records); // Snapshot
            }

            void commit_transaction(const TransactionMemento &memento)
            {
                records = memento.snapshot;
            }

            int record_count() const { return records.size(); }
        };

        class Transaction
        {
        private:
            Database &db;
            stack<TransactionMemento> savepoints;

        public:
            explicit Transaction(Database &d) : db(d)
            {
                savepoints.push(db.begin_transaction());
            }

            void create_savepoint()
            {
                savepoints.push(db.begin_transaction());
            }

            void rollback()
            {
                if (!savepoints.empty())
                {
                    db.commit_transaction(savepoints.top());
                    savepoints.pop();
                }
            }

            bool has_savepoint() const { return !savepoints.empty(); }
        };

    } // namespace transaction_memento

} // namespace memento_patterns
