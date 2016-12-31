// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC11.hpp"

#include "core/lang/Singleton.hpp"
#include "core/file/File.hpp"
#include "core/serialization/EnumString.hpp"
#include "core/serialization/StringUtils.hpp"
#include "core/math/Combination.hpp"
#include "core/algorithm/Dijkstra.hpp"

namespace AOC
{

    class Generator;

    // ---------------- //

    class LabItem: public Tau::Object
    {
    public:
        LabItem( const Tau::String& name )
            : Object( name )
        {}
    };

    TAU_SMART_PTR( LabItem )

    // ---------------- //

    class Microchip: public LabItem
    {
    public:
        Microchip( const Tau::String& name )
            : LabItem( name )
        {}

        bool IsValid() const;

        void SetGenerator( Generator* generator ) { Generator_ = generator; }

        Property::Pointer< Generator, Microchip > Generator_;
    };

    TAU_SMART_PTR( Microchip )

    // ---------------- //

    class Generator: public LabItem
    {
    public:
        Generator( const Tau::String& name )
            : LabItem( name )
        {}

        bool IsValid() const
        {
            return Microchip_ && Microchip_->Name == Name;
        }

        void SetMicrochip( Microchip* microchip ) { Microchip_ = microchip; }

        Property::Pointer< Microchip, Generator > Microchip_;
    };

    TAU_SMART_PTR( Generator )

    // ---------------- //

    bool Microchip::IsValid() const
    {
        return Generator_ && Generator_->Name == Name;
    }

    // ---------------- //

    class Lab: public Tau::Object, public Tau::Singleton< Lab >
    {
    public:
        static Tau::Map< Tau::SmartPtr< Generator > >& Generators() { return Get()->Generators_; }
        static Tau::Map< Tau::SmartPtr< Microchip > >& Microchips() { return Get()->Microchips_; }

        static const int Floors;

    private:
        Tau::Map< Tau::SmartPtr< Generator > > Generators_;
        Tau::Map< Tau::SmartPtr< Microchip > > Microchips_;
    };

    const int Lab::Floors = 4;

    // ---------------- //

    class ItemsState
    {
    public:
        ItemsState( int elevatorFloor = 1 )
            : ElevatorFloor( elevatorFloor )
        {}

        typedef Tau::Map_< const LabItem*, int > FloorMap;

        bool operator<( const ItemsState& state ) const
        {
            return ElevatorFloor == state.ElevatorFloor ? m_itemFloors < state.m_itemFloors : ElevatorFloor < state.ElevatorFloor;
        }

        void SetItemFloor( const LabItem* item, int floor )
        {
            ValidateFloor( floor );
            m_itemFloors.Overwrite( item, floor );
        }

        void GetItemsOnElevatorFloor( Tau::Set< const LabItem* >& items ) const
        {
            TAU_PREDICATE_1( IsOnFloor, int, const FloorMap::value_type& floors, return floors.second == p )
            m_itemFloors.SelectKeys( items, IsOnFloor( ElevatorFloor ) );
        }

        Tau::Nullable< ItemsState > TryMoveElevatorUp( const LabItem* i1, const LabItem* i2 ) const
        {
            return TryMoveElevator( i1, i2, 1 );
        }

        Tau::Nullable< ItemsState > TryMoveElevatorDown( const LabItem* i1, const LabItem* i2 ) const
        {
            return TryMoveElevator( i1, i2, -1 );
        }

