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
        const dim_t _number_of_clocks;

        explicit bounds_table_t(dim_t number_of_clocks);

        bound_t &get(dim_t i, dim_t j);

        [[nodiscard]] bound_t at(dim_t i, dim_t j) const;

    private:
        std::vector<bound_t> _bounds;
    };
}

#endif //BDM_BOUNDS_TABLE_T_H
