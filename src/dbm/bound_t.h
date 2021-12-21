//
// Created by Thomas Grosen on 26/10/2021.
//

#ifndef BDM_BOUND_T_H
#define BDM_BOUND_T_H

#include <cstdlib>
#include <ostream>

namespace dbm{
    using dim_t = uint32_t;
    using val_t = int64_t;

    struct bound_t {
        bound_t(){};
        bound_t(val_t n, bool strict);
        static bound_t inf();
        static bound_t zero();

        val_t _n = 0;
        bool _strict = false, _inf = false;

        static const bound_t& max(const bound_t &a, const bound_t &b);
        static bound_t max(bound_t &&a, bound_t &&b);
        static bound_t max(const bound_t &a, bound_t &&b);
        static bound_t max(bound_t &&a, const bound_t &b);

        static const bound_t& min(const bound_t &a, const bound_t &b);
        static bound_t min(bound_t &&a, bound_t &&b);
        static bound_t min(const bound_t &a, bound_t &&b);
        static bound_t min(bound_t &&a, const bound_t &b);

        bound_t operator+(bound_t rhs) const;

        bool operator<=(bound_t rhs) const;

        bool operator<(bound_t rhs) const;

        bool operator==(bound_t rhs) const;

        bool operator!=(bound_t rhs) const;

        bool operator>(bound_t rhs) const;

        bool operator>=(bound_t rhs) const;

        friend std::ostream& operator<<(std::ostream& out, const bound_t& bound);
    };

    std::ostream& operator<<(std::ostream& out, const bound_t& bound);
}

#endif //BDM_BOUND_T_H
