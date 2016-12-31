// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC14_HPP__
#define __INCLUDED_AOC14_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC14: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC14 > ResultA;
        Property::Int< AOC14 > ResultB;
    };

}

#endif
