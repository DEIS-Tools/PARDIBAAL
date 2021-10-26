//
// Created by Thomas Grosen on 21/10/2021.
//

#ifndef BDM_BOUNDS_TABLE_T_H
#define BDM_BOUNDS_TABLE_T_H

#include "bound_t.h"

#include <vector>

namespace dbm {

    struct bounds_table_t {
    public:
        const int _number_of_clocks;

        bounds_table_t(int size);

        bound_t &get(const int &i, const int &j);
        bound_t &get(int &&i, int &&j);
        bound_t &get(int &&i, const int &j);
        bound_t &get(const int &i, int &&j);

        bound_t at(const int &i, const int &j) const;
        bound_t at(int &&i, int &&j) const;
        bound_t at(int &&i, const int &j) const;
        bound_t at(const int &i, int &&j) const;

    private:
        std::vector<bound_t> _bounds;
    };

    struct bounds_table_ptr_t {
    public:
        const int _number_of_clocks;

        ~bounds_table_ptr_t();
        bounds_table_ptr_t& operator=(bounds_table_ptr_t &&other) noexcept;
        bounds_table_ptr_t& operator=(const bounds_table_ptr_t &other) noexcept;

        bounds_table_ptr_t(const int &_number_of_clocks);
        bounds_table_ptr_t(const int &&_number_of_clocks);

        bound_t* get(const int &i, const int &j);
        bound_t* get(int &&i, int &&j);
        bound_t* get(const int &i, int &&j);
        bound_t* get(int &&i, const int &j);

        bound_t &ref(const int &i, const int &j);

        bound_t at(const int &i, const int &j) const;
        bound_t at(int &&i, int &&j) const;
        bound_t at(const int &i, int &&j) const;
        bound_t at(int &&i, const int &j) const;

        void set(const int &i, const int &j, const bound_t &b);
        void set(const int &i, const int &j, bound_t &&b);

    private:
        bound_t* _bounds;
    };
}

#endif //BDM_BOUNDS_TABLE_T_H
