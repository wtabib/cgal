#ifndef CGAL_LA_EIGEN_H
#define CGAL_LA_EIGEN_H

#ifndef CGAL_USE_EIGEN
#error Requires Eigen
#endif
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <CGAL/Dimension.h>
#include <Eigen/Dense>
#include <CGAL/LA_eigen/constructors.h>
#include <CGAL/iterator_from_indices.h>

namespace CGAL {

//FIXME: where could we use Matrix_base instead of Matrix?
// Dim_ real dimension
// Max_dim_ upper bound on the dimension
template<class NT_,class Dim_,class Max_dim_=Dim_> struct LA_eigen {
	typedef NT_ NT;
	typedef Dim_ Dimension;
	typedef Max_dim_ Max_dimension;
	enum { dimension = Eigen_dimension<Dimension>::value };
	enum { max_dimension = Eigen_dimension<Max_dimension>::value };
	template< class D2, class D3=D2 >
	struct Rebind_dimension {
	  typedef LA_eigen< NT, D2, D3 > Other;
	};
	template<class> struct Property : boost::false_type {};

	typedef Eigen::Matrix<NT,Eigen_dimension<Dim_>::value,1,Eigen::ColMajor|Eigen::AutoAlign,Eigen_dimension<Max_dim_>::value,1> Vector;
	typedef Eigen::Matrix<NT,Eigen::Dynamic,1> Dynamic_vector;
	typedef Construct_eigen<Vector> Construct_vector;

#if (EIGEN_WORLD_VERSION>=3)
	typedef NT const* Vector_const_iterator;
#else
	typedef Iterator_from_indices<const type,const NT
#ifndef CGAL_CXX0X
	  ,NT
#endif
	  > Vector_const_iterator;
#endif

	template<class Vec_>static Vector_const_iterator vector_begin(Vec_ const&a){
#if (EIGEN_WORLD_VERSION>=3)
	  return &a[0];
#else
	  return Vector_const_iterator(a,0);
#endif
	}

	template<class Vec_>static Vector_const_iterator vector_end(Vec_ const&a){
#if (EIGEN_WORLD_VERSION>=3)
	  return &a[0]+a.size();
#else
	  return Vector_const_iterator(a,a.size());
#endif
	}

	typedef Eigen::Matrix<NT,dimension,dimension,Eigen::ColMajor|Eigen::AutoAlign,max_dimension,max_dimension> Square_matrix;
	typedef Eigen::Matrix<NT,dimension,Eigen::Dynamic,Eigen::ColMajor|Eigen::AutoAlign,max_dimension,Eigen::Dynamic> Dynamic_matrix;
		//TODO: don't pass on the values of Max_* for an expensive NT
                // typedef ... Constructor
                // typedef ... Accessor
#if 0
	private:
	template <class T> class Canonicalize_vector {
		typedef typename Dimension_eigen<T::SizeAtCompileTime>::type S1;
		typedef typename Dimension_eigen<T::MaxSizeAtCompileTime>::type S2;
		public:
		typedef typename Vector<S1,S2>::type type;
	};
	public:
#endif

	template<class Vec_>static int size_of_vector(Vec_ const&v){
		return v.size();
	}

	template<class Vec_>static NT dot_product(Vec_ const&a,Vec_ const&b){
		return a.dot(b);
	}

	template<class Mat_> static NT determinant(Mat_ const&m,bool=false){
		return m.determinant();
	}

	template<class Mat_> static typename
	Same_uncertainty_nt<CGAL::Sign, NT>::type
	sign_of_determinant(Mat_ const&m,bool=false)
	{
		return CGAL::sign(m.determinant());
	}

	template<class Vec1,class Vec2> static Vector homogeneous_add(Vec1 const&a,Vec2 const&b){
		//TODO: use compile-time size when available
		int d=a.size();
		Vector v(d);
		v << b[d-1]*a.topRows(d-1)+a[d-1]*b.topRows(d-1), a[d-1]*b[d-1];
		return v;
	}

	template<class Vec1,class Vec2> static Vector homogeneous_sub(Vec1 const&a,Vec2 const&b){
		int d=a.size();
		Vector v(d);
		v << b[d-1]*a.topRows(d-1)-a[d-1]*b.topRows(d-1), a[d-1]*b[d-1];
		return v;
	}

	template<class Vec1,class Vec2> static std::pair<NT,NT> homogeneous_dot_product(Vec1 const&a,Vec2 const&b){
		int d=a.size();
		return make_pair(a.topRows(d-1).dot(b.topRows(d-1)), a[d-1]*b[d-1]);
	}

};
}
#endif
