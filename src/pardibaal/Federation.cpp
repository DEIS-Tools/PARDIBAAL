/*
 * Copyright Thomas M. Grosen
 * Created on 01/04/2022
 */

/*
 * This file is part of pardibaal
 *
 * pardibaal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pardibaal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with pardibaal. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Federation.h"
#include "errors.h"

namespace pardibaal {

    void Federation::make_consistent() {
        std::erase_if(zones, [](DBM& dbm){return dbm.is_empty();});
    }

    Federation::Federation() : zones() {}

    Federation::Federation(dim_t dimension) : zones{DBM(dimension)} {}

    Federation::Federation(DBM dbm) : zones() {this->add(std::move(dbm));}

    auto Federation::begin() const {return zones.begin();}
    auto Federation::end() const {return zones.end();}

    const DBM& Federation::at(dim_t index) const {
#ifndef NEXCEPTIONS
        if (index >= zones.size())
            throw base_error("ERROR: Out of bounds access on index: ", index, " but the federation has: ",
                             zones.size(), " zones");
#endif
        return zones[index];
    }

    void Federation::add(DBM dbm) {
#ifndef NEXCEPTIONS
        if (!zones.empty()) {
            if (dimension() != dbm.dimension())
                throw base_error("ERROR: Adding dbm with dimension: ", dbm.dimension(),
                                 " to a federation with dimension: ", dimension());
        }
#endif
        auto r = this->relation(dbm);
        if (r._subset)
            *this = Federation(dbm);
        if (r._different)
            zones.push_back(dbm);
    }

    void Federation::remove(dim_t index) {
#ifndef NEXCEPTIONS
        if (index >= zones.size())
            throw base_error("ERROR: Out of bounds access on index: ", index, " but the federation has: ",
                             zones.size(), " zones");
#endif
        zones.erase(std::next(zones.begin(), index));
    }

    dim_t Federation::size() const { return zones.size();}

    dim_t Federation::dimension() const {
        return zones.size() >= 1 ? zones[0].dimension() : 0;
    }

    //TODO: If the federation is always consistent, then we just have to check size() == 0
    bool Federation::is_empty() const {
        for (const auto& dbm : zones) {
            if (not dbm.is_empty())
                return false;
        }
        return true;
    }

    bool Federation::satisfies(dim_t x, dim_t y, bound_t g) const {
        for (const auto& dbm : zones) {
            if (dbm.is_satisfied(x, y, g))
                return true;
        }
        return false;
    }

    relation_t Federation::relation(const DBM &dbm) const {
        if (this->is_empty())
            return dbm.is_empty() ? relation_t::equal() : relation_t::subset();
        if (dbm.is_empty())
            return relation_t::superset();

        bool eq = false, sub = false;

        for (const auto& e : zones) {
            auto relation = e.relation(dbm);
            if (relation._superset && not relation._equal) return relation;

            eq |= relation._equal;
            sub &= relation._subset;
        }

        if (eq && sub) return relation_t::equal();
        if (sub) return relation_t::subset();

        return relation_t::different();
    }

    relation_t Federation::relation(const Federation &fed) const {
        if (this->is_empty())
            return fed.is_empty() ? relation_t::equal() : relation_t::subset();
        if (fed.is_empty())
            return relation_t::superset();

        bool eq = false, sup = false;

            for (const auto& e : fed) {
                auto relation = this->relation(e);
                if (relation._subset && not relation._equal) return relation_t::subset();

                eq |= relation._equal;
                sup &= relation._superset;
            }

            if (eq && sup) return relation_t::equal();
            if (sup) return relation_t::superset();

        return relation_t::different();
    }

    bool Federation::equal(const DBM &dbm) const {
        return this->relation(dbm)._equal;
    }

    bool Federation::equal(const Federation &fed) const {
        return this->relation(fed)._equal;
    }

    bool Federation::subset(const DBM &dbm) const {
        return this->relation(dbm)._subset;
    }

    bool Federation::subset(const Federation &fed) const {
        return this->relation(fed)._subset;
    }

    bool Federation::superset(const DBM &dbm) const {
        return this->relation(dbm)._subset;
    }

    bool Federation::superset(const Federation &fed) const {
        return this->relation(fed)._superset;
    }

    bool Federation::is_unbounded() const {
        bool rtn = true;
        for (const DBM& dbm : zones)
            rtn &= dbm.is_unbounded();

        return rtn;
    }

    void Federation::future() {
        for (DBM& dbm : zones) dbm.future();
    }

    void Federation::past() {
        for (DBM& dbm : zones) dbm.past();
    }

    void Federation::restrict(dim_t x, dim_t y, bound_t g) {
        for (DBM& dbm : zones) dbm.restrict(x, y, g);
        make_consistent();
    }

    void Federation::free(dim_t x) {
        for (DBM& dbm : zones) dbm.free(x);
    }

    void Federation::assign(dim_t x, val_t m) {
        for (DBM& dbm : zones) dbm.assign(x, m);
        make_consistent();
    }

    void Federation::copy(dim_t x, dim_t y) {
        for (DBM& dbm : zones) dbm.copy(x, y);
    }

    void Federation::shift(dim_t x, val_t n) {
        for (DBM& dbm : zones) dbm.shift(x, n);
        make_consistent();
    }

    void Federation::extrapolate(const std::vector<val_t> &ceiling) {
        for (DBM& dbm : zones) dbm.extrapolate(ceiling);
    }

    void Federation::extrapolate_diagonal(const std::vector<val_t> &ceiling) {
        for (DBM& dbm : zones) dbm.extrapolate_diagonal(ceiling);
    }

    void Federation::extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper) {
        for (DBM& dbm : zones) dbm.extrapolate_lu(lower, upper);
    }

    void Federation::extrapolate_lu_diagonal(const std::vector<val_t> &lower, const std::vector<val_t> &upper) {
        for (DBM& dbm : zones) dbm.extrapolate_lu_diagonal(lower, upper);
    }

    void Federation::remove_clock(dim_t c) {
        for (DBM& dbm : zones) dbm.remove_clock(c);
    }

    void Federation::swap_clocks(dim_t a, dim_t b) {
        for (DBM& dbm : zones) dbm.swap_clocks(a, b);
    }

    void Federation::add_clock_at(dim_t c) {
        for (DBM& dbm : zones) dbm.add_clock_at(c);
    }

    std::vector<dim_t> Federation::resize(const std::vector<bool> &src_bits, const std::vector<bool> &dst_bits) {
        bool first = true;
        std::vector<dim_t> rtn;

        for (DBM& dbm : zones) {
            if (first) {
                rtn = dbm.resize(src_bits, dst_bits);
                first = false;
            }
            dbm.resize(src_bits, dst_bits);
        }

        return rtn;
    }

    void Federation::reorder(std::vector<dim_t> order, dim_t new_size) {
        for (DBM& dbm : zones) dbm.reorder(order, new_size);
    }

    std::ostream& operator<<(std::ostream& out, const Federation& fed) {
        bool first = true;
        out << "\n<<<<<<\n";
        for (const DBM& dbm : fed) {
            if (first) {out << dbm; first = false;}
            else out << "&&" << dbm;
        }
        out << ">>>>>>\n";
    }
}
