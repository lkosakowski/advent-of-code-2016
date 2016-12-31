// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC12.hpp"

#include "core/file/File.hpp"

namespace AOC
{

    void AOC12::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC12, ProcessLine, const Tau::String& ) );

        Processor processor;

        // A
        {
            processor.Execute( Program( m_instructionParser.GetInstructions() ) );
            ResultA = processor.GetRegister( 0 );
        }

        // B
        {
            Tau::Vector< InstructionPtr > init;
            init.Add( InstructionParser::Cpy( "1 c" ) );
            processor.Execute( Program( init + m_instructionParser.GetInstructions() ) );
            ResultB = processor.GetRegister( 0 );
        }
    }

    bool AOC12::ProcessLine( const Tau::String& line )
    {
        m_instructionParser.Process( line );
        return true;
    }

}
