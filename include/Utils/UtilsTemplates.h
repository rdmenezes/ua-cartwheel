// This file contains all the template instanciations for things needed in Utils

// Instanciate used STL classes
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4275 4251)
#endif

#include <vector>

namespace CartWheel { namespace Util {

class Observer;

//UTILS_TEMPLATE( std::vector<int> )
//UTILS_TEMPLATE( std::vector<unsigned int> )
//UTILS_TEMPLATE( std::vector<double> )

}
}

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
