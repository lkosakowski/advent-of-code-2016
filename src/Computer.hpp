// rev: d5a54d1, exported: 2016-12-31 13:39:29

#ifndef __INCLUDED_COMPUTER_HPP__
#define __INCLUDED_COMPUTER_HPP__

#include "core/lang/Object.hpp"
#include "core/lang/Override.hpp"
#include "core/lang_container/Event.hpp"
#include "core/lang_container/Factory.hpp"

namespace AOC
{

    class Program;

    // ---------------- //

    class Processor: public Tau::Object
    {
    public:
        void Execute( const Program& program );
        void Halt();

        struct State
        {
            State();

            bool operator<( const State& state ) const;

            int programCounter;
            Tau::Vector< long long > registers;
        };

        const State& GetState() const { return m_state; }
        int& GetProgramCounter() { return m_state.programCounter; }
        long long& GetRegister( int i ) { return m_state.registers[i]; }

        struct OutInfo
        {
            OutInfo( long long a_value, Processor* a_processor )
                : value( a_value )
                , processor( a_processor )
            {}

            long long value;
            Processor* processor;
        };

        Tau::Event< const OutInfo& > OutEvent;

        static const int RegisterCount = 4;

    private:
        Property::WeakPtr< Program, Processor > Program_;
        State m_state;
    };

    // ---------------- //

    class Instruction: public Tau::Object
    {
    public:
        Instruction( const Tau::String& name );

        virtual void Execute( Processor* processor, Program* program ) const = 0;
        virtual Tau::SmartPtr< Instruction > Toggle() const;

    protected:
        static void Jump( Processor* processor, int steps );
    };

    TAU_SMART_PTR( Instruction )

    // ---------------- //

    class CpyVR: public Instruction
    {
    public:
        CpyVR( long long v, int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Long< CpyVR > Value;
        Property::Int< CpyVR > Register;
    };

    // ---------------- //

    class CpyRR: public Instruction
    {
    public:
        CpyRR( int r1, int r2 );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Int< CpyRR > R1;
        Property::Int< CpyRR > R2;
    };

    // ---------------- //

    class Inc: public Instruction
    {
    public:
        explicit Inc( int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Int< Inc > Register;
    };

    // ---------------- //

    class Dec: public Instruction
    {
    public:
        explicit Dec( int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Int< Dec > Register;
    };

    // ---------------- //

    class Jnz: public Instruction
    {
    protected:
        explicit Jnz( const Tau::String& name );

        static void Jump( Processor* processor, long long registerValue, int steps );
    };

    // ---------------- //

    class JnzVV: public Jnz
    {
    public:
        JnzVV( long long v, int s );

        virtual void Execute( Processor* processor, Program* program ) const _override;

        Property::Long< JnzVV > Value;
        Property::Int< JnzVV > Steps;
    };

    // ---------------- //

    class JnzVR: public Jnz
    {
    public:
        JnzVR( long long v, int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Long< JnzVR > Value;
        Property::Int< JnzVR > Register;
    };

    // ---------------- //

    class JnzRV: public Jnz
    {
    public:
        JnzRV( int r, int s );

        virtual void Execute( Processor* processor, Program* program ) const _override;

        Property::Int< JnzRV > Register;
        Property::Int< JnzRV > Steps;
    };

    // ---------------- //

    class JnzRR: public Jnz
    {
    public:
        JnzRR( int r1, int r2 );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Int< JnzRV > R1;
        Property::Int< JnzRV > R2;
    };

    // ---------------- //

    class Tgl: public Instruction
    {
    public:
        explicit Tgl( int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;
        virtual InstructionPtr Toggle() const _override;

        Property::Int< Tgl > Register;
    };

    // ---------------- //

    class Mul: public Instruction
    {
    public:
        Mul( int r1, int r2, int r3 );

        virtual void Execute( Processor* processor, Program* program ) const _override;

        Property::Int< Mul > R1;
        Property::Int< Mul > R2;
        Property::Int< Mul > R3;
    };

    // ---------------- //

    class Out: public Instruction
    {
    public:
        Out( int r );

        virtual void Execute( Processor* processor, Program* program ) const _override;

        Property::Int< Out > Register;
    };

    // ---------------- //

    class Null: public Instruction
    {
    public:
        Null();

        virtual void Execute( Processor* processor, Program* program ) const _override;
    };

    // ---------------- //

    class Opt: public Instruction
    {
    public:
        Opt( const Tau::Vector< InstructionPtr >& instructions, int jump );

        virtual void Execute( Processor* processor, Program* program ) const _override;

        Property::Int< Opt > Jump_;

    private:
        Tau::Vector< InstructionPtr > m_instructions;
    };

    // ---------------- //

    class Program: public Tau::Object
    {
    public:
        Program( const Tau::Vector< InstructionPtr >& instructions, bool optimize = false );

        const Instruction* GetInstruction( int i ) const;
        void SwapInstruction( int i, const InstructionPtr& instruction );
        int GetInstructionCount() const;

        Property::Bool< Program > Optimize_;

    private:
        void Optimize();
        void OptimizeMul( int& i );

        template< class T >
        bool Get( int i, const Tau::String& name, Tau::Pointer< T >& instruction ) const;

        Tau::Vector< InstructionPtr > m_instructions;
        Tau::Vector< InstructionPtr > m_preOptimizeInstructions;
    };

    TAU_SMART_PTR( Program )

    // ---------------- //

    class InstructionParser
    {
    public:
        InstructionParser();

        void Process( const Tau::String& line );

        const Tau::Vector< Tau::SmartPtr< Instruction > >& GetInstructions() const { return m_instructions; }

        static InstructionPtr Cpy( const Tau::String& arguments );
        static InstructionPtr Inc( const Tau::String& arguments );
        static InstructionPtr Dec( const Tau::String& arguments );
        static InstructionPtr Jnz( const Tau::String& arguments );
        static InstructionPtr Tgl( const Tau::String& arguments );
        static InstructionPtr Mul( const Tau::String& arguments );
        static InstructionPtr Out( const Tau::String& arguments );
        static InstructionPtr Null( const Tau::String& arguments );

        static int GetRegister( const Tau::String& string );
        static char GetString( int r );

    private:
        Tau::Vector< Tau::SmartPtr< Instruction > > m_instructions;
        Tau::Factory1< Instruction, const Tau::String& > m_factory;
    };

    // ---------------- //

    class Computer
    {
    public:
        static long long Calculate( Processor& processor, long long initialValue, const Tau::Vector< Tau::SmartPtr< Instruction > >& instructions, bool optimize );
    };

}

#endif
