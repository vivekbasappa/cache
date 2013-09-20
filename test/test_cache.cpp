/**
 * filename: test_cache.cpp
 *
 * Original Author: Vivek Basappa Reddy
 * Date           : 23 September, 2013
 *
 * Description:
 *      testing cache class
 *
 * $Id$
 *
 */
#include "../include/Cache.h"
#include <iostream>

int
main(void)
{
    Cache<std::string, double> cc(4);

    cc.insert("pi", 3.14);
    cc.insert("e", 2.17);
    cc.insert("gold", 1.61);
    cc.insert("sq2", 1.14);

    //cc.debug_dump();

    cc.insert("zero", 0);

    double* e_value = cc.find("e");

    cc.insert("one", 1);

    //cc.debug_dump();
    cc.statistics(std::cout);

    for (int i = 0 ; i < 30; ++i)
        double *one_value = cc.find("one");

    cc.statistics(std::cout);
}
