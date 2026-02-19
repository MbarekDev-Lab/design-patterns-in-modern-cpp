#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <cstdint>

using namespace std;

/*
 * FLYWEIGHT PATTERN
 *
 * INTENT:
 * Use sharing to support large numbers of fine-grained objects efficiently.
 *
 * KEY CONCEPTS:
 * - Intrinsic state: Immutable, shared data (font, color, material)
 * - Extrinsic state: Mutable, context-specific data (position, size, angle)
 * - Flyweight factory: Creates/caches and reuses lightweight objects
 * - Object pooling: Reuse objects instead of creating/destroying
 *
 * BENEFITS:
 * - Dramatic memory reduction for large collections
 * - Faster object creation (reuse cached objects)
 * - Improved cache locality
 * - Better garbage collection performance
 *
 * TRADE-OFFS:
 * - Thread safety must be considered
 * - Intrinsic state must be immutable
 * - Separation of intrinsic/extrinsic state
 * - Added complexity with factories
 *
 * WHEN TO USE:
 * - Thousands/millions of similar objects
 * - Memory is a constraint
 * - Intrinsic state is expensive (fonts, textures, materials)
 * - Extrinsic state varies frequently
 */

namespace flyweight_patterns
{

    // ============================================================================
    // VIOLATION: Without Flyweight - Naive Approach (Wasteful Memory)
    // ============================================================================

    namespace violation
    {

        struct GameCharacterNaive
        {
            // Every character stores its own copy of the name
            // If we have 10,000 players named "John", we store "John" 10,000 times!
            string name;
            string class_type;     // Repeated: "Warrior", "Mage", "Rogue"
            string render_texture; // Repeated: expensive texture path
            int health;
            int x, y; // Position (extrinsic state)

            GameCharacterNaive(const string &n, const string &c,
                               const string &tex, int hp)
                : name(n), class_type(c), render_texture(tex),
                  health(hp), x(0), y(0) {}

            size_t memory_usage() const
            {
                return sizeof(*this) + name.capacity() +
                       class_type.capacity() + render_texture.capacity();
            }
        };

        void create_naive_characters()
        {
            vector<GameCharacterNaive> characters;

            // Create 10,000 characters - stores "Warrior" 5000 times!
            for (int i = 0; i < 10000; ++i)
            {
                characters.emplace_back(
                    "Player_" + to_string(i),
                    (i % 2 == 0) ? "Warrior" : "Mage",
                    "textures/class_armor.png",
                    i % 100 + 1);
            }

            size_t total = 0;
            for (const auto &c : characters)
            {
                total += c.memory_usage();
            }

            cout << "Naive approach: " << total << " bytes for 10,000 characters\n";
        }

    } // namespace violation

    // ============================================================================
    // SOLUTION 1: String Interning / String Pool (Singleton Pattern)
    // ============================================================================

    namespace string_interning
    {

        class StringPool
        {
        private:
            static unordered_map<string, shared_ptr<string>> pool;

        public:
            static shared_ptr<string> intern(const string &s)
            {
                auto it = pool.find(s);
                if (it != pool.end())
                {
                    return it->second; // Return cached copy
                }
                // Create new and cache it
                auto ptr = make_shared<string>(s);
                pool[s] = ptr;
                return ptr;
            }

            static size_t pool_size() { return pool.size(); }

            static void info()
            {
                cout << "String pool contains " << pool.size() << " unique strings\n";
            }

            static void clear_pool() { pool.clear(); }
        };

        unordered_map<string, shared_ptr<string>> StringPool::pool;

        struct GameCharacterWithStringPool
        {
            // Share string data via StringPool
            shared_ptr<string> name;       // Multiple chars can point to same "John"
            shared_ptr<string> class_type; // 10,000 Warriors share one "Warrior"
            shared_ptr<string> texture;    // All Warriors share same texture
            int health;
            int x, y;

            GameCharacterWithStringPool(const string &n, const string &c,
                                        const string &tex, int hp)
                : name(StringPool::intern(n)),
                  class_type(StringPool::intern(c)),
                  texture(StringPool::intern(tex)),
                  health(hp), x(0), y(0) {}

