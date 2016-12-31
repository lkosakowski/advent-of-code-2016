// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC24_HPP__
#define __INCLUDED_AOC24_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Vector.hpp"

namespace AOC
{

    class AOC24: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC24 > ResultA;
        Property::Int< AOC24 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        Tau::Vector< Tau::String > m_lines;
    };

}

#endif
