// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC17.hpp"

#include "core/container/ContainerUtils.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

#include "md5.hh"

namespace AOC
{

    static const Tau::Index2D GridSize( 4, 4 );

    // ---------------- //

    static Tau::String CalcMD5( const Tau::String& string )
    {
        char* hex_digest = MD5( (unsigned char *)string.Ptr.Val().Raw() ).hex_digest();
        Tau::String md5( hex_digest, 33 );
        delete [] hex_digest;
        return md5;
    }

    // ---------------- //

    Path::Path( const Tau::Index2D& initialNode )
    {
        AddNode( initialNode );
    }

    void Path::AddNode( const Tau::Index2D& node )
    {
        if( !m_nodes.IsEmpty() )
        {
            Steps += GetStep( node - m_nodes.Last() );
        }

        m_nodes.Add( node );
    }

    const Tau::Vector< Tau::Index2D >& Path::GetNodes() const
    {
        return m_nodes;
    }

    Tau::String Path::GetHash( const Tau::String& salt ) const
    {
        return CalcMD5( salt + Steps );
    }

    Tau::String Path::GetStep( const Tau::Index2D& diff )
    {
        if( diff == -Tau::Index2D::J ) { return "U"; }
        if( diff == Tau::Index2D::I  ) { return "R"; }
        if( diff == Tau::Index2D::J  ) { return "D"; }
        if( diff == -Tau::Index2D::I ) { return "L"; }

        TAU_ASSERT( false )
        return Tau::String::EMPTY;
    }

    // ---------------- //

    void AOC17::Solve( const Tau::String& fileName )
    {
        Salt = Tau::File::Read( fileName );
        Start = Tau::Index2D::ZERO;
        End = GridSize - Tau::Index2D::ONE;

        Tau::Vector< PathPtr > paths;
        paths.Add( PathPtr( new Path( Start ) ) );

        for( int i = 0; i < paths.Size(); i++ )
        {
            ExplorePath( paths[i], paths );
        }

        ResultA = ShortestPath->Steps;
        ResultB = LongestPath->GetNodes().Size() - 1;
    }

    void AOC17::ExplorePath( Path* path, Tau::Vector< PathPtr >& paths )
    {
        const Tau::Index2D& end = path->GetNodes().Last();
        Tau::String hash = path->GetHash( Salt );

        TryAddPath( path, end.DecY(), hash[0], paths );
        TryAddPath( path, end.IncY(), hash[1], paths );
        TryAddPath( path, end.DecX(), hash[2], paths );
        TryAddPath( path, end.IncX(), hash[3], paths );
    }

    void AOC17::TryAddPath( Path* path, const Tau::Index2D& node, char c, Tau::Vector< PathPtr >& paths )
    {
        if( node.X < 0 || node.Y < 0 || node.X >= GridSize.X || node.Y >= GridSize.Y ) { return; }

        TAU_ASSERT( Parser::isdigit( c ) || ( c >= 'a' && c <= 'f' ) )

        if( c < 'b' || c > 'f' ) { return; }

        PathPtr newPath( new Path( *path ) );
        newPath->AddNode( node );

        if( node == End )
        {
            if( !ShortestPath ) { ShortestPath = newPath; }
            if( !LongestPath || newPath->GetNodes().Size() > LongestPath->GetNodes().Size() ) { LongestPath = newPath; }
        }
        else
        {
            paths.Add( newPath );
        }
    }

}
