//
// Created by Thomas Grosen on 13/10/2021.
//

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include <vector>

namespace dbm {

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

    struct bounds_table_vec_t {
        std::vector<bound_t> _bounds;
        const int _number_of_clocks;

        bounds_table_vec_t(int size);

        bound_t& at(int i, int j);

        void set(int i, int j, bound_t b);
    };

    struct bounds_table_t {
    public:
        const int _number_of_clocks;

        ~bounds_table_t();
        bounds_table_t& operator=(bounds_table_t &&other) noexcept;
        bounds_table_t& operator=(const bounds_table_t &other) noexcept;

        bounds_table_t(const int &_number_of_clocks);
        bounds_table_t(const int &&_number_of_clocks);

        bound_t *here() const;

        bound_t* get(const int &i, const int &j);
        bound_t* get(int &&i, int &&j);
        bound_t* get(const int &i, int &&j);
        bound_t* get(int &&i, const int &j);

        bound_t at(const int &i, const int &j) const;
        bound_t at(int &&i, int &&j) const;
        bound_t at(const int &i, int &&j) const;
        bound_t at(int &&i, const int &j) const;



        void set(const int &i, const int &j, const bound_t &b);
        void set(const int &i, const int &j, bound_t &&b);

    private:
        bound_t* _bounds;
    };

    class DBM {
    public:
        bounds_table_t _bounds_table;
        bounds_table_vec_t _bounds_table_vec;

        DBM(const int size);

        static DBM gen_random(int &&size, int &&max);

        bool is_empty();
        void close_p_arr();
        void close_val_arr();
        void close_vec();
    };

}

#endif //BDM_DBM_H
