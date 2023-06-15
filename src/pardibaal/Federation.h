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

#include "difference_bound_t.h"
#include "bound_t.h"
#include "DBM.h"

namespace pardibaal {

    class Federation {

        using zone_vector = std::vector<DBM>;

        zone_vector zones;

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

        [[nodiscard]] zone_vector::const_iterator begin() const;
        [[nodiscard]] zone_vector::const_iterator end() const;

        [[nodiscard]] const DBM& at(dim_t index) const;

        /**
         * Add a new dbm to the federation.
         * Is only added if it is not already included.
         * If the new dbm includes the federation,
         * then all other dbms are removed.
         * @param dbm The dbm that is added to the federation.
         */
        void add(const DBM& dbm);

        /**
         * Add a federation of DBMs to this federation.
         */
        void add(const Federation& fed);

        /**
         * Subtract everything that satisfies the constraint i - j </<= bound constraint
         */
        void subtract(dim_t i, dim_t j, bound_t bound);
        
        /**
         * Subtract everything that satisfies the difference constraint
         */
        void subtract(difference_bound_t constraint);

        /**
         * Subtract a dbm from this federation.
         * @param dbm to be subtracted.
         */
        void subtract(const DBM& dbm);

        /**
         * Subtract a federation from this federation.
         * @param fed The federation to be subtracted.
         */
        void subtract(const Federation& fed);

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
        [[nodiscard]] bool is_satisfying(dim_t x, dim_t y, bound_t g) const;

        [[nodiscard]] bool is_satisfying(const difference_bound_t& constraint) const;
        [[nodiscard]] bool is_satisfying(const std::vector<difference_bound_t>& constraints) const;

        /**
         * Relation between this and a dbm.
         * The non-exact relation is an under-approximation.
         * If it returns different, it might be equal subset, or superset.
         * The exact relation is computationally expensive.
         * @param fed the rhs (federation) of the relation expression.
         * @return relation_t representing this in relation to fed.
         */
        template<bool is_exact = true>
        [[nodiscard]] relation_t relation(const DBM& dbm) const;

        /**
         * Relation between this and another federation.
         * The non-exact relation is an under-approximation.
         * If it returns different, it might be equal subset, or superset.
         * The exact relation is computationally expensive.
         * @param fed the rhs (federation) of the relation expression.
         * @return relation_t representing this in relation to fed.
         */
        template<bool is_exact = true>
        [[nodiscard]] relation_t relation(const Federation& fed) const;

        /**
         * Checks if this is equal to the dbm.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param dbm rhs of the equality.
         * @return true if the dbm and federation includes the exact same space.
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_equal(const DBM& dbm) const;

        /**
         * Checks if the federations are equal.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param fed rhs of the equality.
         * @return true if the federations includes the exact same space.
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_equal(const Federation& fed) const;


        /**
         * Checks if this is a subset of or included in the dbm.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param dbm rhs of the relation.
         * @return true if this is included in the dbm
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_subset(const DBM& dbm) const;


        /**
         * Checks if this is a subset of or included in the federation.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param fed rhs of the relation.
         * @return true if this is included in fed
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_subset(const Federation& fed) const;


        /**
         * Checks if this is a superset of or includes the dbm.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param dbm rhs of the relation.
         * @return true if this includes the dbm
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_superset(const DBM& dbm) const;

        
        /**
         * Checks if this is a superset of or includes the federation.
         * The non-exact is an under-approximation, see Federation::relation<false>
         * @param fed rhs of the relation.
         * @return true if this includes fed
         */
        template<bool is_exact = true>
        [[nodiscard]] bool is_superset(const Federation& fed) const;

        [[nodiscard]] inline relation_t exact_relation(const DBM& dbm) const {return this->relation<true>(dbm);}
        [[nodiscard]] inline relation_t approx_relation(const DBM& dbm) const {return this->relation<false>(dbm);}
        [[nodiscard]] inline relation_t exact_relation(const Federation& fed) const {return this->relation<true>(fed);}
        [[nodiscard]] inline relation_t approx_relation(const Federation& fed) const {return this->relation<false>(fed);}

        [[nodiscard]] inline bool is_exact_equal(const DBM& dbm) const {return this->is_equal<true>(dbm);}
        [[nodiscard]] inline bool is_approx_equal(const DBM& dbm) const {return this->is_equal<false>(dbm);}
        [[nodiscard]] inline bool is_exact_equal(const Federation& fed) const {return this->is_equal<true>(fed);}
        [[nodiscard]] inline bool is_approx_equal(const Federation& fed) const {return this->is_equal<false>(fed);}

        [[nodiscard]] inline bool is_exact_subset(const DBM& dbm) const {return this->is_subset<true>(dbm);}
        [[nodiscard]] inline bool is_approx_subset(const DBM& dbm) const {return this->is_subset<false>(dbm);}
        [[nodiscard]] inline bool is_exact_subset(const Federation& fed) const {return this->is_subset<true>(fed);}
        [[nodiscard]] inline bool is_approx_subset(const Federation& fed) const {return this->is_subset<false>(fed);}

        [[nodiscard]] inline bool is_exact_superset(const DBM& dbm) const {return this->is_superset<true>(dbm);}
        [[nodiscard]] inline bool is_approx_superset(const DBM& dbm) const {return this->is_superset<false>(dbm);}
        [[nodiscard]] inline bool is_exact_superset(const Federation& fed) const {return this->is_superset<true>(fed);}
        [[nodiscard]] inline bool is_approx_superset(const Federation& fed) const {return this->is_superset<false>(fed);}

        /**
         * Checks if a federation intersect with a federation ie. if the intersection is non-empty
         * @return true if this intersects with dbm
         */
        [[nodiscard]] bool is_intersecting(const DBM& dbm) const;

        /**
         * Checks if a federation intersect with a DBM ie. if the intersection is non-empty
         * @return true if this intersects with fed
         */
        [[nodiscard]] bool is_intersecting(const Federation& fed) const;

        /**
         * Checks if the any of the zones have upper bounds
         * @return true if one of the zones in the federation is unbounded
         */
        [[nodiscard]] bool is_unbounded() const;

        void future();
        void future(val_t d);
        void past();

        void delay(val_t d);
        void interval_delay(val_t lower, val_t upper);

        void restrict(dim_t x, dim_t y, bound_t g);
        void restrict(const difference_bound_t& constraint);
        void restrict(const std::vector<difference_bound_t>& constraints);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);

        void extrapolate(const std::vector<val_t>& ceiling);
        void extrapolate_diagonal(const std::vector<val_t>& ceiling);
        void extrapolate_lu(const std::vector<val_t>& lower, const std::vector<val_t>& upper);
        void extrapolate_lu_diagonal(const std::vector<val_t>& lower, const std::vector<val_t>& upper);

        void intersection(const DBM& dbm);
        void intersection(const Federation& fed);

        void remove_clock(dim_t c);
        void swap_clocks(dim_t a, dim_t b);
        void add_clock_at(dim_t c);

        std::vector<dim_t> resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits);
        void reorder(const std::vector<dim_t>& order, dim_t new_size);

        const DBM& operator[](size_t i) const {
#ifndef NDEBUG
            return zones.at(i);
#else
            return zones[i];
#endif
        }

        friend std::ostream& operator<<(std::ostream& out, const Federation& fed);
    };

    std::ostream& operator<<(std::ostream& out, const Federation& fed);
}

#endif //PARDIBAAL_FEDERATION_H
