#pragma once

#include <type_traits>

#define CH_INLINE inline
template<class T>
class CArray
{
private:
	uint32_t	m_Count;
	uint32_t	m_Capacity;
	T *			m_Data;

public:
	CArray()
		: m_Count( 0 )
		, m_Capacity( 16 )
		, m_Data( new T[ 16 ] )
	{
	}

	CArray( const T * raw, uint32_t count )
	{
		delete[] m_Data;
		m_Count = count;
		m_Capacity = count < 17 ? 16 : count + 1;
		m_Data = new T[ count ];
		memcpy( m_Data, raw, sizeof( T ) * count );
	}

	CArray( const CArray<T> & source )
	{
		m_Count = source.m_Count;
		m_Capacity = source.m_Capacity;
		m_Data = new T[ source.m_Capacity ];
		memcpy( m_Data, source.m_Data, sizeof( T ) * source.m_Count );
	}

	CH_INLINE CArray( CArray && source ) noexcept
	{
		delete[] m_Data;
		m_Data = source.m_Data;
		m_Count = source.m_Count;
		m_Capacity = source.m_Capacity;
		source.m_Data = nullptr;
		source.m_Count = 0;
		source.m_Capacity = 0;
	}

	void operator = ( const CArray<T> & source )
	{
		delete[] m_Data;
		m_Count = source.m_Count;
		m_Capacity = source.m_Capacity;
		m_Data = new T[ source.m_Capacity ];
		memcpy( m_Data, source.m_Data, sizeof( T ) * source.m_Count );
	}

	T getCopy( uint32_t index )
	{
		return isValidIndex( index ) ? m_Data[ index ] : throw std::out_of_range( "Out of range exception" );
	}

	CH_INLINE T & operator [] ( uint32_t index )
	{
		return isValidIndex( index ) ? m_Data[ index ] : throw std::out_of_range( "Out of range exception" );
	}

	CH_INLINE const T & operator [] ( uint32_t index ) const
	{
		return isValidIndex( index ) ? m_Data[ index ] : throw std::out_of_range( "Out of range exception" );
	}

	void push( const T & element )
	{
		if ( m_Count >= m_Capacity )
			grow();

		m_Data[ m_Count++ ] = element;
	}

	CH_INLINE void remove( uint32_t index )
	{ 
		m_Data[ index ] = m_Data[ --m_Count ];
	}

	CH_INLINE bool empty()
	{
		return m_Count < 1;
	}

	const T * raw() const
	{
		return m_Data;
	}

	uint32_t length() const
	{
		return m_Count;
	}

private:
	void grow()
	{
		m_Capacity *= 2;
		T * allo = new T[ m_Capacity ];
		if constexpr ( std::is_move_assignable_v<T> )
		{
			std::move( m_Data, m_Data + m_Count, allo );
		}
		else
		{
			std::copy( m_Data, m_Data + m_Count, allo );
		}
		delete[] m_Data;
		m_Data = allo;
	}

	CH_INLINE bool isValidIndex( int32_t index )
	{
		return index < m_Count && index >= 0;
	}

	template<class FromType>
	void copy( const FromType * source, uint32_t size )
	{

	}
};