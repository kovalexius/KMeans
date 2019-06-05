#ifndef __DATA_STRUCTURES_H
#define __DATA_STRUCTURES_H

#include <stdint.h>
#include "locks.h"

namespace TSafe
{
    namespace
    {
        class bloom
        {
        public:
            void add_item(const uint64_t & item);
            void delete_item(const uint64_t & item);
            bool is_item(const uint64_t & item);
        private:
            uint64_t m_filter;
        };
    }

    template<typename T>
    class queue
    {
        struct listCell
        {
            listCell *m_next;
            T m_val;
        };

    public:
    };
}

#endif