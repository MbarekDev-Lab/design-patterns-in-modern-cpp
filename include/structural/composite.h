#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>

struct GraphicObject
{
    virtual ~GraphicObject() = default;
    virtual void draw() = 0;
};

struct Circle : GraphicObject
{
    void draw() override
    {
        std::cout << "Circle" << std::endl;
    }
};

struct Group : GraphicObject
{
    std::string name;

    explicit Group(const std::string &name)
        : name{name}
    {
    }

    ~Group() override = default;

    void draw() override
    {
        /*
            & = lvalue reference (reference to an lvalue)
            && = rvalue reference (reference to an rvalue)
            auto && = forwarding reference (universal reference that can bind to both)

                // Best: const reference (no copying)
            for (const auto& o : objects)
                o->draw();

            // Alternative: simple copy (shared_ptr copies are cheap)
            for (auto o : objects)
                o->draw();
        */

        std::cout << "Group " << name << " contains:" << std::endl;
        for (auto &&o : objects)
            o->draw();
    }

    std::vector<std::shared_ptr<GraphicObject>> objects;
};
