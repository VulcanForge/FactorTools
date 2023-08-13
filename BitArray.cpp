#include "BitArray.h"

BitArray::BitArray (size_t count, bool defaultValue)
    : count (count)
{
    size_t storageCount = (count + 63) / 64;
    uint64_t defaultIntValue = UINT64_MAX * uint64_t (defaultValue);
    storage = std::vector<uint64_t> (storageCount, defaultIntValue);
}

bool BitArray::Get (size_t index) const
{
    size_t storageIndex = index / 64;
    size_t internalIndex = index % 64;
    uint64_t mask = 1ULL << internalIndex;
    return bool (storage[storageIndex] & mask);
}

void BitArray::Set (size_t index)
{
    size_t storageIndex = index / 64;
    size_t internalIndex = index % 64;    
    uint64_t valueMask = 1ULL << internalIndex;
    storage[storageIndex] |= valueMask;
}

void BitArray::Reset (size_t index)
{
    size_t storageIndex = index / 64;
    size_t internalIndex = index % 64;
    uint64_t valueMask = ~(1ULL << internalIndex);
    storage[storageIndex] &= valueMask;
}

size_t BitArray::Count () const
{
    return count;
}
