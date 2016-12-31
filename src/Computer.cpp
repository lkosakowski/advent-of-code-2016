// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "Computer.hpp"

#include "core/serialization/StringUtils.hpp"

#define AOC_MSG( msg ) // TAU_MSG( msg )

namespace AOC
{

    Processor::State::State()
        : programCounter( 0 )
    {
        registers.EnsureSize( RegisterCount );
    }

    bool Processor::State::operator<( const State& state ) const
    {
        return programCounter == state.programCounter ? registers < state.registers : programCounter < state.programCounter;
    }

    // ---------------- //

    void Processor::Execute( const Program& program_ )
    {
        ProgramPtr program( new Program( program_ ) );

        TAU_ASSERT( !Program_ )
        Program_ = Tau::WeakPtr< Program >( program );
        m_state = State();

        while( const Instruction* instruction = program->GetInstruction( m_state.programCounter ) )
        {
            instruction->Execute( this, program );
            m_state.programCounter++;
        }
    }

    void Processor::Halt()
    {
        m_state.programCounter = Program_->GetInstructionCount() - 1;
    }

    // ---------------- //

    Instruction::Instruction( const Tau::String& name )
        : Object( name )
    {}

    void Instruction::Jump( Processor* processor, int steps )
    {
        if( steps != 0 )
        {
            processor->GetProgramCounter() += steps - 1;
        }
    }

    InstructionPtr Instruction::Toggle() const
    {
        TAU_ASSERT( false )
        return InstructionPtr();
    }

    // ---------------- //

    CpyVR::CpyVR( long long v, int r )
        : Instruction( "cpy_vr" )
        , Value( v )
        , Register( r )
    {}

