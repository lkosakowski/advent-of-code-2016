// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC20_HPP__
#define __INCLUDED_AOC20_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Set.hpp"

namespace AOC
{

    class Range
    {
    public:
        Range( unsigned int begin, unsigned int end );

        bool operator<( const Range& range ) const;

        Property::UnsignedInt< Range > Begin;
        Property::UnsignedInt< Range > End;
    };

    // ---------------- //

    class AOC20: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::UnsignedInt< AOC20 > ResultA;
        Property::UnsignedInt< AOC20 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        Tau::Set< Range > m_ranges;
    };

}

#endif
