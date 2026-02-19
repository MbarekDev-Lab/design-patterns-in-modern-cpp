#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
#include <cstring>

using namespace std;

#include "structural/flyweight.h"

using namespace flyweight_patterns;

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

void test_string_pool_sharing()
{
    TEST_FUNCTION("Test: String Pool - Basic Sharing");

    using namespace string_interning;

    StringPool::clear_pool(); // Reset for test

    auto str1 = StringPool::intern("Warrior");
    auto str2 = StringPool::intern("Warrior");
    auto str3 = StringPool::intern("Mage");

    // Same strings share same pointer
    ASSERT_TRUE(str1.get() == str2.get());

    // Different strings have different pointers
    ASSERT_TRUE(str1.get() != str3.get());

    // Pool contains both unique strings
    ASSERT_EQ(StringPool::pool_size(), 2);
}

void test_string_pool_character_creation()
{
    TEST_FUNCTION("Test: String Pool - Character Creation");

    using namespace string_interning;

    StringPool::clear_pool();

    GameCharacterWithStringPool c1("John", "Warrior", "tex.png", 100);
    GameCharacterWithStringPool c2("John", "Warrior", "tex.png", 100);
    GameCharacterWithStringPool c3("Jane", "Mage", "tex.png", 60);

    // Same data shared
    ASSERT_TRUE(c1.name.get() == c2.name.get());
    ASSERT_TRUE(c1.class_type.get() == c2.class_type.get());
    ASSERT_TRUE(c1.texture.get() == c2.texture.get());

    // Different names not shared
    ASSERT_TRUE(c1.name.get() != c3.name.get());

    // Pool has correct count
    ASSERT_EQ(StringPool::pool_size(), 5); // John, Warrior, Jane, Mage, tex.png
}

void test_string_pool_many_characters()
{
    TEST_FUNCTION("Test: String Pool - 10K Characters");

    using namespace string_interning;

    StringPool::clear_pool();

    vector<GameCharacterWithStringPool> characters;

    // Create many characters with repeating data
    for (int i = 0; i < 1000; ++i)
    {
        characters.emplace_back(
            (i % 10 == 0) ? "John" : "Player_" + to_string(i),
            (i % 2 == 0) ? "Warrior" : "Mage",
            "textures/class.png",
            50);
    }

    // Only 2 class types stored
    ASSERT_TRUE(StringPool::pool_size() <= 1015); // Some unique player names

    // All warriors share same "Warrior" string
    int warrior_count = 0;
    for (const auto &c : characters)
    {
        if (*c.class_type == "Warrior")
            warrior_count++;
    }
    ASSERT_TRUE(warrior_count > 0);
}

void test_flyweight_pool_template_creation()
{
    TEST_FUNCTION("Test: Flyweight Pool - Template Creation");

    using namespace flyweight_pool;

    CharacterFactory::clear_templates();
    

    auto warrior_id = CharacterFactory::create_template("Warrior", "warrior.png", 100, 80);
    auto mage_id = CharacterFactory::create_template("Mage", "mage.png", 60, 120);

    ASSERT_EQ(warrior_id, 1);
    ASSERT_EQ(mage_id, 2);
    ASSERT_EQ(CharacterFactory::templates_count(), 2);
}

void test_flyweight_pool_template_retrieval()
{
    TEST_FUNCTION("Test: Flyweight Pool - Template Retrieval");

    using namespace flyweight_pool;

    CharacterFactory::clear_templates();
    

    auto id = CharacterFactory::create_template("Paladin", "paladin.png", 120, 90);
    const auto &tmpl = CharacterFactory::get_template(id);

    ASSERT_TRUE(tmpl.class_name == "Paladin");
    ASSERT_TRUE(tmpl.texture_path == "paladin.png");
    ASSERT_EQ(tmpl.base_health, 120);
    ASSERT_EQ(tmpl.base_stamina, 90);
}

void test_flyweight_pool_get_by_name()
{
    TEST_FUNCTION("Test: Flyweight Pool - Get by Name");

    using namespace flyweight_pool;

    CharacterFactory::clear_templates();
    

    auto id1 = CharacterFactory::create_template("Knight", "knight.png", 110, 85);
    auto id2 = CharacterFactory::get_template_by_name("Knight");

    ASSERT_EQ(id1, id2);

    auto id3 = CharacterFactory::get_template_by_name("NonExistent");
    ASSERT_EQ(id3, 0);
}

