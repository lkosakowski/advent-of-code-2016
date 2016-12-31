// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC21.hpp"

#include "core/file/File.hpp"
#include "core/serialization/EnumString.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class Operation: public Tau::Object
    {
    public:
        virtual void Apply( Tau::String& s ) const = 0;
        virtual Tau::SmartPtr< Operation > GetInverse() const = 0;

    protected:
        static void ValidatePositions( int p1, int p2 )
        {
            TAU_ASSERT( p1 >= 0 )
            TAU_ASSERT( p2 >= 0 )
            TAU_ASSERT( p1 != p2 )
        }

        static void ValidateCharacters( char c1, char c2 )
        {
            TAU_ASSERT( Parser::isalpha( c1 ) )
            TAU_ASSERT( Parser::isalpha( c2 ) )
            TAU_ASSERT( c1 != c2 )
        }
    };

    TAU_SMART_PTR( Operation )

    // ---------------- //

    class SwapPosition: public Operation
    {
    public:
        SwapPosition( int p1, int p2 )
            : P1( p1 )
            , P2( p2 )
        {
            ValidatePositions( P1, P2 );
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            char c = s[P1];
            s = s.Replace( P1, s[P2] ).Replace( P2, c );
        }

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new SwapPosition( P1, P2 ) );
        }

        Property::Int< SwapPosition > P1;
        Property::Int< SwapPosition > P2;
    };

    // ---------------- //

    class SwapLetter: public Operation
    {
    public:
        SwapLetter( char c1, char c2 )
            : C1( c1 )
            , C2( c2 )
        {
            ValidateCharacters( C1, C2 );
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            Tau::String c1 = Tau::String::FromChar( C1 );
            Tau::String c2 = Tau::String::FromChar( C2 );

            s = Tau::StringUtils::ReplaceAll( s, c1, "-", true );
            s = Tau::StringUtils::ReplaceAll( s, c2, c1, true );
            s = Tau::StringUtils::ReplaceAll( s, "-", c2, true );
        }

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new SwapLetter( C1, C2 ) );
        }

        Property::Char< SwapLetter > C1;
        Property::Char< SwapLetter > C2;
    };

    // ---------------- //

    class Rotate: public Operation
    {
    public:
        enum Direction
        {
            ROTATE_LEFT,
            ROTATE_RIGHT
        };

        Rotate( Direction direction, int steps )
            : Steps( steps * GetModifier( direction ) )
        {
            TAU_ASSERT( steps >= 0 )
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            s = Tau::StringUtils::Rotate( s, Steps );
        }

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new Rotate( Steps < 0 ? ROTATE_RIGHT : ROTATE_LEFT, abs( Steps ) ) );
        }

        Property::Int< Rotate > Steps;

    private:
        static int GetModifier( Direction direction )
        {
            switch( direction )
            {
                case ROTATE_LEFT: return -1;
                case ROTATE_RIGHT: return 1;

                default:
                    TAU_ASSERT( false )
                    return 0;
            }
        }
    };

    TAU_ENUM_STRING( Rotate, Direction,
                     "left", Rotate::ROTATE_LEFT,
                     "right", Rotate::ROTATE_RIGHT,
                     NULL )

    // ---------------- //

    class RotateBasedOnPosition: public Operation
    {
    public:
        RotateBasedOnPosition( char c )
            : C( c )
        {
            TAU_ASSERT( Parser::isalpha( C ) )
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            TAU_ASSERT( s.GetLength() == 8 )
            int i = s.FindFront( C ).Get();
            s = Tau::StringUtils::Rotate( s, 1 + i + ( i >= 4 ? 1 : 0 ) );
        }

        class InverseRotateBasedOnPosition: public Operation
        {
        public:
            InverseRotateBasedOnPosition( char c )
                : C( c )
            {
                TAU_ASSERT( Parser::isalpha( C ) )
            }

            virtual void Apply( Tau::String& s ) const _override
            {
                TAU_ASSERT( s.GetLength() == 8 )
                int i = s.FindFront( C ).Get();
                if( i == 0 ) { i = 8; }
                s = Tau::StringUtils::Rotate( s, i % 2 == 0 ? -i / 2 + 3 : -( i + 1 ) / 2 );
            }

            virtual OperationPtr GetInverse() const _override
            {
                return OperationPtr( new RotateBasedOnPosition( C ) );
            }

            Property::Char< InverseRotateBasedOnPosition > C;
        };

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new InverseRotateBasedOnPosition( C ) );
        }

        Property::Char< RotateBasedOnPosition > C;
    };

    // ---------------- //

    class ReversePositions: public Operation
    {
    public:
        ReversePositions( int p1, int p2 )
            : P1( p1 )
            , P2( p2 )
        {
            ValidatePositions( P1, P2 );
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            s = s.SubstringBefore( P1 ) + Tau::StringUtils::Reverse( s.SubstringBetween( P1, P2, true ) ) + s.SubstringAfter( P2 );
        }

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new ReversePositions( P1, P2 ) );
        }

        Property::Int< ReversePositions > P1;
        Property::Int< ReversePositions > P2;
    };

    // ---------------- //

    class MovePosition: public Operation
    {
    public:
        MovePosition( int p1, int p2 )
            : P1( p1 )
            , P2( p2 )
        {
            ValidatePositions( P1, P2 );
        }

        virtual void Apply( Tau::String& s ) const _override
        {
            char c = s[P1];
            s = s.SubstringBefore( P1 ) + s.SubstringAfter( P1 );
            s = s.SubstringBefore( P2 ) + c + s.SubstringAfter( P2, true );
        }

        virtual OperationPtr GetInverse() const _override
        {
            return OperationPtr( new MovePosition( P2, P1 ) );
        }

        Property::Int< ReversePositions > P1;
        Property::Int< ReversePositions > P2;
    };

    // ---------------- //

    template< class T >
    static T GetBetween( const Tau::String& string, const Tau::String& separator1, const Tau::String& separator2 );

    template<>
    int GetBetween( const Tau::String& string, const Tau::String& separator1, const Tau::String& separator2 )
    {
        return Tau::StringUtils::To< int >( Tau::StringUtils::GetBetween( string, separator1, separator2, false ) );
    }

    template<>
    char GetBetween( const Tau::String& string, const Tau::String& separator1, const Tau::String& separator2 )
    {
        Tau::String s = Tau::StringUtils::GetBetween( string, separator1, separator2, false );
        TAU_ASSERT( s.GetLength() == 1 )
        return s[0];
    }

    template< class T >
    static T GetAfter( const Tau::String& string, const Tau::String& separator );

    template<>
    int GetAfter( const Tau::String& string, const Tau::String& separator )
    {
        return Tau::StringUtils::To< int >( Tau::StringUtils::GetAfter( string, separator, false ) );
    }

    template<>
    char GetAfter( const Tau::String& string, const Tau::String& separator )
    {
        Tau::String s = Tau::StringUtils::GetAfter( string, separator, false );
        TAU_ASSERT( s.GetLength() == 1 )
        return s[0];
    }

    // ---------------- //

    static void EnsureAnagram( const Tau::String& a, const Tau::String& b )
    {
        TAU_ASSERT( a.GetLength() == b.GetLength() )

        for( int i = 0; i < a.GetLength(); i++ )
        {
            TAU_ASSERT( b.FindFront( a[i] ).IsSet() )
        }
    }

    void AOC21::Solve( const Tau::String& fileName )
    {
        static const Tau::String PasswordA = "abcdefgh";
        static const Tau::String PasswordB = "fbgdceah";

        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC21, ProcessLine, const Tau::String& ) );

        ResultA = PasswordA;

        for( Tau::Vector< OperationPtr >::const_iterator it = m_operations.begin(); it != m_operations.end(); it++ )
        {
            (*it)->Apply( ResultA.m_v );
            EnsureAnagram( PasswordA, ResultA );
        }

        ResultB = PasswordB;

        for( Tau::Vector< OperationPtr >::const_reverse_iterator it = m_operations.rbegin(); it != m_operations.rend(); it++ )
        {
            (*it)->GetInverse()->Apply( ResultB.m_v );
            EnsureAnagram( PasswordB, ResultB );
        }
    }

    static OperationPtr GetOperation( const Tau::String& line )
    {
        if( Tau::StringUtils::StartsWith( line, "swap position " ) )
        {
            int p1 = GetBetween< int >( line, "swap position ", " with position " );
            int p2 = GetAfter< int >( line, " with position " );
            return OperationPtr( new SwapPosition( p1, p2 ) );
        }
        else if( Tau::StringUtils::StartsWith( line, "swap letter " ) )
        {
            char c1 = GetBetween< char >( line, "swap letter ", " with letter " );
            char c2 = GetAfter< char >( line, " with letter " );
            return OperationPtr( new SwapLetter( c1, c2 ) );
        }
        else if( Tau::StringUtils::StartsWith( line, "rotate based on position of letter " ) )
        {
            char c = GetAfter< char >( line, "rotate based on position of letter " );
            return OperationPtr( new RotateBasedOnPosition( c ) );
        }
        else if( Tau::StringUtils::StartsWith( line, "rotate " ) )
        {
            Tau::String arguments = Tau::StringUtils::GetBetween( line, "rotate ", " steps", true );
            if( !arguments.HasLength() ) { arguments = Tau::StringUtils::GetBetween( line, "rotate ", " steps", false ); }
            Tau::String direction, steps;
            Tau::StringUtils::Split( arguments, direction, ' ', steps, false );

            Rotate::Direction d = Enum::Direction::Parse( direction );
            int s = Tau::StringUtils::To< int >( steps );
            return OperationPtr( new Rotate( d, s ) );
        }
        if( Tau::StringUtils::StartsWith( line, "reverse positions " ) )
        {
            int p1 = GetBetween< int >( line, "reverse positions ", " through " );
            int p2 = GetAfter< int >( line, " through " );
            return OperationPtr( new ReversePositions( p1, p2 ) );
        }
        else if( Tau::StringUtils::StartsWith( line, "move position " ) )
        {
            int p1 = GetBetween< int >( line, "move position ", " to position " );
            int p2 = GetAfter< int >( line, " to position " );
            return OperationPtr( new MovePosition( p1, p2 ) );
        }
        else
        {
            TAU_ASSERT( false )
            return OperationPtr();
        }
    }

    bool AOC21::ProcessLine( const Tau::String& line )
    {
        m_operations.Add( GetOperation( line ) );
        return true;
    }

}
