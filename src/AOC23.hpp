// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC23_HPP__
#define __INCLUDED_AOC23_HPP__

#include "Computer.hpp"

namespace AOC
{

    class AOC23: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Long< AOC23 > ResultA;
        Property::Long< AOC23 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );
        long long Calculate( int initialValue ) const;

        InstructionParser m_instructionParser;
    };

}

#endif