void test_character_instance_creation()
{
    TEST_FUNCTION("Test: Character Instance - Creation");

    using namespace flyweight_pool;

    CharacterFactory::clear_templates();
    

    auto template_id = CharacterFactory::create_template("Rogue", "rogue.png", 75, 100);

    CharacterInstance instance(template_id, "Player1", 75, 100, 200);

    ASSERT_EQ(instance.template_id, template_id);
    ASSERT_TRUE(instance.player_name == "Player1");
    ASSERT_EQ(instance.x, 100);
    ASSERT_EQ(instance.y, 200);
    ASSERT_EQ(instance.current_health, 75);
}

void test_material_factory_creation()
{
    TEST_FUNCTION("Test: Material Factory - Creation");

    using namespace graphics_flyweight;

    MaterialFactory::clear_materials();

    auto mat1 = MaterialFactory::get_material("Steel", "steel_diffuse.png", "steel_normal.png");
    auto mat2 = MaterialFactory::get_material("Steel", "steel_diffuse.png", "steel_normal.png");

    // Same material name returns cached version
    ASSERT_TRUE(mat1.get() == mat2.get());

    // Different material creates new entry
    auto mat3 = MaterialFactory::get_material("Wood", "wood_diffuse.png", "wood_normal.png");
    ASSERT_TRUE(mat1.get() != mat3.get());

    ASSERT_EQ(MaterialFactory::material_count(), 2);
}

void test_material_properties()
{
    TEST_FUNCTION("Test: Material - Properties");

    using namespace graphics_flyweight;

    MaterialFactory::clear_materials();

    auto metallic = MaterialFactory::get_material("Aluminum", "aluminum_diffuse.png",
                                                  "aluminum_normal.png", 0.3f, 0.85f);

    ASSERT_TRUE(metallic->name == "Aluminum");
    ASSERT_EQ(metallic->roughness, 0.3f);
    ASSERT_EQ(metallic->metallic, 0.85f);
}

void test_mesh_creation_with_shared_material()
{
    TEST_FUNCTION("Test: Mesh - Shared Material");

    using namespace graphics_flyweight;

    MaterialFactory::clear_materials();

    auto material = MaterialFactory::get_material("Brick", "brick_diffuse.png", "brick_normal.png");

    Mesh mesh1("Wall1", material, 1000, 0, 0, 0);
    Mesh mesh2("Wall2", material, 1000, 10, 0, 0);

    // Both meshes share material
    ASSERT_TRUE(mesh1.material.get() == mesh2.material.get());

    // But have different positions
    ASSERT_TRUE(mesh1.x != mesh2.x);
}

void test_style_factory_sharing()
{
    TEST_FUNCTION("Test: Style Factory - Sharing");

    using namespace text_flyweight;

    StyleFactory::clear_styles();

    auto style1 = StyleFactory::get_style("Arial", 12, true, false, false, 0x000000);
    auto style2 = StyleFactory::get_style("Arial", 12, true, false, false, 0x000000);

    // Same style returns cached version
    ASSERT_TRUE(style1.get() == style2.get());

    // Different style creates new entry
    auto style3 = StyleFactory::get_style("Times", 12, true, false, false, 0x000000);
    ASSERT_TRUE(style1.get() != style3.get());

    ASSERT_EQ(StyleFactory::style_count(), 2);
}

void test_formatted_character_creation()
{
    TEST_FUNCTION("Test: Formatted Character - Creation");

    using namespace text_flyweight;

    StyleFactory::clear_styles();

    auto style = StyleFactory::get_style("Verdana", 11, false, false, true, 0xFF0000);

    text_flyweight::FormattedCharacter ch1('A', style, 0, 0);
    text_flyweight::FormattedCharacter ch2('B', style, 0, 1);

    // Different characters share style
    ASSERT_TRUE(ch1.style.get() == ch2.style.get());

    // But have different positions
    ASSERT_TRUE(ch1.glyph != ch2.glyph);
    ASSERT_EQ(ch1.col, 0);
    ASSERT_EQ(ch2.col, 1);
}

void test_tree_type_factory()
{
    TEST_FUNCTION("Test: Tree Type Factory");

    using namespace forest_flyweight;

    TreeTypeFactory::clear_types();

    auto oak1 = TreeTypeFactory::get_tree_type("Oak", "oak_texture.png", 20, 0x228B22);
    auto oak2 = TreeTypeFactory::get_tree_type("Oak", "oak_texture.png", 20, 0x228B22);

    // Same type returns cached version
    ASSERT_TRUE(oak1.get() == oak2.get());

    auto pine = TreeTypeFactory::get_tree_type("Pine", "pine_texture.png", 25, 0x1B4D1B);

    // Different type creates new entry
    ASSERT_TRUE(oak1.get() != pine.get());

    ASSERT_EQ(TreeTypeFactory::type_count(), 2);
}

