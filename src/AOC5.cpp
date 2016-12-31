// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC5.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/time/Clock.hpp"
#include "core/stream/Stdio.hpp"

#include "md5.hh"

#define AOC_LOG( msg ) // { TAU_DEBUG_MESSAGE( msg ) TAU_SDK_MSG( debugMessage ) }

namespace AOC
{

    static const int PasswordLength = 8;

    static Tau::String CalcMD5( const Tau::String& string )
    {
        char* hex_digest = MD5( (unsigned char *)string.Ptr.Val().Raw() ).hex_digest();
        Tau::String md5( hex_digest, 33 );
        delete [] hex_digest;
        return md5;
    }

    // ---------------- //

    class CharArray: public Tau::Object
    {
    public:
        CharArray( int count )
        {
            m_array.EnsureSize( count );
        }

        void Insert( char c, int p )
        {
            if( p >= 0 && p < m_array.Size() && m_array[p] == 0 )
            {
                m_array[p] = c;
            }
        }

        Tau::String GetString() const
        {
            Tau::String s;

            for( Tau::Vector< char >::const_iterator it = m_array.begin(); it != m_array.end(); it++ )
            {
                s += *it;
            }

            return s;
        }

        bool IsFull() const
        {
            return !m_array.Has( '\0' );
        }

    private:
        Tau::Vector< char > m_array;
    };

    TAU_SMART_PTR( CharArray )

    // ---------------- //

    AOC5::AOC5()
        : m_charArray( new CharArray( PasswordLength ) )
    {}

    void AOC5::Solve( const Tau::String& fileName )
    {
        Tau::String doorID = Tau::File::Read( fileName );
        int index = 0;

        Tau::Clock c;

        while( true )
        {
            Tau::String md5 = CalcMD5( doorID + Tau::StringUtils::String( index ) );

            if( Tau::StringUtils::StartsWith( md5, "00000" ) )
            {
                AOC_LOG( "Found: " << md5 << ", index: " << index << ", time: " << Tau::StringUtils::String( c.GetTime() ) )

                if( !IsFinishedA() ) { ResultA += md5[5]; }
                if( !IsFinishedB() ) { m_charArray->Insert( md5[6], md5[5] - '0' ); }
                if( IsFinishedA() && IsFinishedB() ) { break; }
            }

            index++;
        }

        AOC_LOG( "Time: " << Tau::StringUtils::String( c.GetTime() ) )
        AOC_LOG( "Iterations: " << index )
        AOC_LOG( "Average 1k: " << Tau::StringUtils::String( Tau::Time( 1000 * c.GetTime().GetTotalS() / index ) ) )

        ResultB = m_charArray->GetString();

        AOC_LOG( "A: " << ResultA )
        AOC_LOG( "B: " << ResultB )
    }

    bool AOC5::IsFinishedA() const
    {
        return ResultA.m_v.GetLength() == PasswordLength;
    }

    bool AOC5::IsFinishedB() const
    {
        return m_charArray->IsFull();
    }

}
