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

#include <cstdlib>
#include <cinttypes>
#include <ostream>

namespace pardibaal {
    using dim_t = uint32_t;
    using val_t = int32_t;

    enum strict_t {STRICT, NON_STRICT};

    struct bound_t {
    private:
        val_t _n = 0;
        bool _strict = false,
             _inf = false;
    public:
        bound_t(){};
        bound_t(val_t n, strict_t strictness);
        bound_t(val_t n, bool strict);

        [[nodiscard]] static bound_t strict(val_t n);
        [[nodiscard]] static bound_t non_strict(val_t n);
        [[nodiscard]] static bound_t inf();
        [[nodiscard]] static bound_t zero();

        [[nodiscard]] inline val_t get_bound() const {return this->_n;}
        [[nodiscard]] inline bool is_strict() const {return this->_strict;}
        [[nodiscard]] inline bool is_non_strict() const {return not this->_strict;}
        [[nodiscard]] inline bool is_inf() const {return this->_inf;}

        [[nodiscard]] static const bound_t& max(const bound_t &a, const bound_t &b);
        [[nodiscard]] static const bound_t max(bound_t &&a, bound_t &&b);
        [[nodiscard]] static const bound_t max(const bound_t &a, bound_t &&b);
        [[nodiscard]] static const bound_t max(bound_t &&a, const bound_t &b);

        [[nodiscard]] static const bound_t& min(const bound_t &a, const bound_t &b);
        [[nodiscard]] static const bound_t min(bound_t &&a, bound_t &&b);
        [[nodiscard]] static const bound_t min(const bound_t &a, bound_t &&b);
        [[nodiscard]] static const bound_t min(bound_t &&a, const bound_t &b);

        [[nodiscard]] const bound_t operator+(bound_t rhs) const;
        [[nodiscard]] const bound_t operator+(val_t rhs) const;

        [[nodiscard]] const bound_t operator*(val_t rhs) const;

        [[nodiscard]] bool operator<(bound_t rhs) const;
        [[nodiscard]] bool operator==(bound_t rhs) const;

        [[nodiscard]] bool operator!=(bound_t rhs) const;
        [[nodiscard]] bool operator>(bound_t rhs) const;
        [[nodiscard]] bool operator>=(bound_t rhs) const;
        [[nodiscard]] bool operator<=(bound_t rhs) const;

        friend std::ostream& operator<<(std::ostream& out, const bound_t& bound);

        [[nodiscard]] inline static bool lt(bound_t lhs, bound_t rhs) {return lhs < rhs;}
        [[nodiscard]] inline static bool le(bound_t lhs, bound_t rhs) {return lhs <= rhs;}
        [[nodiscard]] inline static bool gt(bound_t lhs, bound_t rhs) {return lhs > rhs;}
        [[nodiscard]] inline static bool ge(bound_t lhs, bound_t rhs) {return lhs >= rhs;}
    };

    [[nodiscard]] const bound_t operator+(val_t val, const bound_t bound);
    [[nodiscard]] const bound_t operator*(val_t val, const bound_t bound);
    std::ostream& operator<<(std::ostream& out, const bound_t& bound);
}

#endif //PARDIBAAL_BOUND_T_H
