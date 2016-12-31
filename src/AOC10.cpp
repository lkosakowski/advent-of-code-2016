// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC10.hpp"

#include "core/lang_container/Event.hpp"
#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

#define FACTORY_METHOD( Class )                         \
                                                        \
static FactoryObjectPtr Create( const Tau::String& id ) \
{                                                       \
    return FactoryObjectPtr( new Class( id ) );         \
}

#define AOC_MSG( msg ) // TAU_MSG( msg )

namespace AOC
{

    FactoryObject::FactoryObject( const Tau::String& id )
        : Tau::Object( id )
    {}

    Tau::String FactoryObject::GetType( const Tau::String& id )
    {
        Tau::String type, number;
        Tau::StringUtils::Split( id, type, ' ', number, false );
        return type;
    }

    int FactoryObject::GetNumber( const Tau::String& id )
    {
        Tau::String type, number;
        Tau::StringUtils::Split( id, type, ' ', number, false );
        return Tau::StringUtils::To< int >( number );
    }

    // ---------------- //

    class Chip: public FactoryObject
    {
    public:
        FACTORY_METHOD( Chip )

        Property::Int< Chip > Value;

    private:
        Chip( const Tau::String& id )
            : FactoryObject( id )
            , Value( GetNumber( id ) )
        {}
    };

    TAU_SMART_PTR( Chip )

    // ---------------- //

    class ChipReceiver: public FactoryObject
    {
    public:
        virtual void Receive( Chip* chip )
        {
            AOC_MSG( "Receive (" << this->Name << "): " << chip->Name )
            m_chips.Add( chip );
        }

        const Tau::Set< Chip* >& GetChips() const { return m_chips; }

    protected:
        ChipReceiver( const Tau::String& id )
            : FactoryObject( id )
        {}

        Tau::Set< Chip* > m_chips;
    };

    TAU_SMART_PTR( ChipReceiver )

    // ---------------- //

    class Bot: public ChipReceiver
    {
    public:
        FACTORY_METHOD( Bot )

        virtual void Receive( Chip* chip ) _override
        {
            ChipReceiver::Receive( chip );
            TAU_ASSERT( m_chips.Size() <= 2 )

            if( m_chips.Size() == 2 )
            {
                Dispatch();
            }
        }

        void SetReceivers( ChipReceiver* lowReceiver, ChipReceiver* highReceiver )
        {
            TAU_ASSERT( lowReceiver != highReceiver )
            LowReceiver = lowReceiver;
            HighReceiver = highReceiver;
        }

        Property::Pointer< ChipReceiver, Bot > LowReceiver;
        Property::Pointer< ChipReceiver, Bot > HighReceiver;

        static Tau::Event< const DispatchInfo& > DispatchEvent;

    private:
        Bot( const Tau::String& id )
            : ChipReceiver( id )
        {}

        void Dispatch()
        {
            TAU_ASSERT( m_chips.Size() == 2 )

            Chip* low = m_chips.Pop();
            Chip* high = m_chips.Pop();
            if( high->Value < low->Value ) { std::swap( low, high ); }

            AOC_MSG( "Dispatch (" << this->Name << "): " << low->Name << " -> " << LowReceiver->Name << ", " << high->Name << " -> " << HighReceiver->Name )
            DispatchEvent( DispatchInfo( this, low, high ) );
            LowReceiver->Receive( low );
            HighReceiver->Receive( high );
        }
    };

    Tau::Event< const DispatchInfo& > Bot::DispatchEvent;

    TAU_SMART_PTR( Bot )

    // ---------------- //

    class Bin: public ChipReceiver
    {
    public:
        FACTORY_METHOD( Bin )

    private:
        Bin( const Tau::String& id )
            : ChipReceiver( id )
        {}
    };

    TAU_SMART_PTR( Bin )

    // ---------------- //

    AOC10::AOC10()
    {
        m_factory.Register( "value", Chip::Create );
        m_factory.Register( "bot", Bot::Create );
        m_factory.Register( "output", Bin::Create );

        Bot::DispatchEvent += EventHandler( AOC10, OnDispatch, const DispatchInfo& );
    }

    void AOC10::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC10, ProcessLine, const Tau::String& ) );

        for( Tau::Vector< Tau::String >::const_iterator it = m_gives.begin(); it != m_gives.end(); it++ )
        {
            Bot* bot = GetObject< Bot >( Tau::StringUtils::GetBefore( *it, " gives low to ", false ) );
            ChipReceiver* lowReceiver = GetObject< ChipReceiver >( Tau::StringUtils::GetBetween( *it, " gives low to ", " and high to ", false ) );
            ChipReceiver* highReceiver = GetObject< ChipReceiver >( Tau::StringUtils::GetAfter( *it, " and high to ", false ) );
            bot->SetReceivers( lowReceiver, highReceiver );
        }

        for( Tau::Vector< Tau::String >::const_iterator it = m_goesTo.begin(); it != m_goesTo.end(); it++ )
        {
            Chip* chip = GetObject< Chip >( Tau::StringUtils::GetBefore( *it, " goes to ", false ) );
            Bot* bot = GetObject< Bot >( Tau::StringUtils::GetAfter( *it, " goes to ", false ) );
            bot->Receive( chip );
        }

        for( Tau::Map< FactoryObjectPtr >::const_iterator it = m_objects.begin(); it != m_objects.end(); it++ )
        {
            if( Tau::StringUtils::StartsWith( it->second->Name, "bot" ) )
            {
                TAU_ASSERT( it->second.Cast< Bot >()->GetChips().IsEmpty() )
            }
        }

        ResultB = GetChipValue( "0" ) * GetChipValue( "1" ) * GetChipValue( "2" );
    }

    bool AOC10::ProcessLine( const Tau::String& line )
    {
        if( Tau::StringUtils::Has( line, "gives" ) )
        {
            m_gives.Add( line );
        }
        else if( Tau::StringUtils::Has( line, "goes to" ) )
        {
            m_goesTo.Add( line );
        }
        else
        {
            TAU_MSG_ASSERT( false, "Unexpected instruction: " << line )
        }

        return true;
    }

    void AOC10::OnDispatch( const DispatchInfo& info )
    {
        TAU_ASSERT( info.low->Value < info.high->Value )

        if( info.low->Value == 17 && info.high->Value == 61 )
        {
            TAU_ASSERT( !FinishedA )
            ResultA = Tau::StringUtils::To< int >( Tau::StringUtils::GetAfter( info.bot->Name, "bot ", false ) );
            FinishedA = true;
        }
    }

    template< class T >
    T* AOC10::GetObject( const Tau::String& id )
    {
        return Tau::DownCast< T >( ( m_objects.Has( id ) ? m_objects.Get( id ) : m_objects.Add( id, m_factory.Create( FactoryObject::GetType( id ), id ) ) ).Raw() );
    }

    int AOC10::GetChipValue( const Tau::String& binNumber )
    {
        const Tau::Set< Chip* > chips = GetObject< Bin >( "output " + binNumber )->GetChips();
        TAU_ASSERT( chips.Size() == 1 )
        return chips.First()->Value;
    }

}
