/*
 * Copyright Thomas M. Grosen
 * Created on 13/10/2021.
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

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"

namespace dbm2 {
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
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);
        void norm(const std::vector<val_t> &ceiling);

        /** Diagonal extrapolation
         *
         * Update dbm[i,j] with
         * - infinity if dbm[i,j] > max_xi
         * - infinity if -dbm[0,i] > max_xi
         * - infinity if -dbm[0,j] > max_xj, i != 0
         * - <-max_xj if -dbm[i,j] > max_xj, i == 0
         * - dbm[i,j] otherwise
         * except for when i = j
         * also make sure 0, j is not positive and i, 0 is not negative
         *
         * @param dbm: DBM.
         * @param dim: dimension.
         * @param max: table of maximal constants.
         * @pre
         * - DBM is closed and non empty
         * - max is a int32_t[dim]
         * - max[0] = 0 (reference clock)
         * @post DBM is closed.
         */
         void diagonal_extrapolation(const std::vector<val_t> &ceiling);

        /**
         * Resize the DBM by adding and deleting clocks.
         * The number of true values in src and dst must be equal.
         * False in src means a clock is removed. False in dst means
         * a new clock is added.
         *
         * @param src_bits Vector of which clocks that are kept
         * @param dst_bits Vector of where clocks are added
         * @return indirection table of moved clocks
         */
        std::vector<dim_t> resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits);

        /**
         * Reorder the all clocks i to order[i].
         * If order[i] is max uint32 value, then the clock is removed
         *
         * @param order Vector of ordering
         * @param new_size Number of clocks (including zero)
         */
        void reorder(std::vector<dim_t> order, dim_t new_size);

        friend std::ostream& operator<<(std::ostream& out, const DBM& D);
    };

    std::ostream& operator<<(std::ostream& out, const DBM& D);

}

#endif //BDM_DBM_H