    void CpyVR::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Cpy: " << Value << " " << InstructionParser::GetString( Register ) )
        processor->GetRegister( Register ) = Value;
    }

    InstructionPtr CpyVR::Toggle() const
    {
        return InstructionPtr( new JnzVR( Value, Register ) );
    }

    // ---------------- //

    CpyRR::CpyRR( int r1, int r2 )
        : Instruction( "cpy_rr" )
        , R1( r1 )
        , R2( r2 )
    {}

    void CpyRR::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Cpy: " << InstructionParser::GetString( R1 ) << " " << InstructionParser::GetString( R2 ) )
        processor->GetRegister( R2 ) = processor->GetRegister( R1 );
    }

    InstructionPtr CpyRR::Toggle() const
    {
        return InstructionPtr( new JnzRR( R1, R2 ) );
    }

    // ---------------- //

    Inc::Inc( int r )
        : Instruction( "inc" )
        , Register( r )
    {}

    void Inc::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Inc: " << InstructionParser::GetString( Register ) )
        processor->GetRegister( Register )++;
    }

    InstructionPtr Inc::Toggle() const
    {
        return InstructionPtr( new Dec( Register ) );
    }

    // ---------------- //

    Dec::Dec( int r )
        : Instruction( "dec" )
        , Register( r )
    {}

    void Dec::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Dec: " << InstructionParser::GetString( Register ) )
        processor->GetRegister( Register )--;
    }

    InstructionPtr Dec::Toggle() const
    {
        return InstructionPtr( new Inc( Register ) );
    }

    // ---------------- //

    Jnz::Jnz( const Tau::String& name )
        : Instruction( name )
    {}

    void Jnz::Jump( Processor* processor, long long registerValue, int steps )
    {
        if( registerValue )
        {
            Instruction::Jump( processor, steps );
        }
    }

    // ---------------- //

    JnzVV::JnzVV( long long v, int s )
        : Jnz( "jnz_vv" )
        , Value( v )
        , Steps( s )
    {}

    void JnzVV::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Jnz: " << Value << " " << Steps )
        Jump( processor, Value, Steps );
    }

    // ---------------- //

    JnzVR::JnzVR( long long v, int r )
        : Jnz( "jnz_vr" )
        , Value( v )
        , Register( r )
    {}

    void JnzVR::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Jnz: " << Value << " " << InstructionParser::GetString( Register ) )
        Jump( processor, Value, Tau::StaticCast< int >( processor->GetRegister( Register ) ) );
    }

    InstructionPtr JnzVR::Toggle() const
    {
        return InstructionPtr( new CpyVR( Value, Register ) );
    }

    // ---------------- //

    JnzRV::JnzRV( int r, int s )
        : Jnz( "jnz_rv" )
        , Register( r )
        , Steps( s )
    {}

    void JnzRV::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Jnz: " << InstructionParser::GetString( Register ) << " " << Steps )
        Jump( processor, processor->GetRegister( Register ), Steps );
    }

    // ---------------- //

    JnzRR::JnzRR( int r1, int r2 )
        : Jnz( "jnz_rr" )
        , R1( r1 )
        , R2( r2 )
    {}

    void JnzRR::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Jnz: " << InstructionParser::GetString( R1 ) << " " << InstructionParser::GetString( R2 ) )
        Jump( processor, Tau::StaticCast< int >( processor->GetRegister( R1 ) ), Tau::StaticCast< int >( processor->GetRegister( R2 ) ) );
    }

    InstructionPtr JnzRR::Toggle() const
    {
        return InstructionPtr( new CpyRR( R1, R2 ) );
    }

    // ---------------- //

    Tgl::Tgl( int r )
        : Instruction( "tgl" )
        , Register( r )
    {}

    void Tgl::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Tgl: " << InstructionParser::GetString( Register ) )

        int i = processor->GetProgramCounter() + Tau::StaticCast< int >( processor->GetRegister( Register ) );

        if( i >= 0 && i < program->GetInstructionCount() )
        {
            program->SwapInstruction( i, program->GetInstruction( i )->Toggle() );
        }
    }

    InstructionPtr Tgl::Toggle() const
    {
        return InstructionPtr( new Inc( Register ) );
    }

    // ---------------- //

    Mul::Mul( int r1, int r2, int r3 )
        : Instruction( "mul" )
        , R1( r1 )
        , R2( r2 )
        , R3( r3 )
    {}

    void Mul::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Mul: " << InstructionParser::GetString( R1 ) << " " << InstructionParser::GetString( R2 ) << " " << InstructionParser::GetString( R3 ) )
        processor->GetRegister( R3 ) = processor->GetRegister( R1 ) * processor->GetRegister( R2 );
    }

    // ---------------- //

    Out::Out( int r )
        : Instruction( "out" )
        , Register( r )
    {}

    void Out::Execute( Processor* processor, Program* program ) const
    {
        processor->OutEvent( Processor::OutInfo( processor->GetRegister( Register ), processor ) );
    }

    // ---------------- //

    Null::Null()
        : Instruction( "null" )
    {}

    void Null::Execute( Processor* processor, Program* program ) const
    {
        AOC_MSG( "Null" )
    }

    // ---------------- //

    Opt::Opt( const Tau::Vector< InstructionPtr >& instructions, int jump )
        : Instruction( "opt" )
        , Jump_( jump )
        , m_instructions( instructions )
    {}

    void Opt::Execute( Processor* processor, Program* program ) const
    {
        int pc = processor->GetProgramCounter();

        for( Tau::Vector< InstructionPtr >::const_iterator it = m_instructions.begin(); it != m_instructions.end(); it++ )
        {
            (*it)->Execute( processor, program );
        }

        TAU_ASSERT( pc == processor->GetProgramCounter() )
        Jump( processor, Jump_ );
    }

    // ---------------- //

    Program::Program( const Tau::Vector< InstructionPtr >& instructions, bool optimize )
        : Optimize_( optimize )
        , m_instructions( instructions )
    {
        if( Optimize_ )
        {
            m_preOptimizeInstructions = instructions;
            Optimize();
        }
    }

    const Instruction* Program::GetInstruction( int i ) const
    {
        return i == m_instructions.Size() ? NULL : m_instructions[i].Raw();
    }

    void Program::SwapInstruction( int i, const InstructionPtr& instruction )
    {
        if( Optimize_ )
        {
            m_preOptimizeInstructions[i] = instruction;
            m_instructions = m_preOptimizeInstructions;
            Optimize();
        }
        else
        {
            m_instructions[i] = instruction;
        }
    }

    int Program::GetInstructionCount() const
    {
        return m_instructions.Size();
    }

    void Program::Optimize()
    {
        TAU_ASSERT( Optimize_ && !m_preOptimizeInstructions.IsEmpty() )

        int i = 0;

        while( i < m_instructions.Size() )
        {
            OptimizeMul( i );
            i++;
        }
    }

    void Program::OptimizeMul( int& i )
    {
        static const int BlockSize = 7;

        if( i + BlockSize > m_instructions.Size() ) { return; }

        Tau::Pointer< CpyVR > a;
        Tau::Pointer< CpyRR > b;
        Tau::Pointer< Inc > c;
        Tau::Pointer< Dec > d;
        Tau::Pointer< JnzRV > e;
        Tau::Pointer< Dec > f;
        Tau::Pointer< JnzRV > g;

        if( !Get( i + 0, "cpy_vr", a ) ) { return; }
        if( !Get( i + 1, "cpy_rr", b ) ) { return; }
        if( !Get( i + 2, "inc", c ) )    { return; }
        if( !Get( i + 3, "dec", d ) )    { return; }
        if( !Get( i + 4, "jnz_rv", e ) ) { return; }
        if( !Get( i + 5, "dec", f ) )    { return; }
        if( !Get( i + 6, "jnz_rv", g ) ) { return; }

        int result = a->Register;
        int multiplicand = b->R1;
        int auxiliary = b->R2;
        int multiplier = f->Register;

        if( a->Value == 0 &&
            c->Register == result &&
            d->Register == auxiliary &&
            e->Register == auxiliary && e->Steps == -2 &&
            g->Register == multiplier && g->Steps == -5 )
        {
            Tau::Vector< InstructionPtr > instructions;
            instructions.Add( InstructionPtr( new Mul( multiplicand, multiplier, result ) ) );
            instructions.Add( InstructionPtr( new CpyVR( 0, multiplier ) ) );
            m_instructions[i] = InstructionPtr( new Opt( instructions, BlockSize ) );
            i += BlockSize - 1;
        }
    }

    template< class T >
    bool Program::Get( int i, const Tau::String& name, Tau::Pointer< T >& instruction ) const
    {
        TAU_ASSERT( !instruction )

        if( m_instructions[i]->Name == name )
        {
            instruction = m_instructions[i].Cast< T >();
            return true;
        }

        return false;
    }

    // ---------------- //

    InstructionParser::InstructionParser()
    {
        m_factory.Register( "cpy", Cpy );
        m_factory.Register( "inc", Inc );
        m_factory.Register( "dec", Dec );
        m_factory.Register( "jnz", Jnz );
        m_factory.Register( "tgl", Tgl );
        m_factory.Register( "mul", Mul );
        m_factory.Register( "out", Out );
        m_factory.Register( "null", Null );
    }

    void InstructionParser::Process( const Tau::String& line )
    {
        Tau::String instruction, arguments;
        Tau::StringUtils::Split( line, instruction, ' ', arguments, true );
        m_instructions.Add( m_factory.Create( instruction, arguments ) );
    }

    InstructionPtr InstructionParser::Cpy( const Tau::String& arguments )
    {
        Tau::String x, y;
        Tau::StringUtils::Split( arguments, x, ' ', y, false );
        return Tau::StringUtils::IsNumber( x ) ? InstructionPtr( new CpyVR( Tau::StringUtils::To< int >( x ), GetRegister( y ) ) )
                                               : InstructionPtr( new CpyRR( GetRegister( x ), GetRegister( y ) ) );
    }

    InstructionPtr InstructionParser::Inc( const Tau::String& arguments )
    {
        return InstructionPtr( new AOC::Inc( GetRegister( arguments ) ) );
    }

    InstructionPtr InstructionParser::Dec( const Tau::String& arguments )
    {
        return InstructionPtr( new AOC::Dec( GetRegister( arguments ) ) );
    }

    InstructionPtr InstructionParser::Jnz( const Tau::String& arguments )
    {
        Tau::String x, y;
        Tau::StringUtils::Split( arguments, x, ' ', y, false );
        return Tau::StringUtils::IsNumber( x ) ? Tau::StringUtils::IsNumber( y ) ? InstructionPtr( new JnzVV( Tau::StringUtils::To< int >( x ), Tau::StringUtils::To< int >( y ) ) )
                                                                                 : InstructionPtr( new JnzVR( Tau::StringUtils::To< int >( x ), GetRegister( y ) ) )
                                               : Tau::StringUtils::IsNumber( y ) ? InstructionPtr( new JnzRV( GetRegister( x ), Tau::StringUtils::To< int >( y ) ) )
                                                                                 : InstructionPtr( new JnzRV( GetRegister( x ), GetRegister( y ) ) );
    }

    InstructionPtr InstructionParser::Tgl( const Tau::String& arguments )
    {
        return InstructionPtr( new AOC::Tgl( GetRegister( arguments ) ) );
    }

    InstructionPtr InstructionParser::Mul( const Tau::String& arguments )
    {
        Tau::Vector< Tau::String > parts;
        Tau::StringUtils::Split( arguments, parts, ' ', false );
        TAU_ASSERT( parts.Size() == 3 )
        return InstructionPtr( new AOC::Mul( GetRegister( parts[0] ), GetRegister( parts[1] ), GetRegister( parts[2] ) ) );
    }

    InstructionPtr InstructionParser::Out( const Tau::String& arguments )
    {
        return InstructionPtr( new AOC::Out( GetRegister( arguments ) ) );
    }

    InstructionPtr InstructionParser::Null( const Tau::String& arguments )
    {
        return InstructionPtr( new AOC::Null() );
    }

    int InstructionParser::GetRegister( const Tau::String& string )
    {
        TAU_ASSERT( string.GetLength() == 1 )
        char r = string[0];
        TAU_ASSERT( r >= 'a' && r < 'a' + Processor::RegisterCount )
        return r - 'a';
    }

    char InstructionParser::GetString( int r )
    {
        TAU_ASSERT( r >= 0 && r < Processor::RegisterCount )
        return 'a' + r;
    }

    // ---------------- //

    long long Computer::Calculate( Processor& processor, long long initialValue, const Tau::Vector< Tau::SmartPtr< Instruction > >& instructions, bool optimize )
    {
        Tau::Vector< InstructionPtr > init;
        init.Add( InstructionParser::Cpy( Tau::StringUtils::String( initialValue ) + " a" ) );
        processor.Execute( Program( init + instructions, optimize ) );
        return processor.GetRegister( 0 );
    }

}
