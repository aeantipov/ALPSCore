#include <vector>

#include "boost/mpl/vector.hpp"
#include "boost/mpl/set.hpp"
#include "boost/mpl/insert.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/placeholders.hpp"
#include "boost/mpl/bool.hpp"
#include <boost/mpl/and.hpp>
#include <boost/mpl/logical.hpp>

#include "boost/variant.hpp"

#ifndef ALPS_PARAMS_PARAM_TYPES_INCLUDED
#define ALPS_PARAMS_PARAM_TYPES_INCLUDED

namespace alps {
    namespace params_ns {
        namespace detail {
            
            // Have namespaces handy
            namespace mpl=::boost::mpl;
            namespace mplh=::boost::mpl::placeholders;

            /// "Empty value" type
            struct None {};

            /// Output operator for the "empty value" (@throws runtime_error always)
            std::ostream& operator<<(std::ostream& s, const detail::None&)
            {
                throw std::runtime_error("Attempt to print uninitialized option value");
            }
  
            // Vector of allowed scalar types:
            typedef mpl::vector<int,double,bool>::type scalar_types_vec;

            /// Make a set of allowed types (for fast look-up)
            typedef mpl::fold< scalar_types_vec,
                               mpl::set<>, // empty set
                               mpl::insert<mplh::_1,mplh::_2>
                               >::type scalar_types_set;

            // Vector of std::vector<T> types (aka "vector types")
            typedef mpl::transform< scalar_types_vec, std::vector<mplh::_1> >::type vector_types_subvec;

            // Add std::string to the vector of the "vector types"
            typedef mpl::push_front<vector_types_subvec, std::string>::type vector_types_vec;

            /// Make a set of "vector types" (for fast look-up):
            typedef mpl::fold< vector_types_vec,
                               mpl::set<>, // empty set
                               mpl::insert<mplh::_1,mplh::_2>
                               >::type vector_types_set;

            // Make a set of all types (for fast look-up):
            typedef mpl::fold< vector_types_set,
                               scalar_types_set, 
                               mpl::insert<mplh::_1,mplh::_2>
                               >::type all_types_set;

            // Make a vector of all types (for boost::variant, starting with the scalar types)
            typedef mpl::fold< vector_types_vec, 
                               scalar_types_vec, 
                               mpl::push_back<mplh::_1, mplh::_2>
                               >::type all_types_vec;

            /// A variant of all types, including None (as the first one)
            typedef boost::make_variant_over< mpl::push_front<all_types_vec,None>::type >::type variant_all_type;

            /// A meta-function determining if both types are scalar
            template <typename T, typename U>
            struct both_scalar
                : mpl::and_< mpl::has_key<scalar_types_set,U>,
                             mpl::has_key<scalar_types_set,T> >
            {};

        }

        // Elevate choosen generated types:
        using detail::scalar_types_set;
        using detail::vector_types_set;
        using detail::all_types_set;
        using detail::variant_all_type;
    } // params_ns
}// alps

#endif // ALPS_PARAMS_PARAM_TYPES_INCLUDED