        bool IsSafe() const
        {
            for( Tau::Map< MicrochipPtr >::const_iterator cit = Lab::Microchips().begin(); cit != Lab::Microchips().end(); cit++ )
            {
                const LabItem* microchip = cit->second;
                const LabItem* generator = cit->second->Generator_;
                int microchipFloor = m_itemFloors.Get( microchip );

                if( m_itemFloors.Get( generator ) == microchipFloor ) { continue; }

                for( Tau::Map< GeneratorPtr >::const_iterator git = Lab::Generators().begin(); git != Lab::Generators().end(); git++ )
                {
                    const LabItem* otherGenerator = git->second;

                    if( otherGenerator != generator && m_itemFloors.Get( otherGenerator ) == microchipFloor )
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        bool IsFinal() const
        {
            for( FloorMap::const_iterator it = m_itemFloors.begin(); it != m_itemFloors.end(); it++ )
            {
                if( it->second != Lab::Floors ) { return false; }
            }

            return true;
        }

        Property::Int< ItemsState > ElevatorFloor;

    private:
        FloorMap m_itemFloors;

        Tau::Nullable< ItemsState > TryMoveElevator( const LabItem* i1, const LabItem* i2, int floorIncrement ) const
        {
            ValidateFloorIncrement( floorIncrement );
            if( !IsValidFloor( ElevatorFloor + floorIncrement ) ) { return Tau::Nullable< ItemsState >::EMPTY; }

            ItemsState newState( ElevatorFloor + floorIncrement );
            newState.m_itemFloors = m_itemFloors;
            newState.MoveItem( i1, floorIncrement );
            if( i2 ) { newState.MoveItem( i2, floorIncrement ); }

            return newState.IsSafe() ? newState : Tau::Nullable< ItemsState >::EMPTY;
        }

        void MoveItem( const LabItem* item, int floorIncrement )
        {
            ValidateFloorIncrement( floorIncrement );
            SetItemFloor( item, m_itemFloors.Get( item ) + floorIncrement );
        }

        static void ValidateFloor( int floor )
        {
            TAU_ASSERT( IsValidFloor( floor ) )
        }

        static bool IsValidFloor( int floor )
        {
            return floor >= 1 && floor <= Lab::Floors;
        }

        static void ValidateFloorIncrement( int floorIncrement )
        {
            TAU_ASSERT( floorIncrement == 1 || floorIncrement == -1 )
        }
    };

    // ---------------- //

    static void GetItems( const Tau::String& string, Tau::Vector< Tau::String >& items )
    {
        TAU_ASSERT( items.IsEmpty() )

        if( string == "nothing relevant" ) { return; }

        Tau::String leading, last;

        if( !Tau::StringUtils::Split( string, leading, ", and ", last, true ) )
        {
            Tau::StringUtils::Split( string, leading, " and ", last, true );
        }

        Tau::StringUtils::Split( leading, items, ", ", true );
        if( last.HasLength() ) { items.Add( last ); }
    }

    static void GetItemTraits( const Tau::String& item, Tau::String& name, Tau::String& type )
    {
        Tau::String leading;
        Tau::StringUtils::ReverseSplit( item, leading, ' ', type, false );
        TAU_ASSERT( type == "generator" || type == "microchip" )
        name = type == "generator" ? Tau::StringUtils::GetBetween( item, "a ", " generator", false )
                                   : Tau::StringUtils::GetBetween( item, "a ", "-compatible microchip", false );
    }

    // ---------------- //

    class Solver: public Tau::Object
    {
    public:
        Solver()
        {
            Lab::Set( new Lab(), true );

            m_numbers.Add( "first", 1 );
            m_numbers.Add( "second", 2 );
            m_numbers.Add( "third", 3 );
            m_numbers.Add( "fourth", 4 );
        }

        int Solve( const Tau::String& fileName, const Tau::String& additionalData )
        {
            Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, Solver, ProcessLine, const Tau::String& ) );
            if( additionalData.HasLength() ) { ProcessLine( additionalData ); }

            LinkItems();

            Graph graph;
            Graph::NodePtr initialNode( new Graph::Node( m_initialState ) );
            graph.AddNode( initialNode );
            m_nodes.Add( m_initialState, initialNode );

            Tau::Vector< Graph::Node* > newNodes;
            newNodes.Add( initialNode );

            for( int i = 0; i < newNodes.Size(); i++ )
            {
                ExtendGraph( graph, newNodes[i], newNodes );
            }

            TAU_PREDICATE_0( IsEndNode, const Graph::NodePtr& node, return node->GetData().IsFinal() )
            return Algorithm::Dijkstra< ItemsState, int >::FindPath( graph, initialNode, graph.GetNode( IsEndNode() ) )->GetDistance();
        }

    private:
        typedef Tau::Graph< ItemsState, int > Graph;

        bool ProcessLine( const Tau::String& line )
        {
            int floor = m_numbers.Get( Tau::StringUtils::GetBetween( line, "The ", " floor contains ", false ) );

            Tau::Vector< Tau::String > items;
            GetItems( Tau::StringUtils::GetBetween( line, " floor contains ", ".", false ), items );

            for( Tau::Vector< Tau::String >::const_iterator it = items.begin(); it != items.end(); it++ )
            {
                Tau::String name, type;
                GetItemTraits( *it, name, type );

                if( type == "generator" )
                {
                    GeneratorPtr generator( new Generator( name ) );
                    Lab::Generators().Add( name, generator );
                    m_initialState.SetItemFloor( generator, floor );
                }
                else if( type == "microchip" )
                {
                    MicrochipPtr microchip( new Microchip( name ) );
                    Lab::Microchips().Add( name, microchip );
                    m_initialState.SetItemFloor( microchip, floor );
                }
                else
                {
                    TAU_ASSERT( false )
                }
            }

            return true;
        }

        void LinkItems()
        {
            for( Tau::Map< GeneratorPtr >::iterator it = Lab::Generators().begin(); it != Lab::Generators().end(); it++ )
            {
                Generator* generator = it->second;
                Microchip* microchip = Lab::Microchips().Get( generator->Name );
                generator->SetMicrochip( microchip );
                microchip->SetGenerator( generator );

                TAU_ASSERT( generator->IsValid() )
            }

            for( Tau::Map< MicrochipPtr >::const_iterator it = Lab::Microchips().begin(); it != Lab::Microchips().end(); it++ )
            {
                TAU_ASSERT( it->second->IsValid() )
            }
        }

        void ExtendGraph( Graph& graph, Graph::Node* node, Tau::Vector< Graph::Node* >& newNodes )
        {
            const ItemsState& state = node->GetData();

            Tau::Set< const LabItem* > itemsOnElevatorFloor;
            state.GetItemsOnElevatorFloor( itemsOnElevatorFloor );

            for( Tau::Set< const LabItem* >::const_iterator it1 = itemsOnElevatorFloor.begin(); it1 != itemsOnElevatorFloor.end(); it1++ )
            {
                HandleState( graph, node, state.TryMoveElevatorUp( *it1, NULL ), newNodes );
                HandleState( graph, node, state.TryMoveElevatorDown( *it1, NULL ), newNodes );
            }

            if( itemsOnElevatorFloor.Size() > 1 )
            {
                Tau::Combination< const LabItem* > combination( 2, itemsOnElevatorFloor );
                Tau::Combination< const LabItem* >::Generator generator( combination );
                Tau::Set< const LabItem* > pair;

                while( generator.Next( pair ) )
                {
                    const Tau::Set< const LabItem* >& p = const_cast< const Tau::Set< const LabItem* >& >( pair );
                    HandleState( graph, node, state.TryMoveElevatorUp( p.First(), p.Last() ), newNodes );
                    HandleState( graph, node, state.TryMoveElevatorDown( p.First(), p.Last() ), newNodes );
                }
            }
        }

        void HandleState( Graph& graph, Graph::Node* node, const Tau::Nullable< ItemsState >& state, Tau::Vector< Graph::Node* >& newNodes )
        {
            if( state.IsSet() )
            {
                node->Link( GetNodeToLink( graph, state.Get(), newNodes ), 1, true, true );
            }
        }

        Solver::Graph::Node* GetNodeToLink( Graph& graph, const ItemsState& state, Tau::Vector< Graph::Node* >& newNodes )
        {
            Graph::Node* node = m_nodes.TryGet( state, NULL );

            if( !node )
            {
                node = graph.AddNode( Graph::NodePtr( new Graph::Node( state ) ) );
                newNodes.Add( node );
                m_nodes.Add( state, node );
            }

            return node;
        }

        Tau::Map< int > m_numbers;
        Tau::Map_< ItemsState, Graph::Node* > m_nodes;

        ItemsState m_initialState;
    };

    // ---------------- //

    void AOC11::Solve( const Tau::String& fileName )
    {
        // A
        {
            ResultA = Solver().Solve( fileName, Tau::String::EMPTY );
        }

        // B
        {
            ResultB = Solver().Solve( fileName, "The first floor contains a elerium generator, a elerium-compatible microchip, a dilithium generator and a dilithium-compatible microchip." );
        }
    }

}
