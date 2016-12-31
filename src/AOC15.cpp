// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC15.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    Disc::Disc( int number, int positions, int initialPosition )
        : Number( number )
        , Positions( positions )
        , CurrentPosition( initialPosition )
    {
        TAU_ASSERT( Positions > 0 )
        TAU_ASSERT( CurrentPosition >= 0 && CurrentPosition < Positions)
    }

    void Disc::Advance( int ticks )
    {
        CurrentPosition = ( CurrentPosition + ticks ) % Positions;
    }

    // ---------------- //

    void AOC15::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC15, ProcessLine, const Tau::String& ) );

        ResultA = Solve( m_discs );

        m_discs.Add( Disc( m_discs.Size() + 1, 11, 0 ) );
        ResultB = Solve( m_discs );
    }

    int AOC15::Solve( const Tau::Vector< Disc >& discs ) const
    {
        int time = 0;

        while( true )
        {
            if( Simulate( time ) )
            {
                return time;
            }

            time++;
        }
    }

    static int GetBetween( const Tau::String& line, const Tau::String& separator1, const Tau::String& separator2 )
    {
        return Tau::StringUtils::To< int >( Tau::StringUtils::GetBetween( line, separator1, separator2, false ) );
    }

    bool AOC15::ProcessLine( const Tau::String& line )
    {
        int number = GetBetween( line, "Disc #", " has " );
        int positions = GetBetween( line, " has ", " positions; at time=0, it is at position " );
        int initialPosition = GetBetween( line, " positions; at time=0, it is at position ", "." );

        m_discs.Add( Disc( number, positions, initialPosition ) );
        TAU_ASSERT( m_discs.Size() == number )

        return true;
    }

    bool AOC15::Simulate( int time ) const
    {
        Tau::Vector< Disc > copy = m_discs;

        for( Tau::Vector< Disc >::iterator it = copy.begin(); it != copy.end(); it++ )
        {
            it->Advance( it->Number + time );

            if( it->CurrentPosition != 0 )
            {
                return false;
            }
        }

        return true;
    }

}
