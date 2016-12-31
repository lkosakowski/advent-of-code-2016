// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC1_HPP__
#define __INCLUDED_AOC1_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC1: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC1 > ResultA;
        Property::Int< AOC1 > ResultB;
    };

}

#endif
