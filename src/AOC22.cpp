// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC22.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/file/File.hpp"
#include "core/math/Combination.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/algorithm/Dijkstra.hpp"
#include "core/image/ImageUtils.hpp"
#include "core/image/PngImage.hpp"

namespace AOC
{

    class Node: public Tau::Object
    {
    public:
        Node( const Tau::String& name, int size, int used, int available, int use, const Tau::Index2D& position )
            : Name( name )
            , Size( size )
            , Used( used )
            , Available( available )
            , Use( use )
            , Position( position )
        {
            TAU_ASSERT( Name.m_v.HasLength() )
            TAU_ASSERT( Size > 0 )
            TAU_ASSERT( Used >= 0 && Used <= Size )
            TAU_ASSERT( Available >= 0 && Used + Available == Size )
            TAU_ASSERT( Use >= 0 && ( 100 * Tau::Real( Used ) / Size ).Int() == Use )
        }

        bool IsViable( const Node* B ) const
        {
            TAU_ASSERT( this != B )
            return Used > 0 && Used <= B->Available;
        }

        bool IsCopyable( const Node* B ) const
        {
            TAU_ASSERT( this != B )
            return Used <= B->Size;
        }

        Property::String< Node > Name;
        Property::Int< Node > Size;
        Property::Int< Node > Used;
        Property::Int< Node > Available;
        Property::Int< Node > Use;
        Property::Index2D< Node > Position;
    };

    TAU_SMART_PTR( Node )

    // ---------------- //

    typedef Algorithm::Dijkstra< Node*, int > Dijkstra;
    typedef Dijkstra::Graph Graph;

    // ---------------- //

    static int GetValue( const Tau::String& string )
    {
        TAU_ASSERT( Tau::StringUtils::EndsWith( string, "T" ) || Tau::StringUtils::EndsWith( string, "%" ) )
        return Tau::StringUtils::To< int >( Tau::StringUtils::TrimBack( string, 1 ) );
    }

    static Tau::Index2D GetPosition( const Tau::String& name )
    {
        return Tau::Index2D( Tau::StringUtils::To< int >( Tau::StringUtils::GetBetween( name, "/dev/grid/node-x", "-y", false ) ),
                             Tau::StringUtils::To< int >( Tau::StringUtils::GetAfter( name, "-y", false ) ) );
    }

    template< class P >
    static void TryConnect( Graph::Node* node, const Tau::Vector< Tau::Vector< Tau::Pointer< Graph::Node > > >& grid, int x, int y, P predicate )
    {
        Tau::Nullable< Tau::Pointer< Graph::Node > > neighbour = Tau::ContainerUtils::TryGet( grid, y, x );

        if( neighbour.IsSet() && predicate( neighbour.Get() ) )
        {
            const Node* node_ = node->GetData();
            const Node* neighbour_ = neighbour.Get()->GetData();

            if( node_->IsCopyable( neighbour_ ) ) { node->Link( neighbour.Get(), 1 ); }
            if( neighbour_->IsCopyable( node_ ) ) { neighbour.Get()->Link( node, 1 ); }
        }
    }

    static void DrawPath( Tau::Image* image, const Dijkstra::PathResult& result )
    {
        Tau::List< Graph::Node* > path;
        result->ExtractPath( path );

        for( Tau::List< Graph::Node* >::const_iterator it = path.begin(); it != path.end(); it++ )
        {
            const Tau::Index2D& index = (*it)->GetData()->Position;
            *(unsigned int*)image->Locate( index.X, index.Y ) = Tau::ImageUtils::ABGR( Tau::Color::GREEN, 1 );
        }
    }

