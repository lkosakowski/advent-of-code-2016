// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC3.hpp"

#include "core/file/File.hpp"
#include "core/serialization/Parser.hpp"
#include "core/math/Triangle.hpp"

namespace AOC
{

    void AOC3::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC3, ProcessLine, const Tau::String& ) );
        TAU_ASSERT( Counter == 0 )
    }

    bool AOC3::ProcessLine( const Tau::String& line )
    {
        Tau::Vector3D sides = Parser::Get< Tau::Vector3D >( line );

        if( Tau::Triangle::IsValid( sides ) )
        {
            ResultA++;
        }

        m_sides[ Counter++ ] = sides;

        if( Counter == 3 )
        {
            if( Tau::Triangle::IsValid( m_sides.GetCol1() ) ) { ResultB++; }
            if( Tau::Triangle::IsValid( m_sides.GetCol2() ) ) { ResultB++; }
            if( Tau::Triangle::IsValid( m_sides.GetCol3() ) ) { ResultB++; }

            Counter = 0;
        }

        return true;
    }

}
