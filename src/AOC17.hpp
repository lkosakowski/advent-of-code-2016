// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC17_HPP__
#define __INCLUDED_AOC17_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Vector.hpp"
#include "core/math/Index2D.hpp"

namespace AOC
{

    class Path: public Tau::Object
    {
    public:
        Path( const Tau::Index2D& initialNode );

        void AddNode( const Tau::Index2D& node );

        const Tau::Vector< Tau::Index2D >& GetNodes() const;
        Tau::String GetHash( const Tau::String& salt ) const;

        Property::String< Path > Steps;

    private:
        static Tau::String GetStep( const Tau::Index2D& diff );

        Tau::Vector< Tau::Index2D > m_nodes;
    };

    TAU_SMART_PTR( Path )

    // ---------------- //

    class AOC17: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::String< AOC17 > ResultA;
        Property::Int< AOC17 > ResultB;

    private:
        void ExplorePath( Path* path, Tau::Vector< Tau::SmartPtr< Path > >& paths );
        void TryAddPath( Path* path, const Tau::Index2D& node, char c, Tau::Vector< Tau::SmartPtr< Path > >& paths );

        Property::String< AOC17 > Salt;
        Property::Index2D< AOC17 > Start;
        Property::Index2D< AOC17 > End;

        Property::SmartPtr< Path, AOC17 > ShortestPath;
        Property::SmartPtr< Path, AOC17 > LongestPath;
    };

}

#endif
