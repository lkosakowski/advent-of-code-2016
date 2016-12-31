// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC13.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/image/PngImage.hpp"
#include "core/algorithm/Dijkstra.hpp"

namespace AOC
{

    static int Hash( const Tau::Index2D& index )
    {
        return ( index.X << 16 ) | index.Y;
    }

    // ---------------- //

    class Maze
    {
    public:
        Maze( int seed )
            : Seed( seed )
        {}

        bool IsWall( int x, int y ) const
        {
            TAU_ASSERT( x >= 0 && x >= 0 )
            int value = x * x + 3 * x + 2 * x * y + y + y * y + Seed;
            return Math::PopCount( value ) % 2 != 0;
        }

        typedef Tau::Graph< Tau::Index2D, int > Graph;
        typedef Tau::SmartPtr< Graph > GraphPtr;

        GraphPtr GenerateGraph( const Tau::Index2D& size ) const
        {
            GraphPtr graph( new Graph() );
            Tau::Map_< int, Graph::Node* > nodes;

            for( int y = 0; y < size.Y; y++ )
            {
                for( int x = 0; x < size.X; x++ )
                {
                    if( !IsWall( x, y ) )
                    {
                        Tau::Index2D index( x, y );
                        Graph::NodePtr node( Graph::NodePtr( new Graph::Node( index ) ) );
                        graph->AddNode( node );
                        nodes.Add( Hash( index ), node );
                    }
                }
            }

            struct NodeOperation
            {
                static void TryLink( Graph::Node* node, const Tau::Map_< int, Graph::Node* >& nodes, const Tau::Index2D& index )
                {
                    Graph::Node* neighbour = nodes.TryGet( Hash( index ), NULL );

                    if( neighbour )
                    {
                        node->Link( neighbour, 1 );
                    }
                }
            };

            for( int y = 0; y < size.Y; y++ )
            {
                for( int x = 0; x < size.X; x++ )
                {
                    Tau::Index2D index( x, y );
                    Graph::Node* node = nodes.TryGet( Hash( index ), NULL );

                    if( node )
                    {
                        NodeOperation::TryLink( node, nodes, index.DecX() );
                        NodeOperation::TryLink( node, nodes, index.IncY() );
                        NodeOperation::TryLink( node, nodes, index.IncX() );
                        NodeOperation::TryLink( node, nodes, index.DecY() );
                    }
                }
            }

            return graph;
        }

        Tau::ImagePtr GenerateImage( const Tau::Index2D& size ) const
        {
            Tau::ImagePtr image( new Tau::Image( size.X, size.Y, Tau::Image::FORMAT_GRAY ) );

            char* p = *image;

            for( int y = 0; y < image->Height; y++ )
            {
                for( int x = 0; x < image->Width; x++ )
                {
                    *p++ = IsWall( x, y ) ? 0 : 255;
                }
            }

            return image;
        }

        Property::Int< Maze > Seed;
    };

    // ---------------- //

    typedef Algorithm::Dijkstra< Tau::Index2D, int > Dijkstra;

    TAU_PREDICATE_1( FindGraphNode, Tau::Index2D, const Maze::Graph::NodePtr& node, return node->GetData() == p )
    TAU_PREDICATE_1( FindResultNode, Tau::Index2D, const Dijkstra::TreeResult::value_type& result, return result.first->GetData() == p )
    TAU_PREDICATE_1( SelectResultNodes, int, const Dijkstra::TreeResult::value_type& result, return result.second->GetDistance() <= p )

    void AOC13::Solve( const Tau::String& fileName )
    {
        static const Tau::Index2D Start( 1, 1 );
        static const Tau::Index2D End( 31, 39 );
        static const Tau::Index2D Size( 52, 52 );

        Maze maze( Tau::StringUtils::To< int >( Tau::File::Read( fileName ) ) );

        Maze::GraphPtr graph = maze.GenerateGraph( Size );

        Dijkstra::TreeResult fullResult;
        Dijkstra::FindTree( *graph, graph->GetNode( FindGraphNode( Start ) ), fullResult );
        const Dijkstra::NodeResult* endResult = fullResult.PredicateGet( FindResultNode( End ) );
        ResultA = endResult->GetDistance();

        Dijkstra::TreeResult subset;
        fullResult.Select( subset, SelectResultNodes( 50 ) );
        ResultB = subset.Size();

        if( false ) // images
        {
            struct Render
            {
                static void Path( const Dijkstra::NodeResult* result, Tau::Image* image )
                {
                    Tau::List< Maze::Graph::Node * > path;
                    result->ExtractPath( path );

                    for( Tau::List< Maze::Graph::Node* >::const_iterator it = path.begin(); it != path.end(); it++ )
                    {
                        *(unsigned char*)image->Locate( (*it)->GetData().X, (*it)->GetData().Y ) = 128;
                    }
                }
            };

            Tau::ImagePtr imageA = maze.GenerateImage( Size );
            Tau::PngImage::Save( imageA, "maze.png" );

            Render::Path( endResult, imageA );
            Tau::PngImage::Save( imageA, "mazeA.png" );

            Tau::ImagePtr imageB = maze.GenerateImage( Size );

            for( Dijkstra::TreeResult::const_iterator it = subset.begin(); it != subset.end(); it++ )
            {
                Render::Path( it->second, imageB );
            }

            Tau::PngImage::Save( imageB, "mazeB.png" );
        }
    }

}
