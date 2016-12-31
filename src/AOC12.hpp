// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC12_HPP__
#define __INCLUDED_AOC12_HPP__

#include "Computer.hpp"

namespace AOC
{

    class AOC12: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Long< AOC12 > ResultA;
        Property::Long< AOC12 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        InstructionParser m_instructionParser;
    };

}

#endif
