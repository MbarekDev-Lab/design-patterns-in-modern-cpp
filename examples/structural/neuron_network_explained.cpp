#pragma once
#include <cstdio>
#include <vector>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// FORWARD DECLARATION
// ============================================================================
struct Neuron; // Tell compiler that Neuron exists (defined later)

// ============================================================================
// TEMPLATE MIXIN: SomeNeurons
// ============================================================================
// This is a template mixin that provides the connect_to() method.
// It uses CRTP (Curiously Recurring Template Pattern) for static polymorphism.
// The Self type parameter will be either Neuron or NeuronLayer.
// ============================================================================
template <typename Self>
struct SomeNeurons
{
    // Generic connect_to template that works with any type T
    // T must be iterable (have begin() and end() methods) and contain Neurons
    template <typename T>
    void connect_to(T &other)
    {
        // Step 1: Cast 'this' to Self* (the derived type)
        // Example: if Self=Neuron, convert SomeNeurons<Neuron>* to Neuron*
        Self *self = static_cast<Self *>(this);

        // Step 2: Dereference to iterate: *self either:
        //         - Returns a single Neuron (if Self=Neuron via begin()/end())
        //         - Returns NeuronLayer which is a std::vector<Neuron>
        for (Neuron &from : *self) // Neuron we're connecting FROM
        {
            // Step 3: Iterate through target (other) - could be Neuron or NeuronLayer
            for (Neuron &to : other) // Neuron we're connecting TO
            {
                // Step 4: Create bidirectional connection
                from.out.push_back(&to); // from -> to
                to.in.push_back(&from);  // to <- from
            }
        }
    }
};

// ============================================================================
// NEURON STRUCT
// ============================================================================
// Represents a single neuron with input and output connections.
// Inherits from SomeNeurons<Neuron> to get connect_to() method.
// ============================================================================
struct Neuron : SomeNeurons<Neuron>
{
    vector<Neuron *> in;  // Pointers to neurons sending signals TO this neuron
    vector<Neuron *> out; // Pointers to neurons this neuron sends signals TO
    unsigned int id;      // Unique identifier for this neuron

    // Constructor: assigns a unique incrementing ID to each neuron
    Neuron()
    {
        static int id_counter = 1; // Shared counter across all Neuron instances
        this->id = id_counter++;   // Assign and increment
    }

    // These functions make a Neuron appear as an iterable container with one element
    // This allows: for (Neuron& n : single_neuron) to work
    // Returns a pointer to this Neuron (start of iteration)
    Neuron *begin() { return this; }
    // Returns a pointer one-past this Neuron (end of iteration)
    Neuron *end() { return this + 1; }

    // Overload << operator to print neuron connection info
    friend ostream &operator<<(ostream &os, const Neuron &obj)
    {
        // Print all incoming connections
        for (Neuron *n : obj.in)
        {
            os << n->id << "\t-->\t[" << obj.id << "]" << endl;
        }

        // Print all outgoing connections
        for (Neuron *n : obj.out)
        {
            os << "[" << obj.id << "]\t-->\t" << n->id << endl;
        }
        return os;
    }
};

// ============================================================================
// NEURONLAYER STRUCT
// ============================================================================
// A container of multiple neurons that acts like a single neuron.
// Inherits from std::vector<Neuron> (IS-A relationship)
// Inherits from SomeNeurons<NeuronLayer> to get connect_to() method.
// This is the COMPOSITE pattern: treat individual and groups the same way.
// ============================================================================
struct NeuronLayer : vector<Neuron>, SomeNeurons<NeuronLayer>
{
    // Constructor: creates 'count' neurons and adds them to this layer
    explicit NeuronLayer(int count)
    {
        // emplace_back creates Neuron in-place using Neuron() constructor
        while (count-- > 0)
            emplace_back(Neuron{});
    }

    // Overload << operator to print all neurons in this layer
    friend ostream &operator<<(ostream &os, NeuronLayer &obj)
    {
        // obj is both a vector and a NeuronLayer, iterate through all neurons
        for (auto &n : obj)
            os << n; // Use Neuron's operator<< to print each neuron
        return os;
    }
};

// ============================================================================
// MAIN PROGRAM
// ============================================================================
// Demonstrates the Composite pattern:
// - Single neurons can connect to other neurons
// - Layers (groups of neurons) can connect to single neurons
// - Layers can connect to other layers
// All use the SAME connect_to() interface!
// ============================================================================
int main()
{
    // === EXAMPLE 1: Neuron to Neuron ===
    cout << "=== EXAMPLE 1: Single Neuron to Single Neuron ===" << endl;
    Neuron n1, n2;     // Create two neurons (id: 1 and 2)
    n1.connect_to(n2); // Connect n1 -> n2

    cout << n1 << n2 << endl;

    // === EXAMPLE 2: Layer to Single Neuron ===
    cout << "=== EXAMPLE 2: Layer to Single Neuron ===" << endl;
    NeuronLayer l1{5}; // Create layer with 5 neurons (id: 3, 4, 5, 6, 7)
    Neuron n3;         // Create single neuron (id: 8)
    l1.connect_to(n3); // Connect all neurons in l1 to n3

    cout << "Neuron " << n3.id << endl
         << n3 << endl;
    cout << "Layer " << endl
         << l1 << endl;

    // === EXAMPLE 3: Layer to Layer ===
    cout << "=== EXAMPLE 3: Layer to Layer ===" << endl;
    NeuronLayer l2{2}, l3{3}; // Create two layers
    l2.connect_to(l3);        // Connect all neurons in l2 to all neurons in l3
    cout << "Layer l2" << endl
         << l2;
    cout << "Layer l3" << endl
         << l3;

    return 0;
}
