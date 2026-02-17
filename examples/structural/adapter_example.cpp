#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <structural/adapter.h>

using namespace std;
using namespace adapter_solution;

// ============================================================================
// EXAMPLE CLASSES
// ============================================================================

// Example 1: Database adapter
class OldDatabase
{
public:
    OldDatabase(const string &name) : name_(name) {}

    string query_by_id(int id) const
    {
        return "ID:" + to_string(id);
    }

    string query_by_name(const string &n) const
    {
        return "Name:" + n;
    }

private:
    string name_;
};

// New interface expects different method names
class DatabaseAdapter
{
public:
    DatabaseAdapter(const shared_ptr<OldDatabase> &db) : db_(db) {}

    string execute(const string &type, const string &param) const
    {
        if (type == "id")
            return db_->query_by_id(stoi(param));
        else if (type == "name")
            return db_->query_by_name(param);
        return "Unknown";
    }

private:
    shared_ptr<OldDatabase> db_;
};

// Example 2: File format adapter
class ImageFormat
{
public:
    virtual ~ImageFormat() = default;
    virtual void load() = 0;
    virtual void save() = 0;
};

class JPEGImage : public ImageFormat
{
public:
    void load() override { cout << "  Loading JPEG...\n"; }
    void save() override { cout << "  Saving JPEG...\n"; }
};

// Legacy format with different interface
class LegacyBMPImage
{
public:
    void read() { cout << "  Reading legacy BMP...\n"; }
    void write() { cout << "  Writing legacy BMP...\n"; }
};

// Adapter to make legacy BMP work with modern interface
class BMPImageAdapter : public ImageFormat
{
public:
    BMPImageAdapter(const shared_ptr<LegacyBMPImage> &bmp) : bmp_(bmp) {}

    void load() override { bmp_->read(); }
    void save() override { bmp_->write(); }

private:
    shared_ptr<LegacyBMPImage> bmp_;
};

// Example 3: Voltage adapter (real world!)
class Voltage110V
{
public:
    int get_voltage() const { return 110; }
};

class Voltage220V
{
public:
    int get_voltage() const { return 220; }
};

// Device expecting 110V
class Device110V
{
public:
    void plug_in(const Voltage110V &v)
    {
        if (v.get_voltage() != 110)
            cout << "  Error: Need 110V\n";
        else
            cout << "  Device plugged in (110V)\n";
    }
};

// Adapter to step down 220V to 110V
class VoltageAdapter
{
public:
    VoltageAdapter(const Voltage220V &v) : main_voltage_(v) {}

    Voltage110V adapt()
    {
        // Step down: 220V -> 110V
        return Voltage110V();
    }

private:
    const Voltage220V &main_voltage_;
};

