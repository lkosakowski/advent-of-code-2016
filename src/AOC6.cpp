// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC6.hpp"

#include "core/file/File.hpp"
#include "core/container/ContainerUtils.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    static const int MessageLength = 8;

    // ---------------- //

    struct CompareIteratorLess
    {
        bool operator()( AOC6::MapIterator it1, AOC6::MapIterator it2 )
        {
            return it1->second < it2->second;
        }
    };

    struct CompareIteratorGreater
    {
        bool operator()( AOC6::MapIterator it1, AOC6::MapIterator it2 )
        {
            return it1->second > it2->second;
        }
    };

    // ---------------- //

    void AOC6::Solve( const Tau::String& fileName )
    {
        Frequencies.EnsureSize( MessageLength );

        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC6, ProcessLine, const Tau::String& ) );

        ExtractMessage< CompareIteratorLess >( ResultA.m_v );
        ExtractMessage< CompareIteratorGreater >( ResultB.m_v );
    }

    bool AOC6::ProcessLine( const Tau::String& line )
    {
        TAU_ASSERT( line.GetLength() == MessageLength )

        for( int i = 0; i < line.GetLength(); i++ )
        {
            Frequencies[i].Add( line[i], 0, true )++;
        }

        return true;
    }

    template< class CompareIterator >
    void AOC6::ExtractMessage( Tau::String& message )
    {
        TAU_ASSERT( !message.HasLength() )

        for( Tau::Vector< Tau::Map_< char, int > >::const_iterator it = Frequencies.begin(); it != Frequencies.end(); it++ )
        {
            Tau::SortedMap< char, int, CompareIterator > sortedMap( *it );
            message += sortedMap.Get()->first;
        }
    }

}
