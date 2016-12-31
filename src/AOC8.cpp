// rev: d5a54d1, exported: 2016-12-31 13:39:29

#include "AOC8.hpp"

#include "core/file/File.hpp"
#include "core/serialization/StringUtils.hpp"

namespace AOC
{

    class Operation: public Tau::Object
    {
    public:
        virtual void Apply( Tau::Image* image ) const = 0;
    };

    TAU_SMART_PTR( Operation )

    // ---------------- //

    class Rect: public Operation
    {
    public:
        static OperationPtr Create( const Tau::String& string )
        {
            return OperationPtr( new Rect( string ) );
        }

        virtual void Apply( Tau::Image* image ) const _override
        {
            TAU_ASSERT( image->Format_ == Tau::Image::FORMAT_GRAY )

            char* p = *image;
            int h = Height;

            while( h-- )
            {
                memset( p, 255, Width );
                p += image->Pitch;
            }
        }

    private:
        Rect( const Tau::String& string )
        {
            Tau::String rows, columns;
            Tau::StringUtils::Split( string, rows, 'x', columns, false );
            Width = Tau::StringUtils::To< int >( rows );
            Height = Tau::StringUtils::To< int >( columns );
        }

        Property::Int< Rect > Width;
        Property::Int< Rect > Height;
    };

    // ---------------- //

    class Rotate: public Operation
    {
    public:
        static OperationPtr Create( const Tau::String& string );

    protected:
        Property::Int< Rotate > Index;
        Property::Int< Rotate > Shift;

    private:
        void SetValues( const Tau::String& string )
        {
            Tau::String index, shift;
            Tau::StringUtils::Split( string, index, " by ", shift, false );
            Index = Tau::StringUtils::To< int >( index );
            Shift = Tau::StringUtils::To< int >( shift );
            TAU_ASSERT( Shift > 0 )
        }
    };

    class RotateRow: public Rotate
    {
    public:
        virtual void Apply( Tau::Image* image ) const _override
        {
            TAU_ASSERT( image->Format_ == Tau::Image::FORMAT_GRAY )

            int shift = Shift % image->Width;
            if( shift == 0 ) { return; }

            char* p = image->Locate( 0, Index );
            Tau::BufferPtr tmp( new Tau::Buffer( shift ) );
            memcpy( *tmp, p + image->Width - Shift, Shift );
            memmove( p + Shift, p, image->Width - Shift );
            memcpy( p, *tmp, Shift );
        }
    };

    class RotateColumn: public Rotate
    {
    public:
        virtual void Apply( Tau::Image* image ) const _override
        {
            TAU_ASSERT( image->Format_ == Tau::Image::FORMAT_GRAY )

            int shift = Shift % image->Height;
            if( shift == 0 ) { return; }

            char* src = image->Locate( Index, 0 );
            Tau::BufferPtr tmp( new Tau::Buffer( Shift ) );
            ColumnCopy( src + ( image->Height - Shift ) * image->Pitch, *tmp, Shift, image->Pitch, 1 );
            ColumnMove( src, src + Shift * image->Pitch, image->Height - Shift, image->Pitch );
            ColumnCopy( *tmp, src, Shift, 1, image->Pitch );
        }

    private:
        static void ColumnCopy( const char* src, char* dst, int size, int srcPitch, int dstPitch )
        {
            while( size-- )
            {
                *dst = *src;
                src += srcPitch;
                dst += dstPitch;
            }
        }

        static void ColumnMove( const char* src, char* dst, int size, int pitch )
        {
            int jump = ( size - 1 ) * pitch;
            src += jump;
            dst += jump;

            while( size-- )
            {
                *dst = *src;
                src -= pitch;
                dst -= pitch;
            }
        }
    };

    OperationPtr Rotate::Create( const Tau::String& string )
    {
        Tau::String left, right;
        Tau::StringUtils::Split( string, left, ' ', right, false );
        TAU_ASSERT( ( left == "row" && Tau::StringUtils::StartsWith( right, "y=" ) ||
                    ( left == "column" && Tau::StringUtils::StartsWith( right, "x=" ) ) ) )

        OperationPtr rotate( left == "row" ? static_cast< Operation* >( new RotateRow() ) :
                                             static_cast< Operation* >( new RotateColumn() ) );
        rotate.Cast< Rotate >()->SetValues( Tau::StringUtils::GetAfter( right, "=", false ) );
        return rotate;
    }

    // ---------------- //

    static const Tau::Index2D DisplaySize( 50, 6 );

    AOC8::AOC8()
        : m_image( new Tau::Image( DisplaySize.X, DisplaySize.Y, Tau::Image::FORMAT_GRAY ) )
    {
        m_operations.Register( "rect", Rect::Create );
        m_operations.Register( "rotate", Rotate::Create );
    }

    void AOC8::Solve( const Tau::String& fileName )
    {
        Tau::File::ProcessLines( fileName, TAU_DELEGATE( bool, AOC8, ProcessLine, const Tau::String& ) );

        for( int i = 0; i < m_image->Size; i++ )
        {
            if( m_image->Data[i] ) { ResultA++; }
        }

        ResultB = m_image;
    }

    bool AOC8::ProcessLine( const Tau::String& line )
    {
        Tau::String operation, detail;
        Tau::StringUtils::Split( line, operation, ' ', detail, false );
        m_operations.Create( operation, detail )->Apply( m_image );
        return true;
    }

}
