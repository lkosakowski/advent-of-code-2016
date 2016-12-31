// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC16_HPP__
#define __INCLUDED_AOC16_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC16: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::String< AOC16 > ResultA;
        Property::String< AOC16 > ResultB;
    };

}

#endif
