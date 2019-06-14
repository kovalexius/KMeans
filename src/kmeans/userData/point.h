#ifndef POINT
#define POINT

#include <utility>
#include <exception>
#include <stdexcept>
#include <ostream>

template< unsigned int N >
class CPoint
{
    float m_coord[N];
    
public:
    CPoint()
    {
        for( unsigned int i = 0; i < N; i++ )
            m_coord[i] = 0;
    }

    CPoint( const CPoint<N>& other )
    {
        for( unsigned int i = 0; i < N; i++ )
            m_coord[i] = other.m_coord[i];
    }

    CPoint<N>& operator = (  const CPoint<N>& other )
    {
        for( unsigned int i = 0; i < N; i++ )
            m_coord[i] = other.m_coord[i];
        return *this;
    }

	CPoint<N>& operator = ( const int& other )
	{
		for (unsigned int i = 0; i < N; i++)
			m_coord[i] = other;
		return *this;
	}

    //! Получить размерность
    inline unsigned int getMetric() const
    {
        //return m_num;
        return N;
    }

    //! Записать/модифицировать i-тый компонент вектора
    inline void setCoord( unsigned int index, float val )
    {
        if( index < N )
        {
            m_coord[index] = val;
            return;
        }
        throw std::range_error("index too big");
    }

    //! Прочитать i-ый компонент вектора
    inline float getCoord( unsigned int index ) const
    {
        if( index < N )
            return m_coord[index];
        throw std::range_error("index too big");
    }

    //! Вычитание вектора
    inline CPoint<N> operator-( const CPoint<N>& other ) const
    {
        CPoint<N> vec;
        for( unsigned int i = 0; i < N; i++ )
            vec.m_coord[i] = m_coord[i] - other.m_coord[i];
        //return std::move( vec );
        return vec;
    }

    //! Сложение векторов
    inline CPoint<N> operator+( const CPoint<N>& other ) const
    {
        CPoint<N> vec;
        for( unsigned int i = 0; i < N; i++ )
            vec.m_coord[i] = m_coord[i] + other.m_coord[i];
        //return std::move( vec );
        return vec;
    }

	//! Сложение с присваиванием
    inline CPoint<N>& operator += (const CPoint<N>& other)
	{
		for (unsigned int i = 0; i < N; i++)
			m_coord[i] += other.m_coord[i];
		return *this;
	}

	//! Скалярное умножение векторов на другой вектор (без взятия корня)
    inline float operator*(const CPoint<N>& other) const
	{
		float result = 0.0;
		for (unsigned int i = 0; i < N; i++)
			result += m_coord[i] * other.m_coord[i];
		return result;
	}

    //! Масштабирование вектора на скалярное число
    inline CPoint<N> operator*( const float& mul ) const
    {
        CPoint<N> vec;
        for( unsigned int i = 0; i < N; i++ )
            vec.m_coord[i] = m_coord[i] * mul;
        //return std::move( vec );
        return vec;
    }

	//! Масштабирование с присваиванием
    inline CPoint<N>& operator*=(const float& mul)
	{
		for (unsigned int i = 0; i < N; i++)
			m_coord[i] *= mul;
		return *this;
	}

	//! Деление на скалярное число
    inline CPoint<N> operator / (const float& div) const
	{
		CPoint<N> vec;
		for (unsigned int i = 0; i < N; i++)
			vec.m_coord[i] = m_coord[i] / div;
		return vec;
	}

	//! Деление на число с присваиванием
    inline CPoint<N>& operator /= (const float& div)
	{
		for (unsigned int i = 0; i < N; i++)
			m_coord[i] /= div;
		return *this;
	}

    //! Получить "нуль-вектор"
    static inline CPoint<N> getZero()
    {
        CPoint<N> vec;
        for( unsigned int i = 0; i < N; i++ )
            vec.m_coord[i] = 0;
        //return std::move( vec );
        return vec;
    }
};

//! Оператор << для вывода содержимого CPoint<N> на печать
template<unsigned int N>
inline std::ostream& operator<<( std::ostream& os, const CPoint<N>& pnt )
{
    for( unsigned int i = 0; i < pnt.getMetric() - 1; i++ )
    {
        os << pnt.getCoord(i) << "; ";
    }
    os << pnt.getCoord( pnt.getMetric() - 1 ) << std::endl;
    return os;
}

#endif // POINT

