// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC20.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    Range::Range( unsigned int begin, unsigned int end )
        : Begin( begin )
        , End( end )
    {
        TAU_ASSERT( End >= Begin )
    }

    bool Range::operator<( const Range& range ) const
    {
        return Begin == range.Begin ? End < range.End : Begin < range.Begin;
    }

    // ---------------- //

    void AOC20::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC20, ProcessLine, const Tau::String& ) );

        Tau::Nullable< unsigned int > valid = 0;

        for( Tau::Set< Range >::const_iterator it = m_ranges.begin(); it != m_ranges.end(); it++ )
        {
            if( it->Begin > valid.Get() )
            {
                if( ResultA == 0 ) { ResultA = valid.Get(); }
                ResultB += it->Begin - valid.Get();
            }

            if( it->End == Math::Max< unsigned int >() )
            {
                valid = Tau::Nullable< unsigned int >::EMPTY;
                break;
            }

            if( it->End >= valid.Get() )
            {
                valid = it->End + 1;
            }
        }

        if( valid.IsSet() )
        {
            ResultB += Math::Max< unsigned int >() - valid.Get() + 1;
        }
    }

    bool AOC20::ProcessLine( const Tau::String& line )
    {
        Tau::String start, end;
        Tau::StringUtils::Split( line, start, '-', end, false );
        m_ranges.Add( Range( Tau::StringUtils::To< unsigned int >( start ), Tau::StringUtils::To< unsigned int >( end ) ) );
        return true;
    }

}
