#include "BitArray.h"

BitArray::BitArray (size_t count)
    : count (count)
{
    size_t storageCount = (count + 63) / 64;
    storage = std::vector<uint64_t> (storageCount);
}

BitArray::BitArray (size_t count, bool defaultValue)
    : count (count)
{
    size_t storageCount = (count + 63) / 64;
    uint64_t defaultIntValue = 0xFFFFFFFFFFFFFFFF * uint64_t (defaultValue);
    storage = std::vector<uint64_t> (storageCount, defaultIntValue);
}

bool BitArray::Get (size_t index) const
{
    size_t storageIndex = index / 64;
    size_t internalIndex = index % 64;
    uint64_t mask = 1ULL << internalIndex;
    return bool (storage[storageIndex] & mask);
}

void BitArray::Set (size_t index, bool value)
{
    size_t storageIndex = index / 64;
    size_t internalIndex = index % 64;
    uint64_t intValue = uint64_t (value);
    uint64_t clearMask = ~(1ULL << internalIndex);
    uint64_t valueMask = intValue << internalIndex;
    storage[storageIndex] = (storage[storageIndex] & clearMask) | valueMask;
}

size_t BitArray::Count () const
{
    return count;
}
