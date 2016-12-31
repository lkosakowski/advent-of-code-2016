// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC21_HPP__
#define __INCLUDED_AOC21_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Vector.hpp"

namespace AOC
{

    class Operation;

    // ---------------- //

    class AOC21: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::String< AOC21 > ResultA;
        Property::String< AOC21 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        Tau::Vector< Tau::SmartPtr< Operation > > m_operations;
    };

}

#endif
