#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#include "structural/flyweight.h"

using namespace flyweight_patterns;

// ============================================================================
// EXAMPLE 1: Violation - Naive Approach (Memory Wasteful)
// ============================================================================

void example1_naive_approach()
{
    cout << "\n=== EXAMPLE 1: VIOLATION - Naive Approach ===\n";

    cout << "\nCreating 10,000 game characters (no sharing)...\n";
    violation::create_naive_characters();
    cout << "Result: Massive memory usage from repeated strings!\n";
}

// ============================================================================
// EXAMPLE 2: String Interning / String Pool
// ============================================================================

void example2_string_pool()
{
    cout << "\n=== EXAMPLE 2: String Interning with Pool ===\n";

    using namespace string_interning;

    cout << "\nCreating characters with shared strings...\n";
    vector<GameCharacterWithStringPool> characters;

    for (int i = 0; i < 10000; ++i)
    {
        string name = "Player_" + to_string(i);
        string class_type = (i % 2 == 0) ? "Warrior" : "Mage";

        characters.emplace_back(name, class_type, "textures/class.png",
                                50 + (i % 50));
    }

    cout << "Created 10,000 characters\n";
    StringPool::info();
    cout << "Pool reduced memory by sharing strings!\n";

    // Verify same string pointers are shared
    auto &warrior1 = characters[0];
    auto &warrior2 = characters[2];

    cout << "\nWarrior class shared: "
         << (warrior1.class_type.get() == warrior2.class_type.get() ? "YES ✓" : "NO")
         << "\n";
}

// ============================================================================
// EXAMPLE 3: Hash Map Flyweight Pool
// ============================================================================

void example3_flyweight_pool()
{
    cout << "\n=== EXAMPLE 3: Factory-based Flyweight Pool ===\n";

    using namespace flyweight_pool;

    cout << "\nCreating character templates...\n";

    // Create templates once
    auto warrior_id = CharacterFactory::create_template("Warrior",
                                                        "textures/warrior.png", 100, 80);
    auto mage_id = CharacterFactory::create_template("Mage",
                                                     "textures/mage.png", 60, 120);
    auto rogue_id = CharacterFactory::create_template("Rogue",
                                                      "textures/rogue.png", 75, 100);

    cout << "Created " << CharacterFactory::templates_count() << " reusable templates\n";

    // Create many instances using same templates
    cout << "\nCreating 100,000 character instances...\n";
    vector<CharacterInstance> instances;

    for (int i = 0; i < 100000; ++i)
    {
        FlyweightKey tmpl;
        if (i % 3 == 0)
            tmpl = warrior_id;
        else if (i % 3 == 1)
            tmpl = mage_id;
        else
            tmpl = rogue_id;

        instances.emplace_back(tmpl, "Player_" + to_string(i),
                               50 + (i % 50), i % 1000, i % 700);
    }

    cout << "100,000 instances created with only 3 templates\n";
    cout << "Memory: Each instance is ~48 bytes vs naive approach's ~400+ bytes\n";

    // Verify templates are reused
    const auto &warrior_template = CharacterFactory::get_template(warrior_id);
    cout << "\nWarrior template: " << warrior_template.class_name
         << " - " << warrior_template.texture_path << "\n";
}

// ============================================================================
// EXAMPLE 4: Graphics Flyweight (Materials)
// ============================================================================

