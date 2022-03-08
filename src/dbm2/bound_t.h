/*
 * Copyright Thomas M. Grosen
 * Created on 26/10/2021.
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

#ifndef BDM_BOUND_T_H
#define BDM_BOUND_T_H

#include <cstdlib>
#include <cinttypes>
#include <ostream>

namespace dbm2{
    using dim_t = uint32_t;
    using val_t = int32_t;

    enum strict_t {STRICT, NON_STRICT};

    struct bound_t {
        val_t _n = 0;
        bool _strict = false,
             _inf = false;

        bound_t(){};
        bound_t(val_t n, strict_t strictness);
        bound_t(val_t n, bool strict);

        static bound_t strict(val_t n);
        static bound_t non_strict(val_t n);
        static bound_t inf();
        static bound_t zero();


        static const bound_t& max(const bound_t &a, const bound_t &b);
        static const bound_t max(bound_t &&a, bound_t &&b);
        static const bound_t max(const bound_t &a, bound_t &&b);
        static const bound_t max(bound_t &&a, const bound_t &b);

        static const bound_t& min(const bound_t &a, const bound_t &b);
        static const bound_t min(bound_t &&a, bound_t &&b);
        static const bound_t min(const bound_t &a, bound_t &&b);
        static const bound_t min(bound_t &&a, const bound_t &b);

        const bound_t operator+(bound_t rhs) const;
        const bound_t operator+(val_t rhs) const;

        const bound_t operator*(val_t rhs) const;

        bool operator<=(bound_t rhs) const;

        bool operator<(bound_t rhs) const;

        bool operator==(bound_t rhs) const;

        bool operator!=(bound_t rhs) const;

        bool operator>(bound_t rhs) const;

        bool operator>=(bound_t rhs) const;

        friend std::ostream& operator<<(std::ostream& out, const bound_t& bound);

        inline static bool lt(bound_t lhs, bound_t rhs) {return lhs < rhs;}
        inline static bool le(bound_t lhs, bound_t rhs) {return lhs <= rhs;}
        inline static bool gt(bound_t lhs, bound_t rhs) {return lhs > rhs;}
        inline static bool ge(bound_t lhs, bound_t rhs) {return lhs >= rhs;}
    };

    const bound_t operator+(val_t val, const bound_t bound);
    const bound_t operator*(val_t val, const bound_t bound);
    std::ostream& operator<<(std::ostream& out, const bound_t& bound);
}

#endif //BDM_BOUND_T_H
