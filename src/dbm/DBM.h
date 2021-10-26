//
// Created by Thomas Grosen on 13/10/2021.
//

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"
#include "bound_t.h"

#include <vector>

namespace dbm {

    // Well seems like vectors are way faster.. Or maybe I just suck at pointers
    class DBMPtr {
    public:
        bounds_table_ptr_t _bounds_table;

        DBMPtr(const int &size);

        DBMPtr(int &&size);

        static DBMPtr *gen_random(int &&size, int &&max);

        bool is_empty();
        void close_p();
        void close_val();
    };

    class DBM {
    public:
        bounds_table_t _bounds_table;

        DBM(const int &size);

        DBM(int &&size);

        static DBM gen_random(int &&size, int &&max);

        bool is_empty() const;
        bool relation(const DBM &d) const;
        bool satisfy(const bound_t &g) const;

        void close();

        // Preserves the canonical form
        void delay();
        void restrict(const bound_t &g);
        void free(const int &i);
        void reset(const int &i, const int &k);
        void copy(const int &i, const int &j);




    };

}

#endif //BDM_DBM_H
