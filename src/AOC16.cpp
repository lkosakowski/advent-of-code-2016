// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC16.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class RandomData
    {
    public:
        static Tau::String Generate( int length, const Tau::String& salt )
        {
            Tau::String data = salt;

            while( data.GetLength() < length )
            {
                data = data + '0' + SwapBits( Tau::StringUtils::Reverse( data ) );
            }

            return Tau::StringUtils::GetFront( data, length );
        }

        static Tau::String CalcChecksum( const Tau::String& data )
        {
            TAU_ASSERT( data.GetLength() % 2 == 0 )

            Tau::String checksum;

            for( int i = 0; i < data.GetLength(); i += 2 )
            {
                checksum += data[i] == data[i + 1] ? '1' : '0';
            }

            return checksum.GetLength() % 2 == 0 ? CalcChecksum( checksum ) : checksum;
        }

    private:
        static Tau::String SwapBits( const Tau::String& string )
        {
            Tau::String result;

            for( int i = 0; i < string.GetLength(); i++ )
            {
                char c = string[i];

                switch( c )
                {
                    case '0': result += '1'; break;
                    case '1': result += '0'; break;

                    default:
                        TAU_MSG_ASSERT( false, "Unexpected character." )
                }
            }

            return result;
        }
    };

    // ---------------- //

    Tau::String Solve( int length, const Tau::String& salt )
    {
        return RandomData::CalcChecksum( RandomData::Generate( length, salt ) );
    }

    void AOC16::Solve( const Tau::String& fileName )
    {
        static const int DataLengthA = 272;
        static const int DataLengthB = 35651584;

        Tau::String salt = Tau::File::Read( fileName );

        ResultA = AOC::Solve( DataLengthA, salt );
        ResultB = AOC::Solve( DataLengthB, salt );
    }

}
