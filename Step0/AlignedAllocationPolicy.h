//
// Created by lcy20 on 2019-06-18.
//

#ifndef STEP0_ALIGNEDALLOCATIONPOLICY_H
#define STEP0_ALIGNEDALLOCATIONPOLICY_H

template<size_t N>
class AlignedAllocationPolicy
{
public:
    static void* operator new(size_t size)
    {
        return _aligned_malloc(size, N);
    }

    static void operator delete(void* memory)
    {
        _aligned_free(memory);
    }
};

#endif //STEP0_ALIGNEDALLOCATIONPOLICY_H
