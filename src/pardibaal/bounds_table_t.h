/*
 * Copyright Thomas M. Grosen
 * Created on 21/10/2021.
 */

/*
 * This file is part of PARDIBAAL
 *
 * PARDIBAAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PARDIBAAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PARDIBAAL.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PARDIBAAL_BOUNDS_TABLE_T_H
#define PARDIBAAL_BOUNDS_TABLE_T_H

#include <vector>
#include <ostream>

#include "bound_t.h"


namespace pardibaal {
    struct bounds_table_t {
    public:
        explicit bounds_table_t(dim_t number_of_clocks);

        /** number of clock including the zero clock
         * same as the dimension of the "matrix"
         * @return number of clocks including the zero clock
         */
        [[nodiscard]] dim_t number_of_clocks() const;

        [[nodiscard]] inline bound_t at(dim_t i, dim_t j) const {
            return _bounds[i * _number_of_clocks + j];
        }

        inline void set(dim_t i, dim_t j, bound_t bound) { 
            this->_bounds[i * _number_of_clocks + j] = bound; 
        }

        friend std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);

    private:
        dim_t _number_of_clocks;
        std::vector<bound_t> _bounds;
    };

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);
}

#endif //PARDIBAAL_BOUNDS_TABLE_T_H
