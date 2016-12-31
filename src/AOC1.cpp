// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC1.hpp"

#include "core/file/File.hpp"
#include "core/serialization/EnumString.hpp"

namespace AOC
{

    enum Turn
    {
        TURN_LEFT,
        TURN_RIGHT
    };

    TAU_ENUM_STRING( AOC, Turn,
                     "L", TURN_LEFT,
                     "R", TURN_RIGHT,
                     NULL )

    // ---------------- //

    class Player
    {
    public:
        Player()
            : Direction( Tau::Index2D::J )
        {}

        void Turn( Turn turn )
        {
            Direction = turn == TURN_RIGHT ? Tau::Index2D( Direction.m_v.Y, -Direction.m_v.X ) : Tau::Index2D( -Direction.m_v.Y, Direction.m_v.X );
        }

        void Move( int distance )
        {
            Position += Direction * distance;
        }

        Property::Index2D< Player > Position;
        Property::Index2D< Player > Direction;
    };

    // ---------------- //

    class Maneuver
    {
    public:
        Maneuver( const Tau::String& string )
            : Turn_( Enum::Turn::Parse( Tau::StringUtils::GetFront( string, 1 ) ) )
            , Distance( Tau::StringUtils::To< int >( Tau::StringUtils::TrimFront( string, 1 ) ) )
        {}

        void Advance( Player& player ) const
        {
            player.Turn( Turn_ );
            player.Move( Distance );
        }

        Property::Enum< Turn, Maneuver > Turn_;
        Property::Int< Maneuver > Distance;
    };

    // ---------------- //

    static int CalcDistance( const Tau::Index2D& position )
    {
        return abs( position.X ) + abs( position.Y );
    }

    // ---------------- //

    void AOC1::Solve( const Tau::String& fileName )
    {
        Player player;

        Tau::Vector< Tau::String > maneuvers;
        Tau::StringUtils::Split( Tau::File::Read( fileName ), maneuvers, ", ", false );

        Tau::Vector< Tau::Index2D > positions;
        positions.Add( Tau::Index2D::ZERO );
        bool finishedB = false;

        for( Tau::Vector< Tau::String >::const_iterator it = maneuvers.begin(); it != maneuvers.end(); it++ )
        {
            Maneuver maneuver( *it );
            maneuver.Advance( player );

            if( finishedB ) { continue; }

            for( int i = maneuver.Distance - 1; i >= 0; i-- )
            {
                Tau::Index2D position = player.Position - player.Direction * i;

                if( positions.Has( position ) )
                {
                    ResultB = CalcDistance( position );
                    finishedB = true;
                    break;
                }

                positions.Add( position );
            }
        }

        ResultA = CalcDistance( player.Position );
    }

}