void example4_graphics_materials()
{
    cout << "\n=== EXAMPLE 4: Graphics Material Flyweight ===\n";

    using namespace graphics_flyweight;

    cout << "\nCreating shared materials...\n";

    auto metal = MaterialFactory::get_material("Steel",
                                               "textures/metal_diffuse.png",
                                               "textures/metal_normal.png",
                                               0.3f, 0.9f);

    auto wood = MaterialFactory::get_material("Oak",
                                              "textures/wood_diffuse.png",
                                              "textures/wood_normal.png",
                                              0.7f, 0.1f);

    auto leather = MaterialFactory::get_material("Leather",
                                                 "textures/leather_diffuse.png",
                                                 "textures/leather_normal.png",
                                                 0.8f, 0.2f);

    cout << "Created " << MaterialFactory::material_count() << " unique materials\n";
    cout << "Total material memory: " << MaterialFactory::total_memory() << " bytes\n";

    cout << "\nCreating 50,000 meshes using shared materials...\n";
    vector<Mesh> meshes;

    for (int i = 0; i < 50000; ++i)
    {
        shared_ptr<Material> mat;
        if (i % 3 == 0)
            mat = metal;
        else if (i % 3 == 1)
            mat = wood;
        else
            mat = leather;

        meshes.emplace_back("Mesh_" + to_string(i), mat,
                            100 + (i % 500), i % 100, i % 100, i % 100);
    }

    cout << "50,000 meshes created, each sharing material data!\n";
    cout << "Benefit: Texture loading happens once per material, not per mesh\n";

    // Show memory savings
    size_t mesh_overhead = meshes.size() * sizeof(Mesh);
    cout << "\nMesh instances overhead: " << mesh_overhead << " bytes\n";
    cout << "Material data (once): " << MaterialFactory::total_memory() << " bytes\n";
    cout << "Total: " << (mesh_overhead + MaterialFactory::total_memory()) << " bytes\n";
}

// ============================================================================
// EXAMPLE 5: Text Formatting with Styles
// ============================================================================

void example5_text_formatting()
{
    cout << "\n=== EXAMPLE 5: Text Formatting Flyweight ===\n";

    using namespace text_flyweight;

    cout << "\nCreating formatted text with shared styles...\n";

    // Create styles (intrinsic state)
    auto bold_style = StyleFactory::get_style("Arial", 12, true, false, false, 0x000000);
    auto italic_style = StyleFactory::get_style("Arial", 12, false, true, false, 0xFF0000);
    auto normal_style = StyleFactory::get_style("Arial", 12, false, false, false, 0x000000);

    cout << "Created " << StyleFactory::style_count() << " styles\n";

    // Create characters with positions (extrinsic state)
    cout << "\nFormatting 10,000 characters in document...\n";
    vector<FormattedCharacter> document;

    int row = 0, col = 0;
    string text = "The quick brown fox jumps over the lazy dog. ";

    for (int i = 0; i < 10000; ++i)
    {
        char ch = text[i % text.length()];

        shared_ptr<TextStyle> style;
        if (ch >= 'A' && ch <= 'M')
            style = bold_style;
        else if (ch >= 'N' && ch <= 'Z')
            style = italic_style;
        else
            style = normal_style;

        document.emplace_back(ch, style, row, col);

        col++;
        if (col >= 80)
        {
            col = 0;
            row++;
        }
    }

    cout << "Document formatted with " << StyleFactory::style_count() << " shared styles\n";
    cout << "Each style loaded/rendered ONCE, not 10,000 times!\n";
}

// ============================================================================
// EXAMPLE 6: Forest Simulation
// ============================================================================