    void AOC22::Solve( const Tau::String& fileName )
    {
        Tau::FilePtr file = Tau::File::Open( fileName );
        TAU_VERIFY( file->ReadLine() == "root@ebhq-gridcenter# df -h" )
        TAU_VERIFY( file->ReadLine() == "Filesystem              Size  Used  Avail  Use%" )

        Tau::Vector< NodePtr > nodes;

        while( true )
        {
            Tau::String line = file->ReadLine();
            if( !line.HasLength() ) { break; }

            Tau::Vector< Tau::String > parts;
            Tau::StringUtils::SplitWhitespace( line, parts, false );
            TAU_ASSERT( parts.Size() == 5 )
            nodes.Add( NodePtr( new Node( parts[0], GetValue( parts[1] ), GetValue( parts[2] ), GetValue( parts[3] ), GetValue( parts[4] ), GetPosition( parts[0] ) ) ) );
        }

        TAU_FUNCTOR_0( GetNode, Node*, const NodePtr& node, return node )

        Tau::Map_< Node*, Tau::Set< Node* > > viableNodes;

        Tau::Combination< Node* > combination( 2, nodes, GetNode() );
        Tau::Combination< Node* >::Generator generator( combination );
        Tau::Set< Node* > pair;

        while( generator.Next( pair ) )
        {
            TAU_ASSERT( pair.Size() == 2 )

            const Tau::Set< Node* >& p = const_cast< const Tau::Set< Node* >& >( pair );

            if( p.First()->IsViable( p.Last() ) ) { viableNodes.Add( p.Last(), Tau::Set< Node* >(), true ).Add( p.First() ); }
            if( p.Last()->IsViable( p.First() ) ) { viableNodes.Add( p.First(), Tau::Set< Node* >(), true ).Add( p.Last() ); }
        }

        TAU_FUNCTOR_0( GetCount, int, const Tau::Set< Node* >& set, return set.Size(); )
        ResultA = Tau::ContainerUtils::Sum< int >( viableNodes, GetCount() );

        // ---------------- //

        Graph graph;

        Tau::Vector< Tau::Vector< Tau::Pointer< Graph::Node > > > grid;
        TAU_FUNCTOR_0( GetX, int, const NodePtr& node, return node->Position.Val().X )
        TAU_FUNCTOR_0( GetY, int, const NodePtr& node, return node->Position.Val().Y )
        Tau::ContainerUtils::EnsureSize( grid, Tau::ContainerUtils::Max< int >( nodes, GetY() ) + 1, Tau::ContainerUtils::Max< int >( nodes, GetX() ) + 1 );

        for( Tau::Vector< NodePtr >::const_iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            Graph::Node* graphNode = graph.AddNode( Graph::NodePtr( new Graph::Node( *it ) ) );
            const Tau::Index2D& p = (*it)->Position;
            grid[p.Y][p.X] = graphNode;
        }

        for( int y = 0; y < grid.Size(); y++ )
        {
            for( int x = 0; x < grid[y].Size(); x++ )
            {
                TAU_ASSERT( grid[y][x] )
            }
        }

        TAU_ASSERT( viableNodes.Size() == 1 )
        Node* window = viableNodes.begin()->first;
        const Tau::Set< Node* >& copyable = viableNodes.begin()->second;

        Tau::Set< Node* > noncopyable;
        TAU_PREDICATE_1( IsNoncopyable, Node*, const NodePtr& node, return node != p && node->Used > p->Size )
        nodes.Select( noncopyable, IsNoncopyable( window ) );

        TAU_ASSERT( copyable.Size() + noncopyable.Size() + 1 == nodes.Size() )

        Tau::ImagePtr image( new Tau::Image( grid[0].Size(), grid.Size(), Tau::Image::FORMAT_ABGR ) );

        TAU_PREDICATE_1( IsLinkable, const Tau::Set< Node* >&, const Graph::Node* node, return !p.Has( node->GetData() ) )
        IsLinkable isLinkable( noncopyable );

        for( int y = 0; y < grid.Size(); y++ )
        {
            for( int x = 0; x < grid[0].Size(); x++ )
            {
                Graph::Node* graphNode = grid[y][x];
                TryConnect( graphNode, grid, x - 1, y, isLinkable );
                TryConnect( graphNode, grid, x, y - 1, isLinkable );
            }
        }

        for( int y = 0; y < grid.Size(); y++ )
        {
            for( int x = 0; x < grid[0].Size(); x++ )
            {
                Graph::Node* graphNode = grid[y][x];
                Node* node = graphNode->GetData();
                bool isWindow = node == window;
                bool isCopyable = copyable.Has( node );
                bool isNoncopyable = noncopyable.Has( node );
                TAU_ASSERT( Math::Xor( isWindow, isCopyable, isNoncopyable ) )

                TAU_ASSERT( graphNode->GetNeighbours().Size() <= 4 )
                Tau::Real m = Tau::Real( 1 + graphNode->GetNeighbours().Size() ) / 5;
                unsigned int& p = *(unsigned int*)image->Locate( x, y );
                p = Tau::ImageUtils::ABGR( ( isWindow ? Tau::Color::BLUE : isCopyable ? Tau::Color::WHITE : Tau::Color::RED ) * m, 1 );
            }
        }

        Graph::Node* target = grid[0].GetElement( -2 );

        TAU_PREDICATE_1( IsWindow, Node*, const Graph::NodePtr& node, return node->GetData() == p )
        Graph::Node* windowNode = graph.GetNode( IsWindow( window ) );

        Dijkstra::PathResult path = Dijkstra::FindPath( graph, windowNode, target );

        ResultB = path->GetDistance() + target->GetData()->Position.Val().X * 5 + 1;

        if( false ) // images
        {
            Tau::PngImage::Save( image, "grid1.png" );
            DrawPath( image, path );
            Tau::PngImage::Save( image, "grid2.png" );
        }
    }

}
