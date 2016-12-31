// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_AOC10_HPP__
#define __INCLUDED_AOC10_HPP__

#include "core/lang/Object.hpp"
#include "core/container/Vector.hpp"
#include "core/lang_container/Factory.hpp"

namespace AOC
{

    class Bot;
    class Chip;

    // ---------------- //

    class FactoryObject: public Tau::Object, public Tau::Noncopyable
    {
    public:
        FactoryObject( const Tau::String& id );

        static Tau::String GetType( const Tau::String& id );
        static int GetNumber( const Tau::String& id );
    };

    TAU_SMART_PTR( FactoryObject )

    // ---------------- //

    struct DispatchInfo
    {
        DispatchInfo( Bot* a_bot, Chip* a_low, Chip* a_high ):
            bot( a_bot ), low( a_low ), high( a_high )
        {}

        Bot* bot;
        Chip* low;
        Chip* high;
    };

    // ---------------- //

    class AOC10: public Tau::Object
    {
    public:
        AOC10();

        void Solve( const Tau::String& fileName );

        Property::Int< AOC10 > ResultA;
        Property::Int< AOC10 > ResultB;

    private:
        bool ProcessLine( const Tau::String& line );

        void OnDispatch( const DispatchInfo& dispatchInfo );

        template< class T >
        T* GetObject( const Tau::String& id );

        int GetChipValue( const Tau::String& binNumber );

        Property::Bool< AOC10 > FinishedA;

        Tau::Factory1< FactoryObject, const Tau::String& > m_factory;
        Tau::Map< FactoryObjectPtr > m_objects;
        Tau::Vector< Tau::String > m_gives;
        Tau::Vector< Tau::String > m_goesTo;
    };

}

#endif
