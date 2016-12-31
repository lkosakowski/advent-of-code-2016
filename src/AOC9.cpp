// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC9.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    void AOC9::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC9, ProcessLine, const Tau::String& ) );
    }

    static long long GetDecompressedLength( const Tau::String& string, bool recurse )
    {
        long long result = 0;

        const char* p = string.Ptr;
        const char* end = string.Ptr + string.GetLength();

        while( p < end )
        {
            TAU_ASSERT( !Parser::isspace( *p ) )

            if( *p == '(' )
            {
                Tau::String marker = Tau::StringUtils::GetBefore( Tau::String( p + 1 ), ')', false );
                Tau::String lengthString, repeatString;
                Tau::StringUtils::Split( marker, lengthString, 'x', repeatString, false );
                int length = Tau::StringUtils::To< int >( lengthString );
                int repeat = Tau::StringUtils::To< int >( repeatString );
                p += 2 + marker.GetLength();
                result += repeat * ( recurse ? GetDecompressedLength( Tau::String( p, length ), true ) : length );
                p += length;
            }
            else
            {
                TAU_ASSERT( *p != ')' )
                result++;
                p++;
            }
        }

        return result;
    }

    bool AOC9::ProcessLine( const Tau::String& line )
    {
        ResultA = GetDecompressedLength( line, false );
        ResultB = GetDecompressedLength( line, true );
        return true;
    }

}
