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
    /** Relation struct
     * represents the relation between two DBMs.
     * different means that they are neither equal or a sub/superset of each other, or the dimensions are different.
     */
    struct relation_t {
        bool _incomparable, _equal, _subset, _superset, _different;
        relation_t(bool equal, bool subset, bool superset, bool different) :
            _equal(equal), _subset(subset), _superset(superset), _different(different) {}

        static inline relation_t equal() {return relation_t(true, true, true, false);}
        static inline relation_t subset() {return relation_t(false, true, false, false);}
        static inline relation_t superset() {return relation_t(false, false, true, false);}
        static inline relation_t different() {return relation_t(false, false, false, true);}
    };

    class DBM {
        bounds_table_t _bounds_table;

    public:
        DBM(dim_t number_of_clocks);

        inline bound_t& at(dim_t i, dim_t j) {return this->_bounds_table.at(i, j);}
        inline bound_t at(dim_t i, dim_t j) const {return this->_bounds_table.at(i, j);}
        inline dim_t dimension() const {return this->_bounds_table.number_of_clocks();}

        bool is_empty() const;
        bool is_satisfied(dim_t x, dim_t y, bound_t g) const;
        relation_t relation(const DBM& dbm) const;

        inline bool equal(const DBM& dbm) const {return this->relation(dbm)._equal;}
        inline bool subset(const DBM& dbm) const {return this->relation(dbm)._subset;}
        inline bool superset(const DBM& dbm) const {return this->relation(dbm)._superset;}

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

#endif //BDM_DBM_H