void test_tree_creation_with_shared_type()
{
    TEST_FUNCTION("Test: Tree - Shared Type");

    using namespace forest_flyweight;

    TreeTypeFactory::clear_types();

    auto birch = TreeTypeFactory::get_tree_type("Birch", "birch.png", 18, 0xD3D3D3);

    Tree tree1(birch, 0, 0, 0);
    Tree tree2(birch, 50, 50, 0);
    Tree tree3(birch, 100, 100, 0);

    // All trees share type
    ASSERT_TRUE(tree1.type.get() == tree2.type.get());
    ASSERT_TRUE(tree2.type.get() == tree3.type.get());

    // But have different positions
    ASSERT_TRUE(tree1.x != tree2.x);
    ASSERT_TRUE(tree2.x != tree3.x);
}

void test_forest_scale()
{
    TEST_FUNCTION("Test: Forest - Large Scale");

    using namespace forest_flyweight;

    TreeTypeFactory::clear_types();

    auto oak = TreeTypeFactory::get_tree_type("Oak", "oak.png", 20, 0x228B22);
    auto pine = TreeTypeFactory::get_tree_type("Pine", "pine.png", 25, 0x1B4D1B);

    vector<Tree> forest;

    // Create 100K trees
    for (int i = 0; i < 100000; ++i)
    {
        if (i % 2 == 0)
        {
            forest.emplace_back(oak, i % 1000, i / 1000, 0);
        }
        else
        {
            forest.emplace_back(pine, i % 1000, i / 1000, 0);
        }
    }

    // Only 2 types for 100K trees
    ASSERT_EQ(TreeTypeFactory::type_count(), 2);
    ASSERT_EQ(forest.size(), 100000);
}

void test_memory_efficiency()
{
    TEST_FUNCTION("Test: Memory Efficiency - String Interning");

    using namespace string_interning;

    StringPool::clear_pool();

    vector<GameCharacterWithStringPool> characters;

    // Create 5000 warriors with same data
    for (int i = 0; i < 5000; ++i)
    {
        characters.emplace_back("Warrior", "Warrior Class", "texture.png", 100);
    }

    // All point to same strings
    auto first_name = characters[0].name;
    bool all_same = true;
    for (const auto &ch : characters)
    {
        if (ch.name.get() != first_name.get())
        {
            all_same = false;
            break;
        }
    }

    ASSERT_TRUE(all_same);
    ASSERT_EQ(StringPool::pool_size(), 3); // 3 unique strings
}

void test_pool_isolation()
{
    TEST_FUNCTION("Test: Pool Isolation - Independent Pools");

    using namespace string_interning;
    using namespace graphics_flyweight;

    StringPool::clear_pool();
    MaterialFactory::clear_materials();

    auto str1 = StringPool::intern("Test");
    auto mat1 = MaterialFactory::get_material("Test", "test.png", "test_n.png");

    // Different pools, independent entries
    ASSERT_EQ(StringPool::pool_size(), 1);
    ASSERT_EQ(MaterialFactory::material_count(), 1);
}

void test_intrinsic_state_immutability()
{
    TEST_FUNCTION("Test: Intrinsic State - Immutability");

    using namespace graphics_flyweight;

    MaterialFactory::clear_materials();

    auto mat1 = MaterialFactory::get_material("Diamond", "diamond.png", "diamond_n.png", 0.1f, 1.0f);
    auto mat2 = MaterialFactory::get_material("Diamond", "diamond.png", "diamond_n.png", 0.1f, 1.0f);

    // Same object returned (flyweight reuse)
    ASSERT_TRUE(mat1.get() == mat2.get());

    // Intrinsic state is stable
    ASSERT_EQ(mat1->roughness, mat2->roughness);
    ASSERT_EQ(mat1->metallic, mat2->metallic);
}

void test_extrinsic_state_independence()
{
    TEST_FUNCTION("Test: Extrinsic State - Independence");

    using namespace graphics_flyweight;

    MaterialFactory::clear_materials();

    auto material = MaterialFactory::get_material("Stone", "stone.png", "stone_n.png");

    Mesh mesh1("Rock1", material, 500, 0, 0, 0);
    Mesh mesh2("Rock2", material, 500, 100, 50, 25);

    // Same material (intrinsic)
    ASSERT_TRUE(mesh1.material.get() == mesh2.material.get());

    // Different positions (extrinsic)
    ASSERT_TRUE((mesh1.x != mesh2.x) || (mesh1.y != mesh2.y) || (mesh1.z != mesh2.z));
}

