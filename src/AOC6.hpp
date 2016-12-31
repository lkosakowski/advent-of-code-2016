// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC6_HPP__
#define __INCLUDED_AOC6_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Map.hpp"
#include "core/container/Vector.hpp"

namespace AOC
{

    class AOC6: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::String< AOC6 > ResultA;
        Property::String< AOC6 > ResultB;

        typedef Tau::Map_< char, int >::const_iterator MapIterator;

    private:
        bool ProcessLine( const Tau::String& line );

        template< class CompareIterator >
        void ExtractMessage( Tau::String& message );

        Tau::Vector< Tau::Map_< char, int > > Frequencies;
    };

}

#endif
