// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC2_HPP__
#define __INCLUDED_AOC2_HPP__

#include "core/lang/Object.hpp"
#include "core/math/Index2D.hpp"

namespace AOC
{

    class Numpad
    {
    public:
        Numpad( const Tau::Index2D& position = Tau::Index2D::ZERO )
            : Position( position )
        {}

        void Move( const Tau::Index2D& direction );

        virtual char GetCurrentLabel() const = 0;

    protected:
        Property::Index2D< Numpad > Position;

        virtual bool IsValidPosition( const Tau::Index2D& p ) = 0;
    };

    // ---------------- //

    class NumpadA: public Numpad
    {
    public:
        virtual char GetCurrentLabel() const _override;

    private:
        virtual bool IsValidPosition( const Tau::Index2D& p ) _override;
    };

    // ---------------- //

    class NumpadB: public Numpad
    {
    public:
        NumpadB();

        virtual char GetCurrentLabel() const _override;

    private:
        virtual bool IsValidPosition( const Tau::Index2D& p ) _override;
    };

    // ---------------- //

    class AOC2: public Tau::Object
    {
    public:
        void Solve( const Tau::String& fileName );

        Property::String< AOC2 > ResultA;
        Property::String< AOC2 > ResultB;

    private:
        NumpadA m_numpadA;
        NumpadB m_numpadB;

        bool AppendDigit( const Tau::String& steps );

        static Tau::Index2D GetDirection( char c );
    };

}

#endif