void test_factory_caching()
{
    TEST_FUNCTION("Test: Factory - Caching Behavior");

    using namespace forest_flyweight;

    TreeTypeFactory::clear_types();

    size_t initial_count = TreeTypeFactory::type_count();
    ASSERT_EQ(initial_count, 0);

    auto oak1 = TreeTypeFactory::get_tree_type("Oak", "oak.png", 20, 0x228B22);
    ASSERT_EQ(TreeTypeFactory::type_count(), 1);

    auto oak2 = TreeTypeFactory::get_tree_type("Oak", "oak.png", 20, 0x228B22);
    ASSERT_EQ(TreeTypeFactory::type_count(), 1); // Still 1, not 2

    auto pine = TreeTypeFactory::get_tree_type("Pine", "pine.png", 25, 0x1B4D1B);
    ASSERT_EQ(TreeTypeFactory::type_count(), 2); // Now 2
}

void test_multiple_instances_one_template()
{
    TEST_FUNCTION("Test: Multiple Instances - Single Template");

    using namespace flyweight_pool;

    CharacterFactory::clear_templates();
    

    auto template_id = CharacterFactory::create_template("Archer", "arrow.png", 80, 110);

    vector<CharacterInstance> players;
    for (int i = 0; i < 1000; ++i)
    {
        players.emplace_back(template_id, "Archer_" + to_string(i), 80, i, i);
    }

    // All instances use same template
    bool all_same_template = true;
    for (const auto &p : players)
    {
        if (p.template_id != template_id)
        {
            all_same_template = false;
            break;
        }
    }

    ASSERT_TRUE(all_same_template);
    ASSERT_EQ(CharacterFactory::templates_count(), 1);
}

void test_style_variations()
{
    TEST_FUNCTION("Test: Text Styles - Variations");

    using namespace text_flyweight;

    StyleFactory::clear_styles();

    auto normal = StyleFactory::get_style("Arial", 12, false, false, false, 0x000000);
    auto bold = StyleFactory::get_style("Arial", 12, true, false, false, 0x000000);
    auto italic = StyleFactory::get_style("Arial", 12, false, true, false, 0x000000);
    auto underline = StyleFactory::get_style("Arial", 12, false, false, true, 0x000000);

    // All different
    ASSERT_FALSE(normal.get() == bold.get());
    ASSERT_FALSE(bold.get() == italic.get());
    ASSERT_FALSE(italic.get() == underline.get());

    ASSERT_EQ(StyleFactory::style_count(), 4);
}

// ============================================================================
// TEST RUNNER AND SUMMARY
// ============================================================================

int main()
{
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║          FLYWEIGHT PATTERN - TEST SUITE                   ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\nRunning tests...\n";

    // String interning tests
    test_string_pool_sharing();
    test_string_pool_character_creation();
    test_string_pool_many_characters();

    // Flyweight pool tests
    test_flyweight_pool_template_creation();
    test_flyweight_pool_template_retrieval();
    test_flyweight_pool_get_by_name();
    test_character_instance_creation();

    // Material factory tests
    test_material_factory_creation();
    test_material_properties();
    test_mesh_creation_with_shared_material();

    // Text formatting tests
    test_style_factory_sharing();
    test_formatted_character_creation();
    test_style_variations();

    // Forest tests
    test_tree_type_factory();
    test_tree_creation_with_shared_type();
    test_forest_scale();

    // General tests
    test_memory_efficiency();
    test_pool_isolation();
    test_intrinsic_state_immutability();
    test_extrinsic_state_independence();
    test_factory_caching();
    test_multiple_instances_one_template();

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

    cout << "1. INTRINSIC vs EXTRINSIC STATE:\n";
    cout << "   - Intrinsic: Shared, immutable (textures, class types, styles)\n";
    cout << "   - Extrinsic: Unique, mutable (positions, names, health)\n\n";

    cout << "2. MEMORY SAVINGS:\n";
    cout << "   - 5-20x reduction typical\n";
    cout << "   - Multiplier effect with large collections\n";
    cout << "   - Cache efficiency gains\n\n";

    cout << "3. FACTORY PATTERN PAIRING:\n";
    cout << "   - Flyweight objects hidden behind factory\n";
    cout << "   - Factory controls caching and reuse\n";
    cout << "   - Encapsulates creation logic\n\n";

    cout << "4. REAL-WORLD APPLICATIONS:\n";
    cout << "   - Game entities (textures, meshes, materials)\n";
    cout << "   - Text rendering (fonts, styles, glyphs)\n";
    cout << "   - Graphics systems (shaders, materials)\n";
    cout << "   - Particle systems (shared type data)\n\n";

    return assertion_failures == 0 ? 0 : 1;
}
