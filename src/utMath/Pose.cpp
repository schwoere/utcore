/*
 * Ubitrack - Library for Ubiquitous Tracking
 * Copyright 2006, Technische Universitaet Muenchen, and individual
 * contributors as indicated by the @authors tag. See the 
 * copyright.txt in the distribution for a full listing of individual
 * contributors.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */


#include "Pose.h"
#include "Matrix.h"

namespace Ubitrack { namespace Math {


Pose::Pose( const Matrix< double, 0, 0 >& mat )
{
	m_rotation = Quaternion( mat );

	m_translation( 0 ) =  mat( 0, 3 );
	m_translation( 1 ) =  mat( 1, 3 );
	m_translation( 2 ) =  mat( 2, 3 );
}

Pose Pose::operator~( ) const
{
	Quaternion rinv( ~(m_rotation) );
	return Pose( rinv, -( rinv * m_translation ) );
}

Pose Pose::operator*( const Pose& Q ) const
{
	return Pose(
		  m_rotation * Q.rotation(),
		( m_rotation * Q.translation() ) + m_translation
	);
}

Vector< double, 3 > Pose::operator*( const Vector< double, 3 >& x ) const
{
	return Vector< double, 3 >( ( m_rotation * x ) + m_translation );
}

std::ostream& operator<<( std::ostream& s, const Pose& p )
{
	s << p.m_translation << " " << p.m_rotation;
	return s;
}

Pose linearInterpolate ( const Pose& x, const Pose& y, double t )
{
	return Pose( 
		slerp( x.rotation(), y.rotation(), t ), 
		linearInterpolate( x.translation(), y.translation(), t ) 
	);
}


} } // namespaceUbitrack::Math

