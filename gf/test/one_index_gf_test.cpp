/*
 * Copyright (C) 1998-2016 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "one_index_gf_test.hpp"

TEST_F(OneIndexGFTest,access)
{
    alps::gf::matsubara_index omega; omega=4;

    gf(omega)=std::complex<double>(3,4);
    std::complex<double> x=gf(omega);
    EXPECT_EQ(3, x.real());
    EXPECT_EQ(4, x.imag());
}

TEST_F(OneIndexGFTest,init)
{
    alps::gf::matsubara_index omega; omega=4;

    gf.initialize();
    std::complex<double> x=gf(omega);
    EXPECT_EQ(0, x.real());
    EXPECT_EQ(0, x.imag());
}

TEST_F(OneIndexGFTest,saveload)
{
    namespace g=alps::gf;
    {
        alps::hdf5::archive oar("gf.h5","w");
        gf(g::matsubara_index(4))=std::complex<double>(7., 3.);
        gf.save(oar,"/gf");
    }
    {
        alps::hdf5::archive iar("gf.h5");
        gf2.load(iar,"/gf");
    }
    EXPECT_EQ(7, gf2(g::matsubara_index(4)).real());
    EXPECT_EQ(3, gf2(g::matsubara_index(4)).imag());
    {
        alps::hdf5::archive oar("gf.h5","rw");
        oar["/gf/version/major"]<<7;
        EXPECT_THROW(gf2.load(oar,"/gf"),std::runtime_error);
    }
    EXPECT_EQ(7, gf2(g::matsubara_index(4)).real());
    EXPECT_EQ(3, gf2(g::matsubara_index(4)).imag());
}



TEST_F(OneIndexGFTest,print)
{
  std::stringstream gf_stream;
  gf_stream<<gf;

  std::stringstream gf_stream_by_hand;
  gf_stream_by_hand<<matsubara_mesh(beta,nfreq);
  for(int i=0;i<nfreq;++i){
    gf_stream_by_hand<<(2*i+1)*M_PI/beta<<" 0 0"<<std::endl;
  }
  EXPECT_EQ(gf_stream_by_hand.str(), gf_stream.str());
}

TEST_F(OneIndexGFTest,scaling)
{
    alps::gf::matsubara_index omega; omega=4;

    gf(omega)=std::complex<double>(3,4);
    gf *= 2.;
    std::complex<double> x=gf(omega);
    EXPECT_NEAR(6, x.real(),1.e-10);
    EXPECT_NEAR(8, x.imag(),1.e-10);

    alps::gf::omega_gf gf1=gf/2;
    std::complex<double> x1=gf1(omega);
    EXPECT_NEAR(3, x1.real(),1.e-10);
    EXPECT_NEAR(4, x1.imag(),1.e-10);
}

TEST_F(OneIndexGFTest,Assign)
{
    namespace g=alps::gf;
    g::omega_gf other_gf(matsubara_mesh(beta, nfreq*2));
    const g::matsubara_index omega(4);
    const std::complex<double> data(3,4);
    gf(omega)=data;
    
    gf2=gf;
    EXPECT_EQ(data, gf2(omega));
    
    EXPECT_THROW(other_gf=gf, std::invalid_argument);
    // EXPECT_EQ(data, other_gf(omega));
}
