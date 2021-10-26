//
// Created by Thomas Grosen on 26/10/2021.
//

#ifndef BDM_BOUND_T_H
#define BDM_BOUND_T_H

#include <cstdlib>
#include <ctime>

namespace dbm{

    struct bound_t {
        bound_t(){};
        bound_t(int n, bool strict);
        static bound_t inf();
        static bound_t zero();

        int _n = 0;
        bool _strict = false, _inf = false;

        static const bound_t& min(const bound_t &a, const bound_t &b);
        static bound_t min(bound_t &&a, bound_t &&b);
        static bound_t min(const bound_t &a, bound_t &&b);
        static bound_t min(bound_t &&a, const bound_t &b);

        static bound_t add(const bound_t &a, const bound_t &b);
        static bound_t add(bound_t &&a, bound_t &&b);
        static bound_t add(const bound_t &a, bound_t &&b);
        static bound_t add(bound_t &&a, const bound_t &b);

        static bool less(const bound_t &lhs, const bound_t &rhs);
        static bool less(bound_t &&lhs, bound_t &&rhs);
        static bool less(const bound_t &lhs, bound_t &&rhs);
        static bool less(bound_t &&lhs, const bound_t &rhs);

        static bool less_eq(const bound_t &lhs, const bound_t &rhs);
        static bool less_eq(bound_t &&lhs, bound_t &&rhs);
        static bool less_eq(const bound_t &lhs, bound_t &&rhs);
        static bool less_eq(bound_t &&lhs, const bound_t &rhs);

        static bound_t gen_random(const int &max);
    };

}

#endif //BDM_BOUND_T_H
