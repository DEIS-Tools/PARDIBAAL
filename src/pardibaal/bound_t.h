/*
 * Copyright Thomas M. Grosen
 * Created on 26/10/2021.
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

#ifndef PARDIBAAL_BOUND_T_H
#define PARDIBAAL_BOUND_T_H

#include <ostream>
#include <cinttypes>
#include <limits>


namespace pardibaal {
    using dim_t = uint32_t;
    using val_t = int32_t;

    /**
     * Representing a bound in 32 bit:
     * Least significant bit is strictness, 1 means non strict and 0 means strict.
     * the 31 most significant bits are right shifted (preserving signed bit) and read as 32 bit signed int.
     * Infinite bound is represented as the largesst 31 bit value.
     * This means that the max limit for a bound value is a 32 bit int where the two most significant bits are 0
     */
    const int32_t INF_BOUND = std::numeric_limits<int32_t>::max();
    const int32_t NEG_INF_BOUND = std::numeric_limits<int32_t>::min();
    const int32_t LE_ZERO_BOUND = 1;
    const int32_t LT_ZERO_BOUND = 0;
    const int32_t BOUND_VAL_MAX = std::numeric_limits<int32_t>::max() >> 1;
    const int32_t BOUND_VAL_MIN = std::numeric_limits<int32_t>::min() >> 1;

    enum strict_e {STRICT = 0, NON_STRICT = 1};

    struct bound_t {

    private:


    public:
        val_t _data = LE_ZERO_BOUND;
        constexpr bound_t(val_t bound) : _data(bound) {}
        constexpr bound_t(){};
        constexpr bound_t(val_t n, strict_e strictness) {
            _data = (n << 1) | (val_t) strictness;
        }

        constexpr bound_t(val_t n, bool strict) {
            _data = (n << 1) | (val_t) !strict;
        }

        [[nodiscard]] static constexpr bound_t strict(val_t n)     {return bound_t(n << 1);}
        [[nodiscard]] static constexpr bound_t non_strict(val_t n) {return bound_t(~((~n) << 1));}
        [[nodiscard]] static constexpr bound_t inf()               {return bound_t(INF_BOUND);}
        [[nodiscard]] static constexpr bound_t neg_inf()           {return bound_t(NEG_INF_BOUND);}
        [[nodiscard]] static constexpr bound_t le_zero()           {return bound_t(LE_ZERO_BOUND);}
        [[nodiscard]] static constexpr bound_t lt_zero()           {return bound_t(LT_ZERO_BOUND);}

        [[nodiscard]] inline val_t get_bound()    const {return this->_data >> 1;}
        [[nodiscard]] inline bool is_strict()     const {return not this->is_non_strict();}
        [[nodiscard]] inline bool is_non_strict() const {return this->_data << 31;}
        [[nodiscard]] inline bool is_inf()        const {return this->_data == INF_BOUND;}
        [[nodiscard]] inline bool is_neg_inf()    const {return this->_data == NEG_INF_BOUND;}

        /** Checks if two opposite bounds overlap
         * The bounds MUST be on the form (i,j) and (j,i)
         * @return Bool whether the two opposite bounds are consistent with each other
         */
        [[nodiscard]] static inline bool is_overlapping(bound_t a, bound_t b) {
            return (a.get_bound() > -b.get_bound()) || ((a.get_bound() == -b.get_bound()) && (1 & a._data & b._data));
        }

        [[nodiscard]] static inline bound_t max(bound_t a, bound_t b) {return a < b ? b : a;}
        [[nodiscard]] static inline bound_t min(bound_t a, bound_t b) {return a < b ? a : b;}

        [[nodiscard]] bound_t operator+(bound_t rhs) const;
        [[nodiscard]] bound_t operator+(val_t rhs) const;

        [[nodiscard]] static bound_t finite_addition(const bound_t& rhs, const bound_t& lhs);

        [[nodiscard]] bound_t operator-(val_t rhs) const;

        [[nodiscard]] inline bool operator==(bound_t rhs) const {return this->_data == rhs._data;}
        [[nodiscard]] inline bool operator!=(bound_t rhs) const {return this->_data != rhs._data;}
        [[nodiscard]] inline bool operator<(bound_t rhs)  const {return this->_data < rhs._data;}
        [[nodiscard]] inline bool operator>(bound_t rhs)  const {return this->_data > rhs._data;}
        [[nodiscard]] inline bool operator>=(bound_t rhs) const {return this->_data >= rhs._data;}
        [[nodiscard]] inline bool operator<=(bound_t rhs) const {return this->_data <= rhs._data;}

        [[nodiscard]] bool operator==(val_t rhs) const;
        [[nodiscard]] bool operator!=(val_t rhs) const;
        [[nodiscard]] bool operator<(val_t rhs) const;
        [[nodiscard]] bool operator>(val_t rhs) const;
        [[nodiscard]] bool operator<=(val_t rhs) const;
        [[nodiscard]] bool operator>=(val_t rhs) const;

        friend std::ostream& operator<<(std::ostream& out, const bound_t& bound);

        [[nodiscard]] static bool is_lt(bound_t lhs, bound_t rhs) {return lhs < rhs;}
        [[nodiscard]] static bool is_le(bound_t lhs, bound_t rhs) {return lhs <= rhs;}
        [[nodiscard]] static bool is_gt(bound_t lhs, bound_t rhs) {return lhs > rhs;}
        [[nodiscard]] static bool is_ge(bound_t lhs, bound_t rhs) {return lhs >= rhs;}
    };

    [[nodiscard]] bound_t operator+(val_t val, bound_t bound);
    [[nodiscard]] bound_t operator-(val_t val, bound_t bound);
    std::ostream& operator<<(std::ostream& out, const bound_t& bound);
}

#endif //PARDIBAAL_BOUND_T_H
