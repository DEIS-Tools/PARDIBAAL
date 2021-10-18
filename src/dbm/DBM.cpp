//
// Created by Thomas Grosen on 13/10/2021.
//

#include "DBM.h"
#include <stdlib.h>

namespace dbm {
    /********************************************************
     * bound_t
     ********************************************************/
    bound_t::bound_t(int n, bool strict) : _n(n), _strict(strict) {}

    // Returns const ref to the lowest bound
    const bound_t& bound_t::min(const bound_t &a, const bound_t &b) {
        if (bound_t::less_eq(a, b))
            return a;
        return b;
    }

    //TODO: I hope this is a cool way to do it :S
    bound_t bound_t::min(bound_t &&a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(const bound_t &a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(bound_t &&a, const bound_t &b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::add(const bound_t &a, const bound_t &b) {
        if (a._inf || b._inf)
            return bound_t::inf();

        return bound_t(a._n + b._n, a._strict || b._strict);
    }

    bound_t bound_t::add(bound_t &&a, bound_t &&b) {
        return bound_t::add(a, b);
    }

    bound_t bound_t::add(const bound_t &a, bound_t &&b) {
        return bound_t::add(a, b);
    }

    bound_t bound_t::add(bound_t &&a, const bound_t &b) {
        return bound_t::add(a, b);
    }

    bound_t bound_t::inf() {
        bound_t b;
        b._inf = true;

        return b;
    }

    bool bound_t::less(const bound_t &lhs, const bound_t &rhs) {
        if (lhs._inf) return false;
        if (rhs._inf) return true;

        if (lhs._n == rhs._n) {
            if (rhs._strict)
                return false;

            return lhs._strict;
        }

        return lhs._n < rhs._n;
    }

    bool bound_t::less(bound_t &&lhs, bound_t &&rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less(const bound_t &lhs, bound_t &&rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less(bound_t &&lhs, const bound_t &rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less_eq(const bound_t &lhs, const bound_t &rhs) {
        if (lhs._inf)
            return rhs._inf;

        if (lhs._n == rhs._n) {
            if (rhs._strict)
                return lhs._strict;

            return true;
        }

        return lhs._n < rhs._n;
    }

    bool bound_t::less_eq(bound_t &&lhs, bound_t &&rhs) {
        return bound_t::less_eq(lhs, rhs);
    }

    bool bound_t::less_eq(const bound_t &lhs, bound_t &&rhs) {
        return bound_t::less_eq(lhs, rhs);
    }

    bool bound_t::less_eq(bound_t &&lhs, const bound_t &rhs) {
        return bound_t::less_eq(lhs, rhs);
    }

    /********************************************************
     * bounds_table_t
     ********************************************************/
    bounds_table_t::bounds_table_t(const int &_number_of_clocks) : _number_of_clocks(_number_of_clocks) {
        this->_bounds = new bound_t[_number_of_clocks * _number_of_clocks];
    }

    bounds_table_t::bounds_table_t(const int &&_number_of_clocks) : _number_of_clocks(_number_of_clocks) {
        this->_bounds = new bound_t[_number_of_clocks * _number_of_clocks];
    }

    // i is row and j is column, is this good? => at(i, j) means: "X_i - X_j < n" or the edge "X_j --> X_i"
    bound_t *bounds_table_t::at(const int &i, const int &j) {
        if (i >= _number_of_clocks || j >= _number_of_clocks)
            return nullptr; //TODO: Throw an exception maybe??... Probably.. yes.

        return _bounds + (i * _number_of_clocks) + j;
    }

    bound_t *bounds_table_t::at(int &&i, int &&j) {
        return this->at(i, j); //TODO: is this dumb?
    }

    bound_t *bounds_table_t::at(const int &i, int &&j) {
        return this->at(i, j);
    }

    bound_t *bounds_table_t::at(int &&i, const int &j) {
        return this->at(i, j);
    }

    //TODO: is this crazy? is this doing only a single copy?
    void bounds_table_t::set(const int &i, const int &j, const bound_t &b) {
        *(this->at(i, j)) = b;
    }

    void bounds_table_t::set(const int &i, const int &j, bound_t &&b) {
        this->set(i, j, b);
    }

    bounds_table_t::~bounds_table_t() {
        delete[] _bounds;
    }

    bounds_table_t &bounds_table_t::operator=(bounds_table_t &&other)  noexcept {
        if (this->_number_of_clocks != other._number_of_clocks)
            return *this; //TODO: probably throw an error here

        if (this->_bounds != other._bounds) {
            this->_bounds = other._bounds;
            other._bounds = nullptr;
        }

        return *this;
    }

    //TODO: Maybe if I used a template for the size, then I could specify that the type must match? hmm...
    bounds_table_t &bounds_table_t::operator=(bounds_table_t &other) noexcept {
        if (this->_number_of_clocks != other._number_of_clocks)
            return *this; //TODO: probably throw an error here

        if (this->_bounds != other._bounds) {
            for (int i = 0; i < this->_number_of_clocks; i++) {
                for (int j = 0; j < this->_number_of_clocks; j++) {
                    this->set(i, j, *other.at(i, j));
                }
            }
        }
        return *this;
    }

    /********************************************************
     * DBM Class
     ********************************************************/
    bool DBM::is_empty() {
        // The DBM has to be closed for this to actually work
        for (int i = 0; i < this->_bounds_table._number_of_clocks; i++) {
            for (int j = 0; j < this->_bounds_table._number_of_clocks; j++) {
                bound_t i_to_j_to_i = bound_t::add(*(this->_bounds_table.at(i, j)), *(this->_bounds_table.at(j, i)));
                if (bound_t::less(i_to_j_to_i, bound_t(0, false)))
                    return true;
            }
        }

        return false;
    }

    // Convert to canonical form
    void DBM::close() {
        const int &size = _bounds_table._number_of_clocks;
        for(int k = 0; k < size; k++) {
            for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                    _bounds_table.set(i, j, bound_t::min(*_bounds_table.at(i, j),
                                                         bound_t::add(*_bounds_table.at(i, k), *_bounds_table.at(k, j))));
                }
            }
        }
    }

    DBM::DBM(const int size) : _bounds_table(size) {}

    DBM DBM::gen_random(const int size) {
        return DBM(0);
    }
}
