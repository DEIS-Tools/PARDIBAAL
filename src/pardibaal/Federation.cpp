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

#include <algorithm>

namespace pardibaal {

    void Federation::make_consistent() {
        std::erase_if(zones, [](DBM& dbm){return dbm.is_empty();});
    }

    Federation::Federation() : zones{} {}

    Federation::Federation(dim_t dimension) : zones{DBM::zero(dimension)} {}

    Federation::Federation(const DBM& dbm) : zones{dbm} {}

    Federation Federation::zero(dim_t dimension) {return Federation(DBM::zero(dimension));}

    Federation Federation::unconstrained(dim_t dimension) {return Federation(DBM::unconstrained(dimension));}

    Federation::zone_vector::const_iterator Federation::begin() const {return zones.begin();}
    Federation::zone_vector::const_iterator Federation::end() const {return zones.end();}

    const DBM& Federation::at(dim_t index) const {
#ifndef NEXCEPTIONS
        if (index >= zones.size())
            throw base_error("ERROR: Out of bounds access on index: ", index, " but the federation has: ",
                             zones.size(), " zones");
#endif
        return zones[index];
    }

    void Federation::add(const DBM& dbm) {
#ifndef NEXCEPTIONS
        if (!zones.empty()) {
            if (dimension() != dbm.dimension())
                throw base_error("ERROR: Adding dbm with dimension: ", dbm.dimension(),
                                 " to a federation with dimension: ", dimension());
        }
#endif
        auto r = this->relation(dbm);
        if (r.is_subset())
            *this = Federation(dbm);
        if (r.is_different())
            zones.push_back(dbm);
    }

    void Federation::add(const Federation& fed) {
        for (const auto& dbm : fed)
            this->add(dbm);
    }

    void Federation::subtract(const DBM& dbm) {
#ifndef NEXCEPTIONS
        if (!zones.empty()) {
            if (dimension() != dbm.dimension())
                throw base_error("ERROR: Subtracting dbm with dimension: ", dbm.dimension(),
                                 " from a federation with dimension: ", dimension());
        }
#endif
        auto fed = Federation();
        for (auto z : zones) {
            for (dim_t i = 0; i < dimension(); ++i) {
                for (dim_t j = 0; j < dimension(); ++j) {
                    // This check ensures that the zone added is non-empty iff it is on max canonical form.
                    if (z.at(i, j) > dbm.at(i, j)) {
                        z.restrict(j, i, bound_t(-dbm.at(i, j).get_bound(), dbm.at(i, j).is_non_strict()));
                        fed.add(z);
                    }
                }
            }
        }
        *this = fed;
    }

