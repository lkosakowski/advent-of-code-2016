// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC18_HPP__
#define __INCLUDED_AOC18_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC18: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC18 > ResultA;
        Property::Int< AOC18 > ResultB;
    };

}

#endif