// ============================================================================
// MAIN EXAMPLES
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              ADAPTER PATTERN                               ║\n";
    cout << "║  Makes incompatible interfaces work together               ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    // ========================================================================
    // EXAMPLE 1: Class Adapter - Legacy Shape Integration
    // ========================================================================
    {
        cout << "========== EXAMPLE 1: Class Adapter (Shapes) ==========\n";
        cout << "Adapting legacy Square interface to modern Shape interface:\n\n";

        vector<shared_ptr<Shape>> shapes;

        // Add modern rectangle
        shapes.push_back(make_shared<Rectangle>(100, 50));

        // Add legacy square through adapter
        auto legacy_square = make_shared<LegacySquare>(75);
        shapes.push_back(make_shared<SquareAdapter>(legacy_square));

        // All shapes use unified interface
        cout << "Rendering all shapes:\n";
        for (auto &shape : shapes)
        {
            shape->render();
            cout << "  Width: " << shape->get_width()
                 << ", Height: " << shape->get_height() << "\n";
        }

        cout << "\nBenefit: Legacy code can now work with new interface!\n\n";
    }

    // ========================================================================
    // EXAMPLE 2: Object Adapter - Line to Points
    // ========================================================================
    {
        cout << "========== EXAMPLE 2: Object Adapter (Line to Points) ==========\n";
        cout << "Converting Line structure to Point collection:\n\n";

        Line line1{{10, 10}, {10, 20}}; // Vertical line
        Line line2{{5, 15}, {15, 15}};  // Horizontal line

        cout << "Line 1 (vertical):\n";
        LineToPointAdapter adapter1(line1);
        cout << "  Points: " << adapter1.get_points().size() << " points\n";

        cout << "Line 2 (horizontal):\n";
        LineToPointAdapter adapter2(line2);
        cout << "  Points: " << adapter2.get_points().size() << " points\n";

        cout << "\nBenefit: Convert between incompatible data structures!\n\n";
    }

    // ========================================================================
    // EXAMPLE 3: Caching Adapter
    // ========================================================================
    {
        cout << "========== EXAMPLE 3: Caching Adapter ==========\n";
        cout << "Adapter with caching to avoid redundant conversions:\n\n";

        Line line{{0, 0}, {0, 5}};

        cout << "First adapter (computes):\n";
        {
            CachingLineToPointAdapter adapter1(line);
            cout << "  Got " << adapter1.get_points().size() << " points\n";
        }

        cout << "\nSecond adapter (uses cache):\n";
        {
            CachingLineToPointAdapter adapter2(line);
            cout << "  Got " << adapter2.get_points().size() << " points\n";
        }

        cout << "\nBenefit: Cache prevents expensive conversions!\n\n";
    }

    // ========================================================================
    // EXAMPLE 4: Database Adapter
    // ========================================================================
    {
        cout << "========== EXAMPLE 4: Database Adapter ==========\n";
        cout << "Adapting old database interface to new one:\n\n";

        auto old_db = make_shared<OldDatabase>("users");
        DatabaseAdapter adapter(old_db);

        cout << "Old interface:\n";
        cout << "  query_by_id(1): " << old_db->query_by_id(1) << "\n";
        cout << "  query_by_name(\"Alice\"): " << old_db->query_by_name("Alice") << "\n";

        cout << "\nNew unified interface through adapter:\n";
        cout << "  execute(\"id\", \"1\"): " << adapter.execute("id", "1") << "\n";
        cout << "  execute(\"name\", \"Alice\"): " << adapter.execute("name", "Alice") << "\n";

        cout << "\nBenefit: Old database works with new interface!\n\n";
    }

    // ========================================================================
    // EXAMPLE 5: File Format Adapter
    // ========================================================================
    {
        cout << "========== EXAMPLE 5: File Format Adapter ==========\n";
        cout << "Adapting legacy image format to modern interface:\n\n";

        vector<shared_ptr<ImageFormat>> images;

        // Modern JPEG
        images.push_back(make_shared<JPEGImage>());

        // Legacy BMP through adapter
        auto legacy_bmp = make_shared<LegacyBMPImage>();
        images.push_back(make_shared<BMPImageAdapter>(legacy_bmp));

        cout << "Loading images (different implementations, same interface):\n";
        for (auto &img : images)
        {
            img->load();
        }

        cout << "\nSaving images:\n";
        for (auto &img : images)
        {
            img->save();
        }

        cout << "\nBenefit: Legacy formats work with modern image pipeline!\n\n";
    }

    // ========================================================================
    // EXAMPLE 6: Real-World - Voltage Adapter
    // ========================================================================
    {
        cout << "========== EXAMPLE 6: Real-world Voltage Adapter ==========\n";
        cout << "Adapting power supply for different voltage devices:\n\n";

        Voltage220V main_power;
        cout << "Main power supply: " << main_power.get_voltage() << "V\n";

        Device110V device;
        cout << "Device requires: 110V\n\n";

        cout << "Without adapter:\n";
        cout << "  Cannot plug device directly into 220V!\n\n";

        cout << "With voltage adapter:\n";
        VoltageAdapter adapter(main_power);
        Voltage110V adapted_voltage = adapter.adapt();
        device.plug_in(adapted_voltage);

        cout << "\nBenefit: Adapts incompatible power supplies!\n\n";
    }

    // ========================================================================
    // EXAMPLE 7: Bidirectional Adapter
    // ========================================================================
    {
        cout << "========== EXAMPLE 7: Bidirectional Adapter ==========\n";
        cout << "Adapter that works both directions (Line <-> Points):\n\n";

        Line original_line{{3, 3}, {3, 7}};

        cout << "Adapting Line to Points:\n";
        BidirectionalAdapter adapter1(original_line);
        cout << "  Points: " << adapter1.get_points().size() << "\n";

        cout << "\nAdapting Points back to Line:\n";
        PointSet points{{5, 1}, {5, 2}, {5, 3}};
        BidirectionalAdapter adapter2(points);
        cout << "  Line: (" << adapter2.get_line().start.x << ","
             << adapter2.get_line().start.y << ") -> ("
             << adapter2.get_line().end.x << ","
             << adapter2.get_line().end.y << ")\n";

        cout << "\nBenefit: Single adapter works in both directions!\n\n";
    }

    // ========================================================================
    // KEY TAKEAWAYS
    // ========================================================================
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                           ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. Adapters bridge incompatible interfaces                 │\n";
    cout << "│ 2. Two approaches: Class (inheritance) & Object (compose)  │\n";
    cout << "│ 3. Allows legacy code to work with new systems             │\n";
    cout << "│ 4. Can add functionality (caching, logging, etc.)          │\n";
    cout << "│ 5. Useful for third-party library integration              │\n";
    cout << "│ 6. Appears in practice: drivers, converters, wrappers      │\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
