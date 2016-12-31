// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC7.hpp"

#include "core/file/File.hpp"
#include "core/container/Queue.hpp"
#include "core/container/Set.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class ABBA
    {
    public:
        ABBA( const Tau::String& abba )
            : m_abba( abba )
        {
            TAU_ASSERT( abba.GetLength() == 4 )
            TAU_ASSERT( abba[0] != abba[1] && abba[0] == abba[3] && abba[1] == abba[2] )
        }

        bool operator<( const ABBA& abba ) const { return m_abba < abba.m_abba; }

    private:
        Tau::String m_abba;
    };

    // ---------------- //

    class ABA
    {
    public:
        ABA( const Tau::String& aba )
            : m_aba( aba )
        {
            TAU_ASSERT( aba.GetLength() == 3 )
            TAU_ASSERT( aba[0] != aba[1] && aba[0] == aba[2] )
        }

        bool operator<( const ABA& aba ) const { return m_aba < aba.m_aba; }

        ABA ToBAB() const { return Tau::StringUtils::TrimFront( m_aba, 1 ) + m_aba[1]; }

    private:
        Tau::String m_aba;
    };

    // ---------------- //

    class Window
    {
    public:
        void Consume( char c )
        {
            TAU_ASSERT( Parser::islower( c ) )

            Tau::Pointer< char > prev = m_queue.IsEmpty() ? NULL : &m_queue.GetBack();
            m_queue.AddBack( c );
            TAU_ASSERT( m_queue.Size() <= 4 )

            if( m_queue.Size() == 2 && m_queue.GetBack() == *prev )
            {
                m_queue.RemoveFront();
                return;
            }

            if( m_queue.Size() == 3 )
            {
                if( SameEnds() )
                {
                    TAU_ASSERT( m_queue.GetBack() != *prev )
                    ABAs.Add( GetString(), true );
                }

                if( m_queue.GetBack() != *prev )
                {
                    m_queue.RemoveFront();
                }

                return;
            }

            if( m_queue.Size() == 4 )
            {
                if( SameEnds() )
                {
                    TAU_ASSERT( m_queue.GetBack() != *prev )
                    ABBAs.Add( GetString(), true );
                }

                m_queue.RemoveFront();
                m_queue.RemoveFront();

                if( m_queue.GetBack() == *prev )
                {
                    m_queue.RemoveFront();
                }

                return;
            }
        }

        void Clear()
        {
            m_queue.Clear();
        }

        Tau::Set< ABBA > ABBAs;
        Tau::Set< ABA > ABAs;

    private:
        Tau::Deque< char > m_queue;

        Tau::String GetString() const
        {
            Tau::String string;
            Tau::Deque< char > copy = m_queue;

            while( !copy.IsEmpty() ) { string += copy.PopFront(); }

            return string;
        }

        bool SameEnds() const
        {
            return m_queue.GetFront() == m_queue.GetBack();
        }
    };

    // ---------------- //

    void AOC7::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC7, ProcessLine, const Tau::String& ) );
    }

    static void SwitchWindow( Window*& currentWindow, Window& window )
    {
        TAU_ASSERT( currentWindow != &window )
        currentWindow = &window;
        currentWindow->Clear();
    }

    static ABA ToBAB( const ABA& aba )
    {
        return aba.ToBAB();
    }

    bool AOC7::ProcessLine( const Tau::String& line )
    {
        Window regularWindow, hypernetWindow;
        Window* currentWindow = &regularWindow;

        for( int i = 0; i < line.GetLength(); i++ )
        {
            char c = line[i];

            if( c == '[' )
            {
                SwitchWindow( currentWindow, hypernetWindow );
                continue;
            }

            if( c == ']' )
            {
                SwitchWindow( currentWindow, regularWindow );
                continue;
            }

            currentWindow->Consume( c );
        }

        if( !regularWindow.ABBAs.IsEmpty() && hypernetWindow.ABBAs.IsEmpty() )
        {
            ResultA++;
        }

        Tau::Set< ABA > BABs;
        regularWindow.ABAs.Cast( BABs, ToBAB );

        if( !( BABs * hypernetWindow.ABAs ).IsEmpty() )
        {
            ResultB++;
        }

        return true;
    }

}
