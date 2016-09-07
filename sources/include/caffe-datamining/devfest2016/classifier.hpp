#pragma once

#define CPU_ONLY
#include <caffe-datamining/config.h>

#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <memory>

namespace devfest2016 {

using namespace caffe;

class CAFFE_DATAMINING_API classifer
{
public:
    using network = Net<float>;
    using network_ptr = std::shared_ptr<network>;
    
public:
    classifer();
private:
    network_ptr     net_;
};
    
}   // namespace devfest2016