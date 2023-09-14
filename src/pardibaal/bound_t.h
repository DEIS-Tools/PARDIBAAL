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


namespace pardibaal {
    using dim_t = uint32_t;
    using val_t = int32_t;

    enum strict_e {STRICT, NON_STRICT};

    struct bound_t {
    private:
        val_t _n = 0;
        bool _strict = false,
             _inf = false;

        constexpr bound_t(val_t n, bool strict, bool inf) : _n(n), _strict(strict), _inf(inf){};
    public:
        constexpr bound_t(){};
        constexpr bound_t(val_t n, strict_e strictness) : _n(n) {_strict = strictness == STRICT ? true : false;}
        constexpr bound_t(val_t n, bool strict) : _n(n), _strict(strict) {}

        [[nodiscard]] static constexpr bound_t strict(val_t n)     {return bound_t(n, true,  false);}
        [[nodiscard]] static constexpr bound_t non_strict(val_t n) {return bound_t(n, false, false);}
        [[nodiscard]] static constexpr bound_t inf()               {return bound_t(0, true,  true);}
        [[nodiscard]] static constexpr bound_t le_zero()           {return bound_t(0, false, false);}
        [[nodiscard]] static constexpr bound_t lt_zero()           {return bound_t(0, true,  false);}

        [[nodiscard]] inline val_t get_bound()    const {return this->_n;}
        [[nodiscard]] inline bool is_strict()     const {return this->_strict;}
        [[nodiscard]] inline bool is_non_strict() const {return not this->_strict;}
        [[nodiscard]] inline bool is_inf()        const {return this->_inf;}

        [[nodiscard]] static const bound_t& max(const bound_t &a, const bound_t &b);
        [[nodiscard]] static bound_t max(bound_t &&a, bound_t &&b);
        [[nodiscard]] static bound_t max(const bound_t &a, bound_t &&b);
        [[nodiscard]] static bound_t max(bound_t &&a, const bound_t &b);

        [[nodiscard]] static const bound_t& min(const bound_t &a, const bound_t &b);
        [[nodiscard]] static bound_t min(bound_t &&a, bound_t &&b);
        [[nodiscard]] static bound_t min(const bound_t &a, bound_t &&b);
        [[nodiscard]] static bound_t min(bound_t &&a, const bound_t &b);

        [[nodiscard]] bound_t operator+(bound_t rhs) const;
        [[nodiscard]] bound_t operator+(val_t rhs) const;

        [[nodiscard]] bound_t operator-(val_t rhs) const;

        [[nodiscard]] bound_t operator*(val_t rhs) const;

        [[nodiscard]] bool operator<(bound_t rhs) const;
        [[nodiscard]] bool operator==(bound_t rhs) const;

        [[nodiscard]] bool operator!=(bound_t rhs) const;
        [[nodiscard]] bool operator>(bound_t rhs) const;
        [[nodiscard]] bool operator>=(bound_t rhs) const;
        [[nodiscard]] bool operator<=(bound_t rhs) const;

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
    [[nodiscard]] bound_t operator*(val_t val, bound_t bound);
    std::ostream& operator<<(std::ostream& out, const bound_t& bound);
}

#endif //PARDIBAAL_BOUND_T_H