            size_t memory_usage() const
            {
                // Only counts unique strings once via reference counting
                return sizeof(*this);
            }
        };

    } // namespace string_interning

    // ============================================================================
    // SOLUTION 2: Hash Map Flyweight Pool (Factory Pattern)
    // ============================================================================

    namespace flyweight_pool
    {

        using FlyweightKey = uint32_t;

        // Intrinsic state: shared, immutable data
        struct CharacterTemplate
        {
            string class_name;   // "Warrior", "Mage", "Rogue"
            string texture_path; // "textures/warrior.png"
            int base_health;
            int base_stamina;

            CharacterTemplate() : base_health(0), base_stamina(0) {}

            CharacterTemplate(const string &cn, const string &tp,
                              int bh, int bs)
                : class_name(cn), texture_path(tp),
                  base_health(bh), base_stamina(bs) {}
        };

        // Extrinsic state: unique, mutable data
        struct CharacterInstance
        {
            FlyweightKey template_id; // Points to shared template
            string player_name;       // Unique per instance
            int x, y;                 // Position (changes frequently)
            int current_health;

            CharacterInstance(FlyweightKey tid, const string &pn,
                              int h, int px, int py)
                : template_id(tid), player_name(pn),
                  current_health(h), x(px), y(py) {}
        };

        class CharacterFactory
        {
        private:
            static FlyweightKey next_id;
            static unordered_map<FlyweightKey, CharacterTemplate> templates;

        public:
            static FlyweightKey create_template(const string &class_name,
                                                const string &texture,
                                                int health, int stamina)
            {
                FlyweightKey id = ++next_id;
                templates.emplace(id, CharacterTemplate(class_name, texture, health, stamina));
                return id;
            }

            static FlyweightKey get_template_by_name(const string &name)
            {
                for (auto &[id, tmpl] : templates)
                {
                    if (tmpl.class_name == name)
                        return id;
                }
                return 0; // Not found
            }

            static const CharacterTemplate &get_template(FlyweightKey id)
            {
                static CharacterTemplate empty("", "", 0, 0);
                auto it = templates.find(id);
                if (it != templates.end())
                {
                    return it->second;
                }
                return empty;
            }

            static size_t templates_count() { return templates.size(); }

            // Test helpers: reset factory state
            static void clear_templates()
            {
                templates.clear();
                next_id = 0;
            }
        };

        FlyweightKey CharacterFactory::next_id = 0;
        unordered_map<FlyweightKey, CharacterTemplate> CharacterFactory::templates;

    } // namespace flyweight_pool

    // ============================================================================
    // SOLUTION 3: Game Resource Flyweight with Sharing
    // ============================================================================

    namespace graphics_flyweight
    {

        // Shared intrinsic state: expensive to duplicate
        struct Material
        {
            string name;
            string diffuse_texture;
            string normal_texture;
            float roughness;
            float metallic;

            Material(const string &n, const string &dt,
                     const string &nt, float r, float m)
                : name(n), diffuse_texture(dt), normal_texture(nt),
                  roughness(r), metallic(m) {}

            size_t memory_bytes() const
            {
                return sizeof(*this) + diffuse_texture.size() + normal_texture.size();
            }
        };

        class MaterialFactory
        {
        private:
            static unordered_map<string, shared_ptr<Material>> materials;

        public:
            static shared_ptr<Material> get_material(const string &name,
                                                     const string &diffuse,
                                                     const string &normal,
                                                     float roughness = 0.5f,
                                                     float metallic = 0.0f)
            {
                if (materials.find(name) != materials.end())
                {
                    return materials[name]; // Return cached
                }

                auto mat = make_shared<Material>(name, diffuse, normal, roughness, metallic);
                materials[name] = mat;
                return mat;
            }

            static size_t material_count() { return materials.size(); }

            static size_t total_memory()
            {
                size_t total = 0;
                for (auto &[name, mat] : materials)
                {
                    total += mat->memory_bytes();
                }
                return total;
            }

            static void clear_materials() { materials.clear(); }
        };

        unordered_map<string, shared_ptr<Material>> MaterialFactory::materials;

