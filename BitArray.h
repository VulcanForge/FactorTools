#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

// A densely packed bit array.
class BitArray
{
private:
    // The underlying storage.
    std::vector<uint64_t> storage;

    // The number of bits stored.
    size_t count;

public:
    // Constructs a BitArray with given size and default values.
    BitArray (size_t count, bool defaultValue);

    // Gets the bit at a given index.
    bool Get (size_t index) const;

    // Sets true the bit at a given index.
    void Set (size_t index);

    // Sets false the bit at a given index.
    void Reset (size_t index);

    // Returns the number of bits stored.
    size_t Count () const;
};
