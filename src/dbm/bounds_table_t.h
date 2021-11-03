//
// Created by Thomas Grosen on 21/10/2021.
//

#ifndef BDM_BOUNDS_TABLE_T_H
#define BDM_BOUNDS_TABLE_T_H

#include "bound_t.h"
#include "DBM.h"

#include <vector>


namespace dbm {
    typedef uint32_t dim_t;

    struct bounds_table_t {
    public:
        const int _number_of_clocks;

        bounds_table_t(dim_t number_of_clocks);

        bound_t &get(dim_t i, dim_t j);

        bound_t at(dim_t i, dim_t j) const;

    private:
        std::vector<bound_t> _bounds;
    };
}

#endif //BDM_BOUNDS_TABLE_T_H
