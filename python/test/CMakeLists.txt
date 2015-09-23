include(ALPSEnableTests)

set (test_src
    dict_to_params
    )


foreach(test ${test_src})
    alps_add_gtest(${test})
endforeach(test)
