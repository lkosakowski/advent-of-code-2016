// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC24.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/math/Permutation.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/algorithm/Dijkstra.hpp"
#include "core/image/PngImage.hpp"
#include "core/image/ImageUtils.hpp"

namespace AOC
{

    static const char Wall = '#';
    static const char Empty = '.';

    typedef Algorithm::Dijkstra< Tau::Index2D, int > Dijkstra;
    typedef Dijkstra::Graph Graph;

    // ---------------- //

    static void EnsureWalls( const Tau::String& line )
    {
        for( int i = 0; i < line.GetLength(); i++ )
        {
            TAU_ASSERT( line[i] == Wall )
        }
    }

    static void TryConnect( Graph::Node* node, const Tau::Vector< Tau::Vector< Tau::Pointer< Graph::Node > > >& grid, int x, int y )
    {
        Tau::Nullable< Tau::Pointer< Graph::Node > > neighbour = Tau::ContainerUtils::TryGet( grid, y, x );

        if( neighbour.IsSet() && neighbour.Get() )
        {
            node->Link( neighbour.Get(), 1, true );
        }
    }

    static Tau::ImagePtr GetImage( const Tau::Vector< Tau::Vector< Tau::Pointer< Graph::Node > > >& grid )
    {
        Tau::ImagePtr image( new Tau::Image( grid[0].Size(), grid.Size(), Tau::Image::FORMAT_ABGR ) );

        for( int y = 0; y < grid.Size(); y++ )
        {
            for( int x = 0; x < grid[0].Size(); x++ )
            {
                *(unsigned int*)image->Locate( x, y ) = Tau::ImageUtils::ABGR( grid[y][x] ? Tau::Color::WHITE : Tau::Color::BLACK, 1 );
            }
        }

        return image;
    }

    static void DrawPath( Tau::Image* image, const Dijkstra::PathResult& result )
    {
        Tau::List< Graph::Node* > path;
        result->ExtractPath( path );

        for( Tau::List< Graph::Node* >::const_iterator it = path.begin(); it != path.end(); it++ )
        {
            const Tau::Index2D& index = (*it)->GetData();
            *(unsigned int*)image->Locate( index.X, index.Y ) = Tau::ImageUtils::ABGR( Tau::Color::GREEN, 1 );
        }
    }

    void AOC24::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC24, ProcessLine, const Tau::String& ) );

        Graph graph;
        Tau::Vector< Tau::Vector< Tau::Pointer< Graph::Node > > > grid;
        Tau::ContainerUtils::EnsureSize( grid, m_lines.Size() - 2, m_lines[0].GetLength() - 2 );

        Tau::Pointer< Graph::Node > start;
        Tau::Vector< Graph::Node* > destinations;

        EnsureWalls( m_lines[0] );
        EnsureWalls( m_lines[m_lines.Size() - 1] );

        for( int y = 1; y < m_lines.Size() - 1; y++ )
        {
            const Tau::String& line = m_lines[y];

            TAU_ASSERT( line.GetLength() == m_lines[0].GetLength() )
            TAU_ASSERT( line[0] == Wall )
            TAU_ASSERT( line[line.GetLength() - 1] == Wall )

            for( int x = 1; x < line.GetLength() - 1; x++ )
            {
                char c = line[x];
                if( c == Wall ) { continue; }
                bool poi = Parser::isdigit( c ) != 0;
                TAU_ASSERT( c == Empty || poi )

                Graph::Node* node = graph.AddNode( Graph::NodePtr( new Graph::Node( Tau::Index2D( x - 1, y - 1 ) ) ) );
                grid[y - 1][x - 1] = node;

                TryConnect( node, grid, x - 2, y - 1 );
                TryConnect( node, grid, x - 1, y - 2 );

                if( poi )
                {
                    if( Parser::ToInt( c ) == 0 )
                    {
                        TAU_ASSERT( !start )
                        start = node;
                    }
                    else
                    {
                        destinations.Add( node );
                    }
                }
            }
        }

        struct Route
        {
            Route( Graph::Node* a_start, Graph::Node* a_end )
                : start( a_start ), end( a_end )
            {}

            bool operator<( const Route& route ) const
            {
                return start == route.start ? end < route.end : start < route.start;
            }

            Graph::Node* start;
            Graph::Node* end;
        };

        Tau::Map_< Route, Dijkstra::PathResult > pathCache;
        Tau::Permutation< Graph::Node* > permutations;

        for( Tau::Vector< Graph::Node* >::const_iterator it = destinations.begin(); it != destinations.end(); it++ )
        {
            pathCache.Add( Route( start, *it ), Dijkstra::FindPath( graph, start, *it ) );
            pathCache.Add( Route( *it, start ), Dijkstra::FindPath( graph, *it, start ) );
            permutations.AddElement( *it );
        }

        permutations.Commit();
        Tau::Permutation< Graph::Node* >::Generator generator( permutations );
        Tau::Vector< Graph::Node* > stops;

        Tau::Vector< Graph::Node* > minStopsA;
        Tau::Vector< Graph::Node* > minStopsB;
        ResultA = Math::Max< int >();
        ResultB = Math::Max< int >();

        while( generator.Next( stops ) )
        {
            int steps = 0;

            for( int i = 1; i < stops.Size(); i++ )
            {
                Route route( stops[i - 1], stops[i] );
                Tau::Map_< Route, Dijkstra::PathResult >::const_iterator it = pathCache.Find( route, true );

                if( it == pathCache.end() )
                {
                    Dijkstra::PathResult result = Dijkstra::FindPath( graph, route.start, route.end );
                    pathCache.Add( route, result );
                    steps += result->GetDistance();
                }
                else
                {
                    steps += it->second->GetDistance();
                }
            }

            int stepsA = pathCache.Get( Route( start, stops.First() ) )->GetDistance() + steps;
            int stepsB = stepsA + pathCache.Get( Route( stops.Last(), start ) )->GetDistance();

            if( stepsA < ResultA )
            {
                ResultA = stepsA;
                minStopsA = stops;
            }

            if( stepsB < ResultB )
            {
                ResultB = stepsB;
                minStopsB = stops;
            }
        }

        if( false ) // images
        {
            // A
            {
                Tau::ImagePtr image = GetImage( grid );
                DrawPath( image, pathCache.Get( Route( start, minStopsA.First() ) ) );

                for( int i = 1; i < minStopsA.Size(); i++ )
                {
                    DrawPath( image, pathCache.Get( Route( minStopsA[i - 1], minStopsA[i] ) ) );
                }

                Tau::PngImage::Save( image, "gridA.png" );
            }

            // B
            {
                Tau::ImagePtr image = GetImage( grid );
                DrawPath( image, pathCache.Get( Route( start, minStopsB.First() ) ) );

                for( int i = 1; i < minStopsB.Size(); i++ )
                {
                    DrawPath( image, pathCache.Get( Route( minStopsB[i - 1], minStopsB[i] ) ) );
                }

                DrawPath( image, pathCache.Get( Route( minStopsB.Last(), start ) ) );
                Tau::PngImage::Save( image, "gridB.png" );
            }
        }
    }

    bool AOC24::ProcessLine( const Tau::String& line )
    {
        m_lines.Add( line );
        return true;
    }

}
