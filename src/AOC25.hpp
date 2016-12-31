// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC25_HPP__
#define __INCLUDED_AOC25_HPP__

#include "Computer.hpp"

namespace AOC
{

    class AOC25: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::Int< AOC25 > ResultA;
        Property::Int< AOC25 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        void OnOut( const Processor::OutInfo& out );

        Property::Bool< AOC25 > Finished;
        Property::Long< AOC25 > ExpectedOut;
        Tau::Map_< Processor::State, long long > m_states;
        Tau::String m_output;

        InstructionParser m_instructionParser;
    };

}

#endif
