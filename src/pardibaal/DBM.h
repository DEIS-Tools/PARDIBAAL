/*
 * Copyright Thomas M. Grosen
 * Created on 13/10/2021.
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

#ifndef PARDIBAAL_DBM_H
#define PARDIBAAL_DBM_H

#include <vector>
#include <ostream>

#include "bound_t.h"
#include "bounds_table_t.h"

namespace pardibaal {
    class Federation;
    /** Relation struct
     * represents the relation between two DBMs.
     * different means that they are neither equal or a sub/superset of each other, or the dimensions are different.
     */
    struct relation_t {
        bool _equal, _subset, _superset, _different;
        relation_t(bool equal, bool subset, bool superset, bool different) :
            _equal(equal), _subset(subset), _superset(superset), _different(different) {}

        [[nodiscard]] static relation_t equal();
        [[nodiscard]] static relation_t subset();
        [[nodiscard]] static relation_t superset();
        [[nodiscard]] static relation_t different();
    };

    class DBM {
        bounds_table_t _bounds_table;

    public:
        DBM(dim_t number_of_clocks);

        static DBM zero(dim_t dimension);

        static DBM unconstrained(dim_t dimension);

        [[nodiscard]] bound_t at(dim_t i, dim_t j) const;
        void set(dim_t i, dim_t j, bound_t bound);
        [[nodiscard]] dim_t dimension() const;

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_satisfied(dim_t x, dim_t y, bound_t g) const;
        [[nodiscard]] relation_t relation(const DBM& dbm) const;
        [[nodiscard]] relation_t relation(const Federation& fed) const;

        [[nodiscard]] bool equal(const DBM& dbm) const;
        [[nodiscard]] bool equal(const Federation& fed) const;
        [[nodiscard]] bool subset(const DBM& dbm) const;
        [[nodiscard]] bool subset(const Federation& fed) const;
        [[nodiscard]] bool superset(const DBM& dbm) const;
        [[nodiscard]] bool superset(const Federation& fed) const;

        /**
         * Checks if two DBMs intersect ie. if the intersection is non-empty
         * @return true if this intersects with dbm
         */
        [[nodiscard]] bool intersects(const DBM& dbm) const;

        /**
         * Checks if a DBM intersect with a federation ie. if the intersection is non-empty
         * @return true if this intersects with fed
         */
        [[nodiscard]] bool intersects(const Federation& fed) const;

        /** Is bounded
         * Checks whether all upper bounds are infinite
         * @return true if the DBM is unbounded (has no upper bound)
         */
        [[nodiscard]] bool is_unbounded() const;

        void close();

        void future();
        void past();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);

        void extrapolate(const std::vector<val_t> &ceiling);

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
        void extrapolate_diagonal(const std::vector<val_t> &ceiling);

        void extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper);

        void extrapolate_lu_diagonal(const std::vector<val_t> &lower, const std::vector<val_t> &upper);

        /**
         * Sets this to the intersection between this and dbm
         * this = this intersect dbm
         * @param dbm intersected with
         */
        void intersection(const DBM& dbm);

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

#endif //PARDIBAAL_DBM_H
