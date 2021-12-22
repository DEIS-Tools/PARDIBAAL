/*
 * Copyright Thomas Møller Grosen
 * Created on 21/10/2021.
 */

/*
 * This file is part of dbm2
 *
 * dbm2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dbm2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with dbm2.  If not, see <https://www.gnu.org/licenses/>.
 */

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

        friend std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);

    private:
        std::vector<bound_t> _bounds;
    };

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);
}

#endif //BDM_BOUNDS_TABLE_T_H
