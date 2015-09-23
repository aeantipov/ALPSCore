/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "alps/python.hpp"
#include "gtest/gtest.h"

TEST(param,CompareConst)
{

  boost::python::dict dict1;
  dict1["1"]=1;
  dict1["1.25"]=1.25;
  dict1["str"]="1.25";
  
  alps::params p = alps::dict_to_params(dict1);

  EXPECT_TRUE(p["1"]==1);
  EXPECT_TRUE(p["1.25"]==1.25);
}
