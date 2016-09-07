#pragma once

#define CPU_ONLY
#include <caffe-datamining/config.h>

#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <memory>
#include <string>
#include <vector>

namespace devfest2016 {

using namespace caffe;

using prediction = std::pair<std::string, float>;
using predictions_result = std::vector<prediction>;
using channels_list_type = std::vector<cv::Mat>;
using string_list_type = std::vector<std::string>;

class CAFFE_DATAMINING_API classifer
{
public:
    using network = Net<float>;
    using network_ptr = std::shared_ptr<network>;
    
public:
    classifer(
        const std::string& model_file,
        const std::string& trained_file,
        const std::string& mean_file,
        const std::string& labels_file
    );
    
    predictions_result  classify(const std::string& imgfile);
private:
    std::vector<float>  predict(const cv::Mat& img);
    void wrap_input_layer(channels_list_type& input_channels);
    void pre_process(const cv::Mat& img, channels_list_type& input_channels);

    void load_network(
        const std::string& model_file,
        const std::string& trained_file
    );
    
    void load_mean(const std::string& mean_file);
    
    void load_labels(const std::string& labels_file);
    
private:
    network_ptr     net_;
    
    string_list_type    labels_;
    
    cv::Size input_geometry_;
    int num_channels_ = 0;
    
    cv::Mat mean_;
};
    
}   // namespace devfest2016