#include "BitArray.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

/*
* Bits are stored in blocks of 32 (supposedly offering best performance), each block implemented as a 'uint32_t'.
* Bits can be set, reset, or retrieved within a given 'uint32_t' via bitmask methods.
*/

BitArray::BitArray (std::size_t count, bool defaultValue)
    : count (count)
{
    std::size_t storageCount = (count + 31) / 32;
    // All 0's if false or all 1's if true.
    std::uint32_t defaultIntValue = 0xFFFFFFFF * std::uint32_t (defaultValue);
    storage = std::vector<std::uint32_t> (storageCount, defaultIntValue);
}

bool BitArray::Get (std::size_t index) const
{
    // The index in 'storage' in which the desired bit lies.
    std::size_t storageIndex = index / 32;
    // The offset in 'storage[storageIndex]' at which the desired bit lies.
    std::size_t internalIndex = index % 32;
    // All 0's except a 1 at 'internalIndex'.
    std::uint32_t mask = 1 << internalIndex;
    // Extracts the value of the desired bit.
    return bool (storage[storageIndex] & mask);
}

void BitArray::Set (std::size_t index)
{
    // The index in 'storage' in which the desired bit lies.
    std::size_t storageIndex = index / 32;
    // The offset in 'storage[storageIndex]' at which the desired bit lies.
    std::size_t internalIndex = index % 32;
    // All 0's except a 1 at 'internalIndex'.
    std::uint32_t valueMask = 1U << internalIndex;
    // Sets the desired bit.
    storage[storageIndex] |= valueMask;
}

void BitArray::Reset (std::size_t index)
{
    // The index in 'storage' in which the desired bit lies.
    std::size_t storageIndex = index / 32;
    // The offset in 'storage[storageIndex]' at which the desired bit lies.
    std::size_t internalIndex = index % 32;
    // All 1's except a 0 at 'internalIndex'.
    std::uint32_t valueMask = ~(1U << internalIndex);
    // Resets the desired bit.
    storage[storageIndex] &= valueMask;
}

std::size_t BitArray::Count () const
{
    return count;
}
