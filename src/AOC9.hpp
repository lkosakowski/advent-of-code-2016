// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC9_HPP__
#define __INCLUDED_AOC9_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class AOC9: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Long< AOC9 > ResultA;
        Property::Long< AOC9 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );
    };

}

#endif