    void Federation::subtract(const Federation& fed) {
        for (const auto& dbm : fed)
            this->subtract(dbm);
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
            if (dbm.satisfies(x, y, g))
                return true;
        }
        return false;
    }

    bool Federation::satisfies(const clock_constraint_t& constraint) const {
        return satisfies(constraint._i, constraint._j, constraint._bound);
    }

    bool Federation::satisfies(const std::vector<clock_constraint_t>& constraints) const {
        for (const auto& c : constraints)
            if (not satisfies(c._i, c._j, c._bound))
                return false;

        return true;
    }

    template<bool is_exact>
    relation_t Federation::relation(const DBM& dbm) const {
        if (is_exact)
            return this->relation<true>(Federation(dbm));
        
        if (this->is_empty())
            return dbm.is_empty() ? relation_t::equal() : relation_t::subset();
        if (dbm.is_empty())
            return relation_t::superset();
        if (this->dimension() != dbm.dimension())
            return relation_t::different();

        bool eq = false, sub = true;

        for (const auto& e : zones) {
            auto relation = e.relation(dbm);
            if (relation.is_superset() && not relation.is_equal()) return relation;

            eq = eq || relation.is_equal();
            sub = sub && relation.is_subset();
        }

        if (eq && sub) return relation_t::equal();      // If one dbm is equal and all others are equal/subsets
        if (eq && !sub) return relation_t::superset();  // If one dbm is equal, but not all the dbm are equal/subsets
        if (sub) return relation_t::subset();           // If no dbm is equal but all of them are subsets

        return relation_t::different();
    }

    template<bool is_exact>
    relation_t Federation::relation(const Federation& fed) const {
        if (this->is_empty())
            return fed.is_empty() ? relation_t::equal() : relation_t::subset();
        if (fed.is_empty())
            return relation_t::superset();
        if (this->dimension() != fed.dimension())
            return relation_t::different();

        if (is_exact) {
            
            auto fed1 = *this;
            auto fed2 = fed;

            fed1.subtract(fed);
            fed2.subtract(*this);
            
            if (fed1.is_empty()) {
                if (fed2.is_empty())
                    return relation_t::equal();
                return relation_t::subset();
            } 
            if (fed2.is_empty())
                return relation_t::superset();

            return relation_t::different();
        } else {

            bool eq = false, sup = true;

            for (const auto& dbm : fed) {
                auto relation = this->relation(dbm);
                if (relation.is_subset() && not relation.is_equal()) return relation_t::subset();

                eq = eq || relation.is_equal();
                sup = sup && relation.is_superset();
            }

            // If this is equal to one dbm in fed and supersets/equal to the rest
            if (eq && sup) return relation_t::equal();
            
            // If this is equal to one dbm in fed and not supersets to the rest (assumes no two dbms in fed are equal)
            if (eq && !sup) return relation_t::subset();

            // If this is a superset to all dbms in fed and not equal to any of them
            if (sup) return relation_t::superset();
        }
        return relation_t::different();
    }

    template<bool is_exact=true>
    bool Federation::is_equal(const DBM& dbm) const {
        return this->relation<is_exact>(dbm).is_equal();
    }

    template<bool is_exact=true>
    bool Federation::is_equal(const Federation& fed) const {
        return this->relation<is_exact>(fed).is_equal();
    }

    template<bool is_exact=true>
    bool Federation::is_subset(const DBM& dbm) const {
        return this->relation<is_exact>(dbm).is_subset();
    }

    template<bool is_exact=true>
    bool Federation::is_subset(const Federation& fed) const {
        return this->relation<is_exact>(fed).is_subset();
    }

    template<bool is_exact=true>
    bool Federation::is_superset(const DBM& dbm) const {
        return this->relation<is_exact>(dbm).is_superset();
    }

    template<bool is_exact=true>
    bool Federation::is_superset(const Federation& fed) const {
        return this->relation<is_exact>(fed).is_superset();
    }


    template relation_t Federation::relation<true>(const DBM& dbm) const;
    template relation_t Federation::relation<false>(const DBM& dbm) const;
    template relation_t Federation::relation<true>(const Federation& fed) const;
    template relation_t Federation::relation<false>(const Federation& fed) const;
    
    template bool Federation::is_equal<true>(const DBM& dbm) const;
    template bool Federation::is_equal<false>(const DBM& dbm) const;
    template bool Federation::is_equal<true>(const Federation& fed) const;
    template bool Federation::is_equal<false>(const Federation& fed) const;
    
    template bool Federation::is_subset<true>(const DBM& dbm) const;
    template bool Federation::is_subset<false>(const DBM& dbm) const;
    template bool Federation::is_subset<true>(const Federation& fed) const;
    template bool Federation::is_subset<false>(const Federation& fed) const;
    
    template bool Federation::is_superset<true>(const DBM& dbm) const;
    template bool Federation::is_superset<false>(const DBM& dbm) const;
    template bool Federation::is_superset<true>(const Federation& fed) const;
    template bool Federation::is_superset<false>(const Federation& fed) const;
    

    bool Federation::intersects(const DBM& dbm) const {
        return std::any_of(this->begin(), this->end(), [&dbm](const DBM& z){return z.intersects(dbm);});
    }

    bool Federation::intersects(const Federation& fed) const {
        return std::any_of(fed.begin(), fed.end(), [this](const DBM& dbm){return this->intersects(dbm);});
    }

    bool Federation::is_unbounded() const {
        bool rtn = true;
        for (const DBM& dbm : zones)
            rtn = rtn && dbm.is_unbounded();

        return rtn;
    }

    void Federation::future() {
        for (DBM& dbm : zones) dbm.future();
    }

    void Federation::future(val_t d) {
        for (DBM& dbm : zones) dbm.future(d);
    }

    void Federation::past() {
        for (DBM& dbm : zones) dbm.past();
    }

    void Federation::delay(val_t d) {
        for (DBM& dbm : zones) dbm.delay(d);
    }

    void Federation::interval_delay(val_t lower, val_t upper) {
        for (DBM& dbm : zones) dbm.interval_delay(lower, upper);
    }

    void Federation::restrict(dim_t x, dim_t y, bound_t g) {
        for (DBM& dbm : zones) dbm.restrict(x, y, g);
        make_consistent();
    }

    void Federation::restrict(const clock_constraint_t& constraint) {
        restrict(constraint._i, constraint._j, constraint._bound);
    }

    void Federation::restrict(const std::vector<clock_constraint_t>& constraints) {
        for (DBM& dbm : zones)
            dbm.restrict(constraints);
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

    void Federation::extrapolate(const std::vector<val_t>& ceiling) {
        for (DBM& dbm : zones) dbm.extrapolate(ceiling);
    }

    void Federation::extrapolate_diagonal(const std::vector<val_t>& ceiling) {
        for (DBM& dbm : zones) dbm.extrapolate_diagonal(ceiling);
    }

    void Federation::extrapolate_lu(const std::vector<val_t>& lower, const std::vector<val_t>& upper) {
        for (DBM& dbm : zones) dbm.extrapolate_lu(lower, upper);
    }

    void Federation::extrapolate_lu_diagonal(const std::vector<val_t>& lower, const std::vector<val_t>& upper) {
        for (DBM& dbm : zones) dbm.extrapolate_lu_diagonal(lower, upper);
    }

    void Federation::intersection(const DBM& dbm) {
        auto fed = Federation();
        for (auto& z : zones) {
            z.intersection(dbm);
            fed.add(z);
        }

        *this = std::move(fed);
    }

    void Federation::intersection(const Federation& fed) {
        auto union_fed = Federation();
        auto tmp_fed = Federation();
        for (const auto& z : fed) {
            tmp_fed = *this;
            tmp_fed.intersection(z);
            union_fed.add(tmp_fed);
        }

        *this = std::move(union_fed);
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

    std::vector<dim_t> Federation::resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits) {
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

    void Federation::reorder(const std::vector<dim_t>& order, dim_t new_size) {
        for (DBM& dbm : zones) dbm.reorder(order, new_size);
    }

    std::ostream& operator<<(std::ostream& out, const Federation& fed) {
        bool first = true;
        out << "\n<<<<<<";
        for (const DBM& dbm : fed) {
            if (first) {out << dbm; first = false;}
            else out << "&&" << dbm;
        }
        out << ">>>>>>\n";
        return out;
    }
}
