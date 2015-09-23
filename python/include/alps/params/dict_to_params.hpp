/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_DICT_TO_PARAMS_INCLUDED
#define ALPS_DICT_TO_PARAMS_INCLUDED

#include <boost/python/dict.hpp>
#include <alps/params.hpp>

namespace alps { 

alps::params dict_to_params(boost::python::dict const & arg);

} // end of namespace alps

#endif // #ifndef ALPS_DICT_TO_PARAMS_INCLUDED
