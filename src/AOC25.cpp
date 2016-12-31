// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC25.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

#define AOC_MSG( msg ) // TAU_MSG( msg )

namespace AOC
{

    void AOC25::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC25, ProcessLine, const Tau::String& ) );

        Processor processor;
        processor.OutEvent += EventHandler( AOC25, OnOut, const Processor::OutInfo& );

        int i = 0;

        while( !Finished )
        {
            i++;
            m_states.Clear();
            ExpectedOut = 0;
            m_output.Clear();
            Computer::Calculate( processor, i, m_instructionParser.GetInstructions(), true );
            AOC_MSG( i << " -> " << m_output )
        }

        ResultA = i;
    }

    bool AOC25::ProcessLine( const Tau::String& line )
    {
        TAU_ASSERT( !Tau::StringUtils::StartsWith( line, "tgl" ) )
        m_instructionParser.Process( line );
        return true;
    }

    void AOC25::OnOut( const Processor::OutInfo& out )
    {
        TAU_ASSERT( out.value == 0 || out.value == 1 )

        m_output += Tau::StringUtils::String( out.value );

        if( out.value != ExpectedOut )
        {
            out.processor->Halt();
            return;
        }

        ExpectedOut = out.value == 0 ? 1 : 0;

        Tau::Map_< Processor::State, long long >::const_iterator it = m_states.Find( out.processor->GetState(), true );

        if( it != m_states.end() )
        {
            TAU_ASSERT( it->second == out.value )
            Finished = true;
            out.processor->Halt();
            return;
        }

        m_states.Add( out.processor->GetState(), out.value );
    }

}
