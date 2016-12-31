// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC8_HPP__
#define __INCLUDED_AOC8_HPP__

#include "core/lang/Object.hpp"
#include "core/lang_container/Factory.hpp"
#include "core/math/Index2D.hpp"
#include "core/image/Image.hpp"

namespace AOC
{

    class Operation;

    // ---------------- //

    class AOC8: public Tau::Object
    {
    public:
        AOC8();

        void Solve( const Tau::String& fileName );

        Property::Int< AOC8 > ResultA;
        Property::SmartPtr< Tau::Image, AOC8 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        Tau::ImagePtr m_image;
        Tau::Factory1< Operation, const Tau::String& > m_operations;
    };

}

#endif