        // Mesh: uses shared Material
        struct Mesh
        {
            string name;
            shared_ptr<Material> material; // Shared across many meshes
            int vertex_count;
            int x, y, z; // Position

            Mesh(const string &n, shared_ptr<Material> m,
                 int vc, int px, int py, int pz)
                : name(n), material(m), vertex_count(vc),
                  x(px), y(py), z(pz) {}

            size_t memory_bytes() const
            {
                return sizeof(*this) + name.capacity();
                // Material NOT counted - it's shared!
            }
        };

    } // namespace graphics_flyweight

    // ============================================================================
    // SOLUTION 4: Text Formatting with Flyweight Styles
    // ============================================================================

    namespace text_flyweight
    {

        // Intrinsic state: shared formatting
        struct TextStyle
        {
            string font_name;
            int font_size;
            bool bold, italic, underline;
            uint32_t color;

            TextStyle(const string &fn, int fs, bool b, bool i, bool u, uint32_t c)
                : font_name(fn), font_size(fs), bold(b), italic(i),
                  underline(u), color(c) {}

            string to_string_debug() const
            {
                return font_name + "_" + ::to_string(font_size) + "_" +
                       (bold ? "B" : "") + (italic ? "I" : "") +
                       (underline ? "U" : "");
            }
        };

        class StyleFactory
        {
        private:
            static map<string, shared_ptr<TextStyle>> styles;

            static string style_key(const string &fn, int fs,
                                    bool b, bool i, bool u, uint32_t c)
            {
                return fn + "_" + ::to_string(fs) + "_" +
                       ::to_string(b) + ::to_string(i) + ::to_string(u) +
                       ::to_string(c);
            }

        public:
            static shared_ptr<TextStyle> get_style(const string &font,
                                                   int size, bool bold,
                                                   bool italic, bool underline,
                                                   uint32_t color)
            {
                string key = style_key(font, size, bold, italic, underline, color);

                if (styles.find(key) != styles.end())
                {
                    return styles[key];
                }

                auto style = make_shared<TextStyle>(font, size, bold, italic, underline, color);
                styles[key] = style;
                return style;
            }

            static size_t style_count() { return styles.size(); }

            // Test helper: clear styles
            static void clear_styles() { styles.clear(); }
        };

        map<string, shared_ptr<TextStyle>> StyleFactory::styles;

        // Extrinsic state: unique character position
        struct FormattedCharacter
        {
            char glyph;
            shared_ptr<TextStyle> style; // Shared style
            int row, col;                // Position varies

            FormattedCharacter(char g, shared_ptr<TextStyle> s, int r, int c)
                : glyph(g), style(s), row(r), col(c) {}
        };

    } // namespace text_flyweight

    // ============================================================================
    // SOLUTION 5: Tree Forest Flyweight
    // ============================================================================

    namespace forest_flyweight
    {

        // Intrinsic state: shared tree type data
        struct TreeType
        {
            string name;
            string texture;
            int height;
            uint32_t color; // Packed RGB

            TreeType(const string &n, const string &t, int h, uint32_t c)
                : name(n), texture(t), height(h), color(c) {}
        };

        class TreeTypeFactory
        {
        private:
            static unordered_map<string, shared_ptr<TreeType>> types;

        public:
            static shared_ptr<TreeType> get_tree_type(const string &name,
                                                      const string &texture,
                                                      int height,
                                                      uint32_t color)
            {
                if (types.find(name) != types.end())
                {
                    return types[name];
                }

                auto type = make_shared<TreeType>(name, texture, height, color);
                types[name] = type;
                return type;
            }

            static size_t type_count() { return types.size(); }

            // Test helper: clear types
            static void clear_types() { types.clear(); }
        };

        unordered_map<string, shared_ptr<TreeType>> TreeTypeFactory::types;

        // Extrinsic state: position varies per tree
        struct Tree
        {
            shared_ptr<TreeType> type; // Shared type
            int x, y, z;               // Unique position

            Tree(shared_ptr<TreeType> t, int px, int py, int pz)
                : type(t), x(px), y(py), z(pz) {}
        };

    } // namespace forest_flyweight

} // namespace flyweight_patterns
