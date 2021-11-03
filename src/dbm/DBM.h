//
// Created by Thomas Grosen on 13/10/2021.
//

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"
#include "bound_t.h"


namespace dbm {
    class DBM {
    public:
        bounds_table_t _bounds_table;

        DBM(dim_t number_of_clocks);

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_included_in(const DBM &d) const;

        void close();

        void delay();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void reset(dim_t x, dim_t m);
        void copy(dim_t x, dim_t y);

    };

}

#endif //BDM_DBM_H
