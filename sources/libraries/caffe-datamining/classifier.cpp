#include <devfest2016/classifier.hpp>

namespace devfest2016 {
    
classifer::classifer()
{
    // Activate CPU Mode
    Caffe::set_mode(Caffe::CPU);
}
    
}   // namespace devfest2016