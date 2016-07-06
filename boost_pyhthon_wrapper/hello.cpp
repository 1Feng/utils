#include <boost/python.hpp>
#include <iostream>

//using namespace boost::python;

void greet() {
    std::cout << "hello" << std::endl;
}




BOOST_PYTHON_MODULE(hello)
{
    boost::python::def("greet", greet);
}
