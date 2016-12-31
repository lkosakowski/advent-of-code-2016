// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC13_HPP__
#define __INCLUDED_AOC13_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC13: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC13 > ResultA;
        Property::Int< AOC13 > ResultB;
    };

}

#endif
