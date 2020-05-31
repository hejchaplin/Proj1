#pragma once
#define CH_INLINE inline
class CString
{
	typedef unsigned int uint32_t;

	uint32_t	m_Size;
	char *		m_Str;

public:
	CString( const char * source )
		: m_Size( strPtrLen( source ) )
		, m_Str( new char[ m_Size ] )
	{
		for ( uint32_t i = 0; i < m_Size; ++i )
		{
			m_Str[ i ] = *source++;
		}
	}

	CString()
		: m_Size( 0 )
		, m_Str( nullptr )
	{
	}

	~CString()
	{
		delete[] m_Str;
	}

	CString( const CString & source )
	{
		m_Size = source.m_Size;
		m_Str = new char[ m_Size ];
		memcpy( m_Str, source.m_Str, sizeof( char ) * m_Size );
	}

	CH_INLINE CString( CString && source ) noexcept
	{
		m_Str = source.m_Str;
		m_Size = source.m_Size;
		source.m_Str = nullptr;
		source.m_Size = 0;
	}

	CH_INLINE CString & operator = ( CString && source ) noexcept
	{
		m_Str = source.m_Str;
		m_Size = source.m_Size;
		source.m_Str = nullptr;
		source.m_Size = 0;
		return *this;
	}

	CString & operator = ( const CString & source )
	{
		if ( m_Str != source.m_Str )
		{
			delete[] m_Str;
			m_Size = source.m_Size;
			m_Str = new char[ m_Size ];
			memcpy( m_Str, source.m_Str, sizeof( char ) * m_Size );
		}
		return *this;
	}

	bool operator == ( const CString & a ) const
	{
		return  strcmp( m_Str, a.raw() ) == 0;
	}

	CString & operator = ( const char * source )
	{
		if ( m_Str != source )
		{
			delete[] m_Str;
			m_Size = strPtrLen( source );
			m_Str = new char[ m_Size ];
			for ( uint32_t i = 0; i < m_Size; ++i )
			{
				m_Str[ i ] = *source++;
			}
		}
		return *this;
	}

	CString & operator += ( const CString & source )
	{
		append( source.raw(), source.length() );
		return *this;
	}

	CString & operator += ( const char * source )
	{
		append( source, strPtrLen( source ) - 1 );
		return *this;
	}

	CString & operator + ( const CString & source )
	{
		append( source.raw(), source.length() );
		return *this;
	}

	CString & operator + ( const char * source )
	{
		append( source, strPtrLen( source ) - 1 );
		return *this;
	}

	CH_INLINE const char * raw() const
	{
		return m_Str;
	}

	CH_INLINE uint32_t length() const
	{
		return m_Size - 1;
	}

	char getRightmost() const
	{
		if ( m_Size < 2 )
		{
			return *m_Str;
		}
		return m_Str[ m_Size - 2 ];
	}

	char getLeftmost() const
	{
		return *m_Str;
	}

	void append( const char * source, uint32_t count )
	{
		if ( count < 1 )
			return;

		m_Size += count;
		char * replacement = new char[ m_Size ];
		int i;
		for ( i = 0; m_Str[ i ] != 0; ++i )
		{
			replacement[ i ] = m_Str[ i ];
		}

		for ( int j = 0; source[ j ] != 0; j++, i++ )
		{
			replacement[ i ] = source[ j ];
		}
		replacement[ i ] = 0;
		delete[] m_Str;
		m_Str = replacement;
	}

	//split return array of original string, split on char input, and removes the char that it's split on
	//trim removes whitespace on left, right or both sides (not in middle)
	//substring, input = start, end. |---> "henric" substring(0, 3) returns a string with value "henr"
private:
	uint32_t strPtrLen( const char * source )
	{
		if ( source == nullptr )
		{
			return 0;
		}

		const char * tmp = source;
		while ( *source++ != 0 );
		return source - tmp;
	}
};