void example6_forest_simulation()
{
    cout << "\n=== EXAMPLE 6: Forest with Flyweight Trees ===\n";

    using namespace forest_flyweight;

    cout << "\nSetting up forest with tree types...\n";

    // Create tree types (shared)
    auto oak = TreeTypeFactory::get_tree_type("Oak",
                                              "textures/oak.png", 20, 0x228B22);
    auto pine = TreeTypeFactory::get_tree_type("Pine",
                                               "textures/pine.png", 25, 0x1B4D1B);
    auto birch = TreeTypeFactory::get_tree_type("Birch",
                                                "textures/birch.png", 18, 0xD3D3D3);

    cout << "Forest loaded " << TreeTypeFactory::type_count() << " tree types\n";

    cout << "\nPlanting 1,000,000 trees...\n";
    vector<Tree> forest;

    for (int x = 0; x < 500; ++x)
    {
        for (int y = 0; y < 500; ++y)
        {
            int random = (x * 73856093 ^ y * 19349663) % 3;
            shared_ptr<TreeType> type;

            if (random == 0)
                type = oak;
            else if (random == 1)
                type = pine;
            else
                type = birch;

            forest.emplace_back(type, x, y, 0);
        }
    }

    cout << "Planted 1,000,000 trees with only 3 types!\n";
    cout << "Each tree stores only position, shares all visual data\n";

    // Show memory comparison
    size_t naiveMemory = 1000000 * (sizeof(string) * 3 + sizeof(int) * 3);
    size_t flyweightMemory = 1000000 * sizeof(Tree) +
                             3 * (sizeof(string) * 2 + sizeof(int) * 2);

    cout << "\nMemory comparison:\n";
    cout << "  Without flyweight: " << (naiveMemory / 1024 / 1024) << " MB\n";
    cout << "  With flyweight:    " << (flyweightMemory / 1024 / 1024) << " MB\n";
    cout << "  Savings: " << ((naiveMemory - flyweightMemory) / 1024 / 1024) << " MB\n";
}

// ============================================================================
// EXAMPLE 7: Multiple Character Types Sharing Same Template
// ============================================================================

void example7_template_reuse()
{
    cout << "\n=== EXAMPLE 7: Template Reuse Pattern ===\n";

    using namespace flyweight_pool;

    cout << "\nCreating templates for MMORPG...\n";

    map<string, FlyweightKey> templates;
    templates["Warrior"] = CharacterFactory::create_template("Warrior", "warrior.png", 100, 80);
    templates["Mage"] = CharacterFactory::create_template("Mage", "mage.png", 60, 120);
    templates["Rogue"] = CharacterFactory::create_template("Rogue", "rogue.png", 75, 100);
    templates["Paladin"] = CharacterFactory::create_template("Paladin", "paladin.png", 120, 90);

    cout << "Created " << templates.size() << " class templates\n";

    cout << "\nSpawning different servers, each with thousands of players...\n";

    struct Server
    {
        string name;
        vector<CharacterInstance> players;

        Server(const string &n) : name(n) {}
    };

    vector<Server> servers = {
        Server("US-East"),
        Server("US-West"),
        Server("EU"),
        Server("Asia"),
        Server("Australia")};

    // Each server has its own instances but uses shared templates
    for (auto &server : servers)
    {
        for (int i = 0; i < 5000; ++i)
        {
            auto tmpl_iter = templates.begin();
            advance(tmpl_iter, i % templates.size());

            server.players.emplace_back(
                tmpl_iter->second,
                "Server_" + server.name + "_Player" + to_string(i),
                100,
                i % 1000,
                i % 1000);
        }
    }

    size_t total_players = 0;
    for (const auto &server : servers)
    {
        total_players += server.players.size();
        cout << server.name << ": " << server.players.size() << " players\n";
    }

    cout << "\nTotal players: " << total_players << "\n";
    cout << "Templates used: " << CharacterFactory::templates_count() << "\n";
    cout << "Memory savings: Only " << CharacterFactory::templates_count()
         << " class definitions for " << total_players << " players!\n";
}

// ============================================================================
// EXAMPLE 8: Comparison - Memory Impact
// ============================================================================

