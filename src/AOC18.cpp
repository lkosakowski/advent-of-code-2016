// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC18.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class Floor
    {
    public:
        Floor( int rows, const Tau::String& firstRow )
        {
            TAU_ASSERT( rows > 0 )
            TAU_ASSERT( firstRow.GetLength() > 0 )

            Tau::ContainerUtils::EnsureSize( m_traps, rows, firstRow.GetLength() );

            for( int x = 0; x < firstRow.GetLength(); x++ )
            {
                m_traps[0][x] = IsTrap( firstRow[x] );
            }

            for( int y = 1; y < rows; y++ )
            {
                FillRow( m_traps[y], m_traps[y - 1] );
            }
        }

        int CountSafeTiles() const
        {
            int count = 0;

            for( int y = 0; y < m_traps.Size(); y++ )
            {
                for( int x = 0; x < m_traps[y].Size(); x++ )
                {
                    if( !m_traps[y][x] ) { count++; }
                }
            }

            return count;
        }

        void Save( const Tau::String& fileName )
        {
            Tau::String floor;

            for( int y = 0; y < m_traps.Size(); y++ )
            {
                for( int x = 0; x < m_traps[y].Size(); x++ )
                {
                    floor += m_traps[y][x] ? '^' : '.';
                }

                floor += '\n';
            }

            Tau::File::Write( fileName, floor );
        }

    private:
        static void FillRow( Tau::Vector< int >& row, const Tau::Vector< int >& prev )
        {
            TAU_ASSERT( row.Size() == prev.Size() )

            row.First() = IsTrap( false, prev[0], prev[1] );

            for( int x = 1; x < row.Size() - 1; x++ )
            {
                row[x] = IsTrap( prev[x - 1], prev[x], prev[x + 1] );
            }

            row.Last() = IsTrap( prev[ prev.Size() - 2 ], prev.Last(), false );
        }

        static bool IsTrap( char c )
        {
            switch( c )
            {
                case '.': return false;
                case '^': return true;

                default:
                    TAU_ASSERT( false )
                    return false;
            }
        }

        static bool IsTrap( int left, int center, int right )
        {
            return ( left && center && !right ) || ( !left && center && right ) ||
                   ( left && !center && !right ) || ( !left && !center && right );
        }

        Tau::Vector< Tau::Vector< int > > m_traps;
    };

    void AOC18::Solve( const Tau::String& fileName )
    {
        static const int RowsA = 40;
        static const int RowsB = 400000;

        Tau::String input = Tau::File::Read( fileName );

        ResultA = Floor( RowsA, input ).CountSafeTiles();
        ResultB = Floor( RowsB, input ).CountSafeTiles();
    }

}
