// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC19_HPP__
#define __INCLUDED_AOC19_HPP__

#include "core/lang/Object.hpp"
#include "core/container/List.hpp"

namespace AOC
{

    class AOC19: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC19 > ResultA;
        Property::Int< AOC19 > ResultB;
    };

}

#endif
