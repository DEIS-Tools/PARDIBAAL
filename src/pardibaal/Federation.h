/*
 * Copyright Thomas M. Grosen
 * Created on 01/04/2022
 */

/*
 * This file is part of pardibaal
 *
 * pardibaal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pardibaal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with pardibaal. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef PARDIBAAL_FEDERATION_H
#define PARDIBAAL_FEDERATION_H

#include <vector>
#include <ostream>

#include "bound_t.h"
#include "DBM.h"

namespace pardibaal {

    class Federation {
        std::vector<DBM> zones;

        /**
         * Makes the federation consistent, by deleting all empty zones.
         * A federation is consistent if all zones are nonempty.
         */
        void make_consistent();

    public:
        // Creates an empty federation with no zones
        Federation();

        // Creates a federation with a single zone where all clocks are zero
        Federation(dim_t dimension);

        // Creates a federation with a given zone
        Federation(const DBM& dbm);

        // Returns a federation with single zero bounded dbm
        static Federation zero(dim_t dimension);

        // Returns a federation with a single unconstrained dbm
        static Federation unconstrained(dim_t dimension);

        auto begin() const;
        auto end() const;

        [[nodiscard]] const DBM& at(dim_t index) const;

        /**
         * Add a new dbm to the federation.
         * Is only added if it is not already included.
         * If the new dbm includes the federation,
         * then all other dbms are removed.
         * @param dbm The dbm that is added to the federation
         */
        void add(const DBM& dbm);
        void remove(dim_t index);

        /**
         * Number of DBMs stored in the federation
         *
         * @return Number of DBMs (zones) stored in the federation
         */
        [[nodiscard]] dim_t size() const;

        /**
         * The dimension of the DBMs in this federation.
         * Zero per default if the federation is empty.
         *
         * @return The number of clocks including the zero clock
         */
        [[nodiscard]] dim_t dimension() const;

        /**
         * @return true if the size is 0 or all zones are empty
         */
        [[nodiscard]] bool is_empty() const;

        /**
         * Checks whether a specific bound is satisfied by any of the zones.
         *
         * @param x, y Clock indexes where the bound is applied
         * @param g The bound (value and strictness) that is checked for
         * @return True if any of the zones satisfy the bound
         */
        [[nodiscard]] bool satisfies(dim_t x, dim_t y, bound_t g) const;

        /**
         * Relation between this and a dbm.
         * The relation is an under-approximation.
         * Can possibly return different, even though it is equal/subset/superset.
         * If it returns equal, subset, or superset it is always true.
         * @param dbm the rhs of the relation expression.
         * @return relation_t representing this in relation to dbm.
         */
        [[nodiscard]] relation_t relation(const DBM& dbm) const;

        /**
         * Relation between this and another federation.
         * The relation is an under-approximation.
         * If it returns different, it might actually be equal subset, or superset.
         * If it returns equal, subset, or superset it is always true.
         * @param fed the rhs (federation) of the relation expression.
         * @return relation_t representing this in relation to fed.
         */
        [[nodiscard]] relation_t relation(const Federation& fed) const;

        /**
         * Checks if this is equal to the dbm.
         * This is an under-approximation, see Federation::relation
         * @param dbm rhs of the equality.
         * @return true if the dbm and federation includes the same space.
         */
        [[nodiscard]] bool equal(const DBM& dbm) const;

        /**
         * Checks if the federations are equal.
         * This is an under-approximation, see Federation::relation
         * @param fed rhs of the equality.
         * @return true if the federations includes the same space.
         */
        [[nodiscard]] bool equal(const Federation& fed) const;

        /**
         * Checks if this is a subset of or included in the dbm.
         * This is an under-approximation, see Federation::relation
         * @param dbm rhs of the relation.
         * @return true if this is included in the dbm
         */
        [[nodiscard]] bool subset(const DBM& dbm) const;

        /**
         * Checks if this is a subset of or included in the federation.
         * This is an under-approximation, see Federation::relation
         * @param fed rhs of the relation.
         * @return true if this is included in fed
         */
        [[nodiscard]] bool subset(const Federation& fed) const;

        /**
         * Checks if this is a superset of or includes the dbm.
         * This is an under-approximation, see Federation::relation
         * @param dbm rhs of the relation.
         * @return true if this includes the dbm
         */
        [[nodiscard]] bool superset(const DBM& dbm) const;

        /**
         * Checks if this is a superset of or includes the federation.
         * This is an under-approximation, see Federation::relation
         * @param fed rhs of the relation.
         * @return true if this includes fed
         */
        [[nodiscard]] bool superset(const Federation& fed) const;

        /**
         * Checks if the any of the zones have upper bounds
         * @return true if one of the zones in the federation is unbounded
         */
        [[nodiscard]] bool is_unbounded() const;

        void future();
        void past();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);

        void extrapolate(const std::vector<val_t> &ceiling);
        void extrapolate_diagonal(const std::vector<val_t> &ceiling);
        void extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper);
        void extrapolate_lu_diagonal(const std::vector<val_t> &lower, const std::vector<val_t> &upper);

        void remove_clock(dim_t c);
        void swap_clocks(dim_t a, dim_t b);
        void add_clock_at(dim_t c);

        std::vector<dim_t> resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits);
        void reorder(std::vector<dim_t> order, dim_t new_size);

        friend std::ostream& operator<<(std::ostream& out, const Federation& fed);
    };

    std::ostream& operator<<(std::ostream& out, const Federation& fed);
}

#endif //PARDIBAAL_FEDERATION_H
