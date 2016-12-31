// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC14.hpp"

#include "core/container/List.hpp"
#include "core/container/Queue.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

#include "md5.hh"

namespace AOC
{

    static Tau::String CalcMD5( const Tau::String& string )
    {
        char* hex_digest = MD5( (unsigned char *)string.Ptr.Val().Raw() ).hex_digest();
        Tau::String md5( hex_digest, 33 );
        delete [] hex_digest;
        return md5;
    }

    // ---------------- //

    class Scanner: public Tau::Object
    {
    public:
        static Tau::SmartPtr< Scanner > Create( const Tau::String& hash, int index )
        {
            Tau::Nullable< char > c = FindTriplet( hash );
            return Tau::SmartPtr< Scanner >( c.IsSet() ? new Scanner( hash, index, c.Get() ) : NULL );
        }

        bool FindQuintet( const Tau::String& hash )
        {
            TAU_ASSERT( !Found )

            if( Tau::StringUtils::Has( hash, Quintet ) )
            {
                Found = true;
                return true;
            }

            return false;
        }

        Property::String< Scanner > Hash;
        Property::Int< Scanner > Index;
        Property::String< Scanner > Quintet;
        Property::Bool< Scanner > Found;

    private:
        Scanner( const Tau::String& hash, int index, char character )
            : Hash( hash )
            , Index( index )
            , Quintet( Tau::String::FromChar( character ) * 5 )
        {}

        static Tau::Nullable< char > FindTriplet( const Tau::String& string )
        {
            Tau::Deque< char > queue;

            for( int i = 0; i < string.GetLength(); i++ )
            {
                char c = string[i];
                queue.AddBack( c );

                TAU_ASSERT( queue.Size() <= 3 )

                if( queue.Size() == 2 && queue.GetFront() != queue.GetBack() )
                {
                    queue.PopFront();
                }
                else if( queue.Size() == 3 )
                {
                    if( queue.GetFront() == queue.GetBack() )
                    {
                        return c;
                    }

                    queue.PopFront();
                    queue.PopFront();
                }
            }

            return Tau::Nullable< char >::EMPTY;
        }
    };

    TAU_SMART_PTR( Scanner )

    // ---------------- //

    class HashProcessor
    {
    public:
        ScannerPtr Consume( const Tau::String& hash, int index )
        {
            static const int Expiration = 1000;
            static const int Count = 64;

            TAU_PREDICATE_1( Expired, int, const ScannerPtr& scanner, return scanner->Index < p || scanner->Found )
            m_scanners.RemoveIf( Expired( index - Expiration ) );

            for( Tau::List< ScannerPtr >::const_iterator it = m_scanners.begin(); it != m_scanners.end(); it++ )
            {
                if( !(*it)->FindQuintet( hash ) ) { continue; }

                m_hashes.Add( (*it)->Hash );

                if( m_hashes.Size() == Count )
                {
                    return *it;
                }
            }

            ScannerPtr scanner = Scanner::Create( hash, index );

            if( scanner )
            {
                m_scanners.Add( scanner );
            }

            return ScannerPtr();
        }

    private:
        Tau::List< ScannerPtr > m_scanners;
        Tau::Vector< Tau::String > m_hashes;
    };

    // ---------------- //

    static Tau::String HashA( const Tau::String& string )
    {
        return CalcMD5( string );
    }

    static Tau::String HashB( const Tau::String& string )
    {
        static const int Repeat = 2017;

        int repeat = Repeat;
        Tau::String input = string;

        while( repeat-- )
        {
            input = CalcMD5( input );
        }

        return input;
    }

    static int Solve( const Tau::String& salt, Tau::String (*hash)( const Tau::String& input ) )
    {
        HashProcessor processor;
        int index = 0;

        while( true )
        {
            ScannerPtr scanner = processor.Consume( hash( salt + Tau::StringUtils::String( index ) ), index );

            if( scanner )
            {
                return scanner->Index;
            }

            index++;
        }
    }

    void AOC14::Solve( const Tau::String& fileName )
    {
        Tau::String salt = Tau::File::Read( fileName );

        ResultA = AOC::Solve( salt, HashA );
        ResultB = AOC::Solve( salt, HashB );
    }

}
