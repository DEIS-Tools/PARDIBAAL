/*
 * Copyright Thomas M. Grosen
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


namespace dbm2 {

    struct bounds_table_t {
    public:
        dim_t _number_of_clocks;//TODO: write getter method

        explicit bounds_table_t(dim_t number_of_clocks);

        bound_t &at(dim_t i, dim_t j);

        [[nodiscard]] bound_t at(dim_t i, dim_t j) const;

        /** Remove clock at index c
         *
         * @param c index of clock to be removed
         */
        void remove_clock(dim_t c);

        /** Swaps index of clock a and b
         *
         * All bounds of a and b will be relocated to match the new index.
         * @param a index of clock to be swapped with b
         * @param b index of clock to be swapped with a
         */
        void swap_clocks(dim_t a, dim_t b);

        /** Add a new clock at c.
         *
         * All clocks at index i >= c will be at i + 1 afterwards.
         * All bounds on the new clock are (<, 0).
         * @param c is the index of the new clock
         */
        void add_clock_at(dim_t c);

        friend std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);

    private:
        std::vector<bound_t> _bounds;
    };

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);
}

#endif //BDM_BOUNDS_TABLE_T_H
