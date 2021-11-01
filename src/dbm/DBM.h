//
// Created by Thomas Grosen on 13/10/2021.
//

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"
#include "bound_t.h"

typedef uint32_t dim_t;

namespace dbm {

    class DBM {
    public:
        bounds_table_t _bounds_table;

        DBM(dim_t size);

        static DBM gen_random(int size, int max);

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
