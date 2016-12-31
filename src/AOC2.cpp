// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC2.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    void Numpad::Move( const Tau::Index2D& direction )
    {
        Tau::Index2D position = Position + direction;

        if( IsValidPosition( position ) )
        {
            Position = position;
        }
    }

    // ---------------- //

    char NumpadA::GetCurrentLabel() const
    {
        return '5' + Position.Val().X - Position.Val().Y * 3;
    }

    bool NumpadA::IsValidPosition( const Tau::Index2D& p )
    {
        return abs( p.X ) <= 1 && abs( p.Y ) <= 1;
    }

    // ---------------- //

    template< unsigned int H >
    class DiamondNumpad
    {
    public:
        static bool IsValidPosition( int x, int y )
        {
            return abs( x ) + abs( y ) < H;
        }

        static char GetLabel( int x, int y )
        {
            static const Tau::String Labels = "0123456789ABCDEF";

            TAU_MSG_ASSERT( H <= 3, "Provide more labels." )
            TAU_ASSERT( IsValidPosition( x, y ) )
            return Labels[ MiddleDigit( H ) + x - Math::Sign( y ) * ( MiddleDigit( H ) - MiddleDigit( H - abs( y ) ) ) ];
        }

    private:
        static int MiddleDigit( int layer )
        {
            TAU_ASSERT( layer >= 1 )
            return 1 + layer * ( layer - 1 );
        }
    };

    // ---------------- //

    NumpadB::NumpadB()
        : Numpad( Tau::Index2D( -2, 0 ) )
    {}

    char NumpadB::GetCurrentLabel() const
    {
        return DiamondNumpad< 3 >::GetLabel( Position.Val().X, Position.Val().Y );
    }

    bool NumpadB::IsValidPosition( const Tau::Index2D& p )
    {
        return DiamondNumpad< 3 >::IsValidPosition( p.X, p.Y );
    }

    // ---------------- //

    void AOC2::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC2, AppendDigit, const Tau::String& ) );
    }

    bool AOC2::AppendDigit( const Tau::String& steps )
    {
        for( int i = 0; i < steps.GetLength(); i++ )
        {
            Tau::Index2D direction = GetDirection( steps[i] );
            m_numpadA.Move( direction );
            m_numpadB.Move( direction );
        }

        ResultA += m_numpadA.GetCurrentLabel();
        ResultB += m_numpadB.GetCurrentLabel();
        return true;
    }

    Tau::Index2D AOC2::GetDirection( char c )
    {
        switch( c )
        {
            case 'U': return Tau::Index2D::J;
            case 'R': return Tau::Index2D::I;
            case 'D': return -Tau::Index2D::J;
            case 'L': return -Tau::Index2D::I;

            default:
                TAU_MSG_ASSERT( false, "Unexpected direction: " << c )
                return Tau::Index2D::ZERO;
        }
    }

}
