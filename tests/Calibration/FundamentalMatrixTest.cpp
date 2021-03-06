#include <vector>
#include <iostream>
#include <math.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <utCalibration/FundamentalMatrix.h>
#include <utMath/MatrixOperations.h>
#include "../tools.h"

using namespace Ubitrack;
namespace ublas = boost::numeric::ublas;


void TestFundamentalMatrix()
{
	for( int j=0; j<100; j++ )
	{
		//compute fundamental matrix according to F = |e|x * P1 * P2+
		Math::Pose CamPose1( randomQuaternion() , randomVector< double, 3 >() );
		Math::Pose CamPose2( randomQuaternion() , randomVector< double, 3 >() );

		Math::Matrix< double, 3, 3 > I;

		I( 0, 0 ) = 400;
		I( 0, 1 ) = 0;
		I( 0, 2 ) = -160;
		I( 1, 0 ) = 0;
		I( 1, 1 ) = 400;
		I( 1, 2 ) = -120;
		I( 2, 0 ) = 0;
		I( 2, 1 ) = 0;
		I( 2, 2 ) = -1;

		Math::Matrix< double, 3, 4 > E1( CamPose1 );
		Math::Matrix< double, 3, 4 > E2( CamPose2 );

		E1 = ublas::prod( I, E1);
		E2 = ublas::prod( I, E2);

		Math::Matrix< double, 3, 3 > F = Calibration::fundamentalMatrixFromPoses( CamPose1, CamPose2, I, I );

		//compute random points and according fundamental matrix
		std::vector< Math::Vector< double, 2 > > fromPoints;
		std::vector< Math::Vector< double, 2 > > toPoints;

		for( int i=0; i<60; i++ )
		{
			Math::Vector< double, 4 > v = randomVector< double, 4 >();
			Math::Vector< double, 3 > v1 = ublas::prod( E1, v );
			Math::Vector< double, 3 > v2 = ublas::prod( E2, v );
			
			fromPoints.push_back( Math::Vector< double, 2 >( v1( 0 )/v1( 2 ), v1( 1 )/v1( 2 ) ) );
			toPoints.push_back( Math::Vector< double, 2 >( v2( 0 )/v2( 2 ), v2( 1 )/v2( 2 ) ) );
		}

		Math::Matrix< double, 3, 3 > FTest = Calibration::getFundamentalMatrix( fromPoints, toPoints );

		BOOST_CHECK_SMALL( homMatrixDiff( F, FTest ), 0.001 );
	}
}