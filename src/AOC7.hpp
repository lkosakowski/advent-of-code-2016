// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC7_HPP__
#define __INCLUDED_AOC7_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC7: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC7 > ResultA;
        Property::Int< AOC7 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );
    };

}

#endif
