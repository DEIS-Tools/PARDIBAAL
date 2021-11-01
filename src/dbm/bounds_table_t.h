//
// Created by Thomas Grosen on 21/10/2021.
//

#ifndef BDM_BOUNDS_TABLE_T_H
#define BDM_BOUNDS_TABLE_T_H

#include "bound_t.h"

#include <vector>

namespace dbm {

    struct bounds_table_t {
    public:
        const int _number_of_clocks;

        bounds_table_t(int size);

        bound_t &get(int i, int j);

        bound_t at(int i, int j) const;

    private:
        std::vector<bound_t> _bounds;
    };
}

#endif //BDM_BOUNDS_TABLE_T_H