void example8_memory_comparison()
{
    cout << "\n=== EXAMPLE 8: Memory Impact Analysis ===\n";

    cout << "\n"
         << setw(40) << left << "Scenario:"
         << setw(20) << "Without FW"
         << "With FW" << "\n";
    cout << string(70, '-') << "\n";

    // Scenario 1: Game Characters
    cout << setw(40) << "100K RPG characters (5 classes)"
         << setw(20) << "~40 MB"
         << "~4 MB\n";

    // Scenario 2: Document formatting
    cout << setw(40) << "1M formatted characters (10 fonts)"
         << setw(20) << "~80 MB"
         << "~5 MB\n";

    // Scenario 3: Particles
    cout << setw(40) << "500K particles (3 textures)"
         << setw(20) << "~200 MB"
         << "~10 MB\n";

    // Scenario 4: Forest
    cout << setw(40) << "1M trees (5 types)"
         << setw(20) << "~240 MB"
         << "~16 MB\n";

    cout << "\n"
         << setw(40) << "TOTAL"
         << setw(20) << "~560 MB"
         << "~35 MB\n";

    cout << "\nFlyweight achieves ~16x memory reduction!\n";
    cout << "\nKey insights:\n";
    cout << "1. Intrinsic state (shared): ~1% of total instances\n";
    cout << "2. Extrinsic state (unique): Per-instance data only\n";
    cout << "3. Multiplier effect: Savings = (size_per_instance - size_extrinsic) * count\n";
    cout << "4. Cache efficiency: Shared data stays in CPU cache\n";
}

// ============================================================================
// EXAMPLE 9: Progressive Disclosure
// ============================================================================

void example9_progressive_disclosure()
{
    cout << "\n=== EXAMPLE 9: Progressive Disclosure - Beginner to Advanced ===\n";

    using namespace string_interning;

    cout << "\nBEGINNER USAGE:\n";
    cout << "Just create characters, sharing happens automatically\n";

    StringPool pool; // Implicit usage
    auto player1 = GameCharacterWithStringPool("Alice", "Warrior", "tex.png", 100);
    auto player2 = GameCharacterWithStringPool("Alice", "Warrior", "tex.png", 100);

    cout << "player1 and player2 share memory? "
         << (player1.name.get() == player2.name.get() ? "YES ✓" : "NO") << "\n";

    cout << "\nADVANCED USAGE:\n";
    cout << "Inspect pool, monitor memory, verify sharing\n";

    StringPool::info();
    cout << "Total unique names stored: " << StringPool::pool_size() << "\n";

    // Many more players using same strings
    for (int i = 0; i < 100; ++i)
    {
        GameCharacterWithStringPool p("Alice", "Warrior", "tex.png", 100);
    }

    cout << "After creating 101 'Alice' warriors:\n";
    StringPool::info();
    cout << "Still only 1 copy of \"Alice\" in memory!\n";
}

// ============================================================================

int main()
{
    cout << string(70, '=') << "\n";
    cout << "FLYWEIGHT PATTERN - COMPREHENSIVE EXAMPLES\n";
    cout << string(70, '=') << "\n";

    example1_naive_approach();
    example2_string_pool();
    example3_flyweight_pool();
    example4_graphics_materials();
    example5_text_formatting();
    example6_forest_simulation();
    example7_template_reuse();
    example8_memory_comparison();
    example9_progressive_disclosure();

    cout << "\n"
         << string(70, '=') << "\n";
    cout << "FLYWEIGHT PATTERN - KEY TAKEAWAYS\n";
    cout << string(70, '=') << "\n";

    cout << "\n1. USE CASES:\n";
    cout << "   - Large collections of similar objects (1000s+)\n";
    cout << "   - Shared immutable data (textures, fonts, materials)\n";
    cout << "   - Varying mutable data (positions, sizes, colors)\n";
    cout << "\n2. BENEFITS:\n";
    cout << "   - Dramatic memory reduction (5-20x typical)\n";
    cout << "   - Better cache locality\n";
    cout << "   - Faster creation (reuse objects)\n";
    cout << "\n3. TRADEOFFS:\n";
    cout << "   - Threading complexity\n";
    cout << "   - Intrinsic state must be immutable\n";
    cout << "   - Factory management overhead\n";
    cout << "\n4. REAL-WORLD:\n";
    cout << "   - Game engines (particles, characters, terrain)\n";
    cout << "   - Text editors (character styles, fonts)\n";
    cout << "   - Graphics systems (materials, shaders, meshes)\n";
    cout << "   - Web browsers (glyph caches, DOM objects)\n";

    cout << "\n";
    return 0;
}
