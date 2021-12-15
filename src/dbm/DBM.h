//
// Created by Thomas Grosen on 13/10/2021.
//

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"

namespace dbm {
    class DBM {
    public:
        bounds_table_t _bounds_table;

        DBM(dim_t number_of_clocks);

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_included_in(const DBM &d) const;
        [[nodiscard]] bool is_satisfied(dim_t x, dim_t y, bound_t g) const;

        void close();

        void future();
        void past();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void assign(dim_t x, guard_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, guard_t n);
        void norm(const std::vector<guard_t> &ceiling);
    };

}

#endif //BDM_DBM_H
