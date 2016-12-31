// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC19.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class Elf
    {
    public:
        Elf( int number )
            : Number( number )
            , Presents( 1 )
        {
            TAU_ASSERT( number > 0 )
        }

        void Rob( Elf& victim )
        {
            TAU_ASSERT( victim.Presents > 0 )
            Presents += victim.Presents;
        }

        Property::Int< Elf > Number;
        Property::Int< Elf > Presents;
    };

    // ---------------- //

    static Tau::List< Elf >::iterator NextIterator( Tau::List< Elf >& list, Tau::List< Elf >::iterator it )
    {
        it++;
        return it != list.end() ? it : list.begin();
    }

    static Tau::List< Elf >::iterator OppositeIterator( Tau::List< Elf >& list, Tau::List< Elf >::iterator it )
    {
        int steps = list.Size() / 2;

        while( steps-- )
        {
            it++;
            if( it == list.end() ) { it = list.begin(); }
        }

        return it;
    }

    void AddElfs( Tau::List< Elf >& list, int count )
    {
        for( int i = 0; i < count; i++ )
        {
            list.Add( Elf( i + 1 ) );
        }
    }

    void AOC19::Solve( const Tau::String& fileName )
    {
        int elfCount = Tau::StringUtils::To< int >( Tau::File::Read( fileName ) );

        // A
        {
            Tau::List< Elf > elfs;
            AddElfs( elfs, elfCount );

            Tau::List< Elf >::iterator current = elfs.begin();

            while( elfs.Size() > 1 )
            {
                Tau::List< Elf >::iterator next = NextIterator( elfs, current );
                current->Rob( *next );
                current = NextIterator( elfs, next );
                elfs.Erase( next );
            }

            ResultA = current->Number;
        }

        // B
        {
            Tau::List< Elf > elfs;
            AddElfs( elfs, elfCount );

            Tau::List< Elf >::iterator current = elfs.begin();
            Tau::List< Elf >::iterator victim = OppositeIterator( elfs, current );

            while( elfs.Size() > 1 )
            {
                Tau::List< Elf >::iterator next = NextIterator( elfs, current );
                current->Rob( *victim );
                current = victim == next ? NextIterator( elfs, next ) : next;
                Tau::List< Elf >::iterator prevVictim = victim;
                victim = NextIterator( elfs, victim );
                if( elfs.Size() % 2 == 1 ) { victim = NextIterator( elfs, victim ); }
                elfs.Erase( prevVictim );
            }

            ResultB = current->Number;
        }
    }

}
