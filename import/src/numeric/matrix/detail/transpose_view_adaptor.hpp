/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_MATRIX_TRANSPOSE_VIEW_ADAPTOR_HPP
#define ALPS_MATRIX_TRANSPOSE_VIEW_ADAPTOR_HPP

#include <boost/numeric/bindings/detail/adaptor.hpp>
namespace alps { namespace numeric {

    template <typename T, typename MemoryBlock>
    class matrix;

    template <typename Matrix>
    class transpose_view;
} }
//
// An adaptor for the matrix to the boost::numeric::bindings
//

namespace boost { namespace numeric { namespace bindings { namespace detail {

    template <typename T, typename MemoryBlock, typename Id, typename Enable>
    struct adaptor< ::alps::numeric::transpose_view< ::alps::numeric::matrix<T,MemoryBlock> >, Id, Enable>
    {
        typedef typename copy_const< Id, T >::type              value_type;
        // TODO: fix the types of size and stride -> currently it's a workaround, since std::size_t causes problems with boost::numeric::bindings
        //typedef typename ::alps::numeric::matrix<T,Alloc>::size_type         size_type;
        //typedef typename ::alps::numeric::matrix<T,Alloc>::difference_type   difference_type;
        typedef std::ptrdiff_t  size_type;
        typedef std::ptrdiff_t  difference_type;

        typedef mpl::map<
            mpl::pair< tag::value_type,      value_type >,
            mpl::pair< tag::entity,          tag::matrix >,
            mpl::pair< tag::size_type<1>,    size_type >,
            mpl::pair< tag::size_type<2>,    size_type >,
            mpl::pair< tag::data_structure,  tag::linear_array >,
            mpl::pair< tag::data_order,      tag::row_major >,
            mpl::pair< tag::data_side,       tag::lower >,
            mpl::pair< tag::stride_type<1>,  difference_type >,
            mpl::pair< tag::stride_type<2>,  tag::contiguous >
        > property_map;

        static size_type size1( const Id& id ) {
            return id.num_rows();
        }

        static size_type size2( const Id& id ) {
            return id.num_cols();
        }

        static value_type* begin_value( Id& id ) {
            return &(*id.col(0).first);
        }

        static value_type* end_value( Id& id ) {
            return &(*(id.col(id.num_cols()-1).second-1));
        }

        static difference_type stride1( const Id& id ) {
            return id.stride1();
        }

        static difference_type stride2( const Id& id ) {
           return id.stride2();
        }

    };

}}}}

#endif //ALPS_MATRIX_TRANSPOSE_VIEW_ADAPTOR_HPP
