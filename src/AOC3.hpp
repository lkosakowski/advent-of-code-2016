// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC3_HPP__
#define __INCLUDED_AOC3_HPP__

#include "core/lang/Object.hpp"
#include "core/math/Matrix3D.hpp"

namespace AOC
{

    class AOC3: public Tau::Object
    {
    public:
        virtual void Solve( const Tau::String& fileName );

        Property::Int< AOC3 > ResultA;
        Property::Int< AOC3 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        Property::Int< AOC3 > Counter;
        Tau::Matrix3D m_sides;
    };

}

#endif
