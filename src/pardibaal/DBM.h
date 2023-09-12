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
#include "difference_bound_t.h"

namespace pardibaal {
    class Federation;
    
    enum relation_e {EQUAL, SUBSET, SUPERSET, DIFFERENT};
    /** 
     * Relation struct
     * represents the relation between two DBMs.
     * different means that they are not equal or a sub/superset of each other, or the dimensions are different.
     * Subset and superset is sharp (not equal)
     */
    struct relation_t {
    private:
        bool _is_equal, _is_subset, _is_superset, _is_different;
    
    public:
        constexpr relation_t(bool equal, bool subset, bool superset, bool different) :
            _is_equal(equal), _is_subset(subset), _is_superset(superset), _is_different(different) {}

        [[nodiscard]] static constexpr relation_t equal() {return relation_t(true, false, false, false);}
        [[nodiscard]] static constexpr relation_t subset() {return relation_t(false, true, false, false);}
        [[nodiscard]] static constexpr relation_t superset() {return relation_t(false, false, true, false);}
        [[nodiscard]] static constexpr relation_t different() {return relation_t(false, false, false, true);}

        [[nodiscard]] relation_e type() const;

        [[nodiscard]] bool is_equal() const;
        [[nodiscard]] bool is_subset() const;
        [[nodiscard]] bool is_superset() const;
        [[nodiscard]] bool is_different() const;
    };

    class DBM {
        enum empty_status_e {EMPTY, NON_EMPTY, UNKNOWN};

        bounds_table_t _bounds_table;
        mutable empty_status_e _empty_status = NON_EMPTY;
        mutable bool _is_closed = true;

    public:
        DBM(dim_t number_of_clocks);

        static DBM zero(dim_t dimension);

        static DBM unconstrained(dim_t dimension);

        [[nodiscard]] inline bound_t at(dim_t i, dim_t j) const { return this->_bounds_table.at(i, j); }

        inline void set(dim_t i, dim_t j, bound_t bound) {
            this->_bounds_table.set(i, j, bound);
            _is_closed = false;
            _empty_status = UNKNOWN;
        }

        inline void set(const difference_bound_t& constraint) {
            this->set(constraint._i, constraint._j, constraint._bound);
        }

        [[nodiscard]] inline empty_status_e empty_status() const { return _empty_status; }

        void subtract(dim_t i, dim_t j, bound_t bound);
        void subtract(difference_bound_t constraint);

        [[nodiscard]] dim_t dimension() const;

        [[nodiscard]] bool is_empty() const;

        [[nodiscard]] bool is_satisfying(dim_t x, dim_t y, bound_t g) const;
        [[nodiscard]] bool is_satisfying(const difference_bound_t& constraint) const;
        [[nodiscard]] bool is_satisfying(const std::vector<difference_bound_t>& constraints) const;

        /**
         * Relation between this dbm and another dbm.
         * This relation is always exact.
         * @param dbm the rhs of the relation expression.
         * @return relation_t representing this in relation to dbm
         */
        [[nodiscard]] relation_t relation(const DBM& dbm) const;

        /**
         * Relation between this dbm and a federation
         * Can be either exact (slow, default) or approximate (fast) see Federation::relation
         * @param fed The rhs of the relation
         * @return relation_t representing this in relation to fed
         */
        template<bool is_exact = true>
        [[nodiscard]] relation_t relation(const Federation& fed) const;

        [[nodiscard]] bool is_equal(const DBM& dbm) const;

        template<bool is_exact = true>
        [[nodiscard]] bool is_equal(const Federation& fed) const;

        [[nodiscard]] bool is_subset(const DBM& dbm) const;

        template<bool is_exact = true>
        [[nodiscard]] bool is_subset(const Federation& fed) const;
        
        [[nodiscard]] bool is_superset(const DBM& dbm) const;

        template<bool is_exact = true>
        [[nodiscard]] bool is_superset(const Federation& fed) const;

        [[nodiscard]] bool is_different(const DBM& dbm) const;

        template<bool is_exact = true>
        [[nodiscard]] bool is_different(const Federation& fed) const;

        [[nodiscard]] inline relation_t exact_relation(const Federation& fed) const {return this->relation<true>(fed);}
        [[nodiscard]] inline relation_t approx_relation(const Federation& fed) const {return this->relation<false>(fed);}

        [[nodiscard]] inline bool is_exact_equal(const Federation& fed) const {return this->is_equal<true>(fed);}
        [[nodiscard]] inline bool is_approx_equal(const Federation& fed) const {return this->is_equal<false>(fed);}

        [[nodiscard]] inline bool is_exact_subset(const Federation& fed) const {return this->is_subset<true>(fed);}
        [[nodiscard]] inline bool is_approx_subset(const Federation& fed) const {return this->is_subset<false>(fed);}

        [[nodiscard]] inline bool is_exact_superset(const Federation& fed) const {return this->is_superset<true>(fed);}
        [[nodiscard]] inline bool is_approx_superset(const Federation& fed) const {return this->is_superset<false>(fed);}

        [[nodiscard]] inline bool is_exact_different(const Federation& fed) const {return this->is_different<true>(fed);}
        [[nodiscard]] inline bool is_approx_different(const Federation& fed) const {return this->is_different<false>(fed);}

        /**
         * Checks if two DBMs intersect ie. if the intersection is non-empty
         * @return true if this intersects with dbm
         */
        [[nodiscard]] bool is_intersecting(const DBM& dbm) const;

        /**
         * Checks if a DBM intersect with a federation ie. if the intersection is non-empty
         * @return true if this intersects with fed
         */
        [[nodiscard]] bool is_intersecting(const Federation& fed) const;

        /** Is bounded
         * Checks whether all upper bounds are infinite
         * @return true if the DBM is unbounded (has no upper bound)
         */
        [[nodiscard]] bool is_unbounded() const;

        void close();

        /**
         * Also known as "up" raises all upper bounds to inf
         */
        void future();
        
        /**
         * Bounded future raises all upper bounds by d
         * @param d Value added to all upper bounds 
         */ 
        void future(val_t d);

        /**
         * Also known as "down", lowers all lower bounds to zero
         */
        void past();

        /**
         * A concrete delay. Effectively raises upper and lower bounds by d
         * @param d value all bounds are raised by
         */
        void delay(val_t d);

        /**
         * Delays an interval ammount. Effectively raises lower bounds by lower and upper bounds by upper
         * @param lower lower bound of interval
         * @param upper upper bound of interval
         */
        void interval_delay(val_t lower, val_t upper);

        void restrict(dim_t x, dim_t y, bound_t g);
        void restrict(const difference_bound_t& constraint);
        void restrict(const std::vector<difference_bound_t>& constraints);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);

        /**
         * sets all bounds above the ceiling to inf. 
         * @param ceiling Vector of clock maximum (ceiling) values
         */ 
        void extrapolate(const std::vector<val_t> &ceiling);

        /**
         * Read about this in 
         *   Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
         *   Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
         */
        void extrapolate_diagonal(const std::vector<val_t> &ceiling);

        /**
         * Read about this in 
         *   Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
         *   Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
         */
        void extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper);

        /**
         * Read about this in 
         *   Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
         *   Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
         */
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
        void reorder(const std::vector<dim_t>& order, dim_t new_size);

        friend std::ostream& operator<<(std::ostream& out, const DBM& D);
    };

    std::ostream& operator<<(std::ostream& out, const DBM& D);
}

#endif //PARDIBAAL_DBM_H
