//
// Created by Thomas Grosen on 21/10/2021.
//

#include "bounds_table_t.h"
#include "bound_t.h"

#include <vector>

namespace dbm {

    /********************************************************
     * bounds_table_t
     ********************************************************/
    bounds_table_t::bounds_table_t(int size) : _number_of_clocks(size) {
        _bounds = std::vector<bound_t>(size * size);
    }

    bound_t &bounds_table_t::get(const int &i, const int &j) {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

    bound_t &bounds_table_t::get(int &&i, int &&j) {
        return get(i, j);
    }

    bound_t &bounds_table_t::get(int &&i, const int &j) {
        return get(i, j);
    }

    bound_t &bounds_table_t::get(const int &i, int &&j) {
        return get(i, j);
    }

    bound_t bounds_table_t::at(const int &i, const int &j) const {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

    bound_t bounds_table_t::at(int &&i, int &&j) const {
        return at(i, j);
    }

    bound_t bounds_table_t::at(int &&i, const int &j) const {
        return at(i, j);
    }

    bound_t bounds_table_t::at(const int &i, int &&j) const {
        return at(i, j);
    }

    /********************************************************
     * bounds_table_ptr_t
     ********************************************************/
    bounds_table_ptr_t::bounds_table_ptr_t(const int &_number_of_clocks) : _number_of_clocks(_number_of_clocks) {
        this->_bounds = new bound_t[_number_of_clocks * _number_of_clocks];
    }

    bounds_table_ptr_t::bounds_table_ptr_t(const int &&_number_of_clocks) : _number_of_clocks(_number_of_clocks) {
        this->_bounds = new bound_t[_number_of_clocks * _number_of_clocks];
    }

    // i is row and j is column, is this good? => get(i, j) means: "X_i - X_j < n" or the edge "X_j --> X_i"
    bound_t *bounds_table_ptr_t::get(const int &i, const int &j) {
        if (i >= _number_of_clocks || j >= _number_of_clocks)
            return nullptr; //TODO: Throw an exception maybe??... Probably.. yes.

        return _bounds + (i * _number_of_clocks) + j;
    }

    bound_t *bounds_table_ptr_t::get(int &&i, int &&j) {
        return this->get(i, j); //TODO: is this dumb?
    }

    bound_t *bounds_table_ptr_t::get(const int &i, int &&j) {
        return this->get(i, j);
    }

    bound_t *bounds_table_ptr_t::get(int &&i, const int &j) {
        return this->get(i, j);
    }

    //TODO: is it faster to return by pointer? or by value?
    bound_t bounds_table_ptr_t::at(const int &i, const int &j) const {
        if (i >= _number_of_clocks || j >= _number_of_clocks)
            return *_bounds;; //TODO: Throw an exception maybe??... Probably.. yes.

        return *(_bounds + (i * _number_of_clocks) + j);
    }

    bound_t bounds_table_ptr_t::at(int &&i, int &&j) const {
        return at(i, j);
    }

    bound_t bounds_table_ptr_t::at(const int &i, int &&j) const {
        return at(i, j);
    }

    bound_t bounds_table_ptr_t::at(int &&i, const int &j) const {
        return at(i, j);
    }

    //TODO: is this crazy? is this doing only a single copy?
    void bounds_table_ptr_t::set(const int &i, const int &j, const bound_t &b) {
        *(this->get(i, j)) = b;
    }

    void bounds_table_ptr_t::set(const int &i, const int &j, bound_t &&b) {
        this->set(i, j, b);
    }

    bounds_table_ptr_t::~bounds_table_ptr_t() {
        delete[] _bounds;
    }

    // Move assignment - Switch pointers, delete old allocated space, set other to nullptr
    bounds_table_ptr_t &bounds_table_ptr_t::operator=(bounds_table_ptr_t &&other)  noexcept {
        if (this->_number_of_clocks != other._number_of_clocks)
            return *this;

        // OUCH - Pointer handling be HOT! >:)
        if (this->_bounds != other._bounds) {
            bound_t *tmp = this->_bounds;
            this->_bounds = other._bounds;
            other._bounds = nullptr;
            delete[] tmp;
        }

        return *this;
    }

    // Copy assignment - Copy all entries
    bounds_table_ptr_t &bounds_table_ptr_t::operator=(const bounds_table_ptr_t &other) noexcept {
        if (this->_number_of_clocks != other._number_of_clocks)
            return *this;

        if (this->_bounds != other._bounds) {
            for (int i = 0; i < this->_number_of_clocks; i++) {
                for (int j = 0; j < this->_number_of_clocks; j++) {
                    this->set(i, j, other.at(i, j));
                }
            }
        }
        return *this;
    }

    bound_t &bounds_table_ptr_t::ref(const int &i, const int &j) {
        return *(_bounds + (i * _number_of_clocks) + j);
    }

}
