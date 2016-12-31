// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC15_HPP__
#define __INCLUDED_AOC15_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Vector.hpp"

namespace AOC
{

    class Disc
    {
    public:
        Disc( int number, int positions, int initialPosition );

        void Advance( int ticks );

        Property::Int< Disc > Number;
        Property::Int< Disc > Positions;
        Property::Int< Disc > CurrentPosition;
    };

    // ---------------- //

    class AOC15: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC15 > ResultA;
        Property::Int< AOC15 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        int Solve( const Tau::Vector< Disc >& discs ) const;
        bool Simulate( int time ) const;

        Tau::Vector< Disc > m_discs;
    };

}

#endif
