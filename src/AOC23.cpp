// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC23.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    void AOC23::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC23, ProcessLine, const Tau::String& ) );

        ResultA = Calculate( 7 );
        ResultB = Calculate( 12 );
    }

    bool AOC23::ProcessLine( const Tau::String& line )
    {
        m_instructionParser.Process( line );
        return true;
    }

    long long AOC23::Calculate( int initialValue ) const
    {
        Processor processor;
        return Computer::Calculate( processor, initialValue, m_instructionParser.GetInstructions(), true );
    }

}
