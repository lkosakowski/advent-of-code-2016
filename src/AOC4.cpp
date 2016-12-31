// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC4.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class Room: public Tau::Object
    {
    public:
        Room( const Tau::String& code )
        {
            Tau::String last;
            Tau::StringUtils::ReverseSplit( code, EncryptedName.m_v, '-', last, false );
            ParseTrailingPart( last );

            Real = Checksum == CalculateChecksum();
            DecryptedName = DecryptName();
        }

        Property::String< Room > EncryptedName;
        Property::Int< Room > SectorID;
        Property::String< Room > Checksum;

        Property::Bool< Room > Real;
        Property::String< Room > DecryptedName;

    private:
        void ParseTrailingPart( const Tau::String& string )
        {
            Tau::String left, right;
            Tau::StringUtils::Split( string, left, '[', right, false );

            TAU_ASSERT( left.GetLength() == SectorIDLength )
            SectorID = Tau::StringUtils::To< int >( left );

            Checksum = Tau::StringUtils::TrimBack( right, 1 );
            TAU_ASSERT( Checksum.m_v.GetLength() == ChecksumLength )
        }

        typedef Tau::Map_< char, int >::const_iterator MapIterator;

        Tau::String CalculateChecksum()
        {
            Tau::Vector< Tau::String > parts;
            Tau::StringUtils::Split( EncryptedName, parts, '-', false );

            Tau::Map_< char, int > map;

            for( Tau::Vector< Tau::String >::const_iterator it = parts.begin(); it != parts.end(); it++ )
            {
                for( int i = 0; i < it->GetLength(); i++ )
                {
                    map.Add( (*it)[i], 0, true )++;
                }
            }

            struct CompareIterator
            {
                bool operator()( MapIterator it1, MapIterator it2 ) const
                {
                    return it1->second == it2->second ? it1->first > it2->first : it1->second < it2->second;
                }
            };

            Tau::String checksum;
            Tau::SortedMap< char, int, CompareIterator > sortedMap( map );

            while( checksum.GetLength() < ChecksumLength )
            {
                checksum += sortedMap.Pop()->first;
            }

            return checksum;
        }

        Tau::String DecryptName()
        {
            return Tau::StringUtils::Map( EncryptedName, TAU_DELEGATE( char, Room, RotateCharacter, char ) );
        }

        char RotateCharacter( char c )
        {
            if( c == '-' ) { return ' '; }

            static const int Span = 'z' - 'a' + 1;

            TAU_ASSERT( Parser::islower( c ) )
            int result = c + SectorID % Span;
            return result > 'z' ? result - Span : result;
        }

        static const int SectorIDLength = 3;
        static const int ChecksumLength = 5;
    };

    // ---------------- //

    void AOC4::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC4, ProcessLine, const Tau::String& ) );
    }

    bool AOC4::ProcessLine( const Tau::String& line )
    {
        Room room( line );

        if( room.Real )
        {
            ResultA += room.SectorID;
        }

        if( Tau::StringUtils::Has( room.DecryptedName, "north" ) )
        {
            TAU_ASSERT( ResultB == 0 )
            ResultB = room.SectorID;
        }

        return true;
    }

}
