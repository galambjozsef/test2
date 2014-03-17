#ifndef CHECKEDCAST_H
#define CHECKEDCAST_H



template< class T2, class T1 > T2 checked_cast( const T1 t )
{
  return( static_cast< T2 >( t ) ) ;
}

#endif // CHECKEDCAST_H
