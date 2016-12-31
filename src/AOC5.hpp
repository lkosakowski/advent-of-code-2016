// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC5_HPP__
#define __INCLUDED_AOC5_HPP__

#include "core/lang/Object.hpp"

namespace AOC
{

    class CharArray;

    // ---------------- //

    class AOC5: public Tau::Object
    {
    public:
        AOC5();

        void Solve( const Tau::String& fileName );

        Property::String< AOC5 > ResultA;
        Property::String< AOC5 > ResultB;

    private:
        bool IsFinishedA() const;
        bool IsFinishedB() const;

        Tau::SmartPtr< CharArray > m_charArray;
    };

}

#endif
