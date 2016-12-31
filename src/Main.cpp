// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC1.hpp"
#include "AOC2.hpp"
#include "AOC3.hpp"
#include "AOC4.hpp"
#include "AOC5.hpp"
#include "AOC6.hpp"
#include "AOC7.hpp"
#include "AOC8.hpp"
#include "AOC9.hpp"
#include "AOC10.hpp"
#include "AOC11.hpp"
#include "AOC12.hpp"
#include "AOC13.hpp"
#include "AOC14.hpp"
#include "AOC15.hpp"
#include "AOC16.hpp"
#include "AOC17.hpp"
#include "AOC18.hpp"
#include "AOC19.hpp"
#include "AOC20.hpp"
#include "AOC21.hpp"
#include "AOC22.hpp"
#include "AOC23.hpp"
#include "AOC24.hpp"
#include "AOC25.hpp"

#include "core/time/Clock.hpp"
#include "core/file/FileUtils.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/image/PngImage.hpp"

#define AOC_LOG( msg ) { TAU_DEBUG_MESSAGE( msg ) TAU_SDK_MSG( debugMessage ) }

namespace AOC
{

    static const unsigned char PngImage_UPOJFLBCEZ[] =
    {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
        0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0xA4, 0xBF, 0x92,
        0xD6, 0x00, 0x00, 0x00, 0x53, 0x49, 0x44, 0x41, 0x54, 0x18, 0x95, 0x75, 0x8F, 0x41, 0x0E, 0xC0,
        0x40, 0x08, 0x02, 0x87, 0xA6, 0xFF, 0xFF, 0x32, 0x3D, 0xA0, 0xAE, 0x49, 0x5B, 0x0F, 0x06, 0x50,
        0x97, 0x45, 0x16, 0x96, 0x41, 0x60, 0x04, 0x18, 0x19, 0x90, 0x43, 0x7A, 0x10, 0x0D, 0x02, 0xBD,
        0x1A, 0x85, 0x06, 0xAF, 0x29, 0x51, 0x6E, 0xBE, 0x2A, 0x7E, 0x0B, 0x9D, 0x3F, 0xA8, 0x4F, 0xCE,
        0x0E, 0xB5, 0x31, 0xC8, 0x5A, 0x9A, 0x05, 0x57, 0xF1, 0xF3, 0xEA, 0x8F, 0x2D, 0x7D, 0x81, 0x92,
        0x4D, 0x63, 0x35, 0xB4, 0xF3, 0xBE, 0xE2, 0x3F, 0xBD, 0x24, 0x2E, 0x02, 0x33, 0xB9, 0xDD, 0xB1,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
    };

    // ---------------- //

    static bool Equal( long long a, long long b ) { return a == b; }
    static bool Equal( const Tau::String& a, const Tau::String& b ) { return a == b; }
    static bool Equal( const Tau::ImagePtr& a, const Tau::ImagePtr& b ) { return Tau::Image::Compare( a, b ); }

    template< class A, class R1, class R2 >
    static void Expect( const Tau::String& fileName, const R1& resultA, const R2& resultB )
    {
        Tau::Clock clock;

        A aoc;
        aoc.Solve( fileName );
        TAU_ASSERT( Equal( aoc.ResultA, resultA ) )
        TAU_ASSERT( Equal( aoc.ResultB, resultB ) )

        AOC_LOG( Tau::FileUtils::ExtractName( fileName ) << ": " << Tau::StringUtils::String( clock.GetTime() ) )
    }

    static void Test()
    {
        Expect< AOC1 >( "AOC1.txt", 301, 130 );
        Expect< AOC2 >( "AOC2.txt", "47978", "659AD" );
        Expect< AOC3 >( "AOC3.txt", 1050, 1921 );
        Expect< AOC4 >( "AOC4.txt", 409147, 991 );
        Expect< AOC5 >( "AOC5.txt", "f77a0e6e", "999828ec" );
        Expect< AOC6 >( "AOC6.txt", "gyvwpxaz", "jucfoary" );
        Expect< AOC7 >( "AOC7.txt", 110, 242 );
        Expect< AOC8 >( "AOC8.txt", 116, Tau::PngImage::Load( PngImage_UPOJFLBCEZ, sizeof( PngImage_UPOJFLBCEZ ) ) );
        Expect< AOC9 >( "AOC9.txt", 99145, 10943094568 );
        Expect< AOC10 >( "AOC10.txt", 161, 133163 );
        Expect< AOC11 >( "AOC11.txt", 31, 55 );
        Expect< AOC12 >( "AOC12.txt", 318077, 9227731 );
        Expect< AOC13 >( "AOC13.txt", 96, 141 );
        Expect< AOC14 >( "AOC14.txt", 16106, 22423 );
        Expect< AOC15 >( "AOC15.txt", 122318, 3208583 );
        Expect< AOC16 >( "AOC16.txt", "10101001010100001", "10100001110101001" );
        Expect< AOC17 >( "AOC17.txt", "RDRLDRDURD", 596 );
        Expect< AOC18 >( "AOC18.txt", 1951, 20002936 );
        Expect< AOC19 >( "AOC19.txt", 1808357, 1407007 );
        Expect< AOC20 >( "AOC20.txt", 19449262, 119 );
        Expect< AOC21 >( "AOC21.txt", "gfdhebac", "dhaegfbc" );
        Expect< AOC22 >( "AOC22.txt", 993, 202 );
        Expect< AOC23 >( "AOC23.txt", 12000, 479008560 );
        Expect< AOC24 >( "AOC24.txt", 462, 676 );
        Expect< AOC25 >( "AOC25.txt", 158, 0 );
    }

}

int main( int argc, char* argv[] )
{
    AOC::Test();
    return 0;
}
