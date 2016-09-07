#include <devfest2016/classifier.hpp>

namespace devfest2016 {
    
classifer::classifer(
    const std::string& model_file,
    const std::string& trained_file,
    const std::string& mean_file,
    const std::string& labels_file
)
{
    // Activate CPU Mode
    Caffe::set_mode(Caffe::CPU);
    
    // Load Network
    load_network(model_file, trained_file);
    
    // Load Mean
    load_mean(mean_file);
    
    // Load Label ?
    load_labels(labels_file);
}


predictions_result  
classifer::classify(const std::string& imgfile)
{
    cv::Mat img = cv::imread(imgfile.c_str(), -1);
    
    auto output = predict(img);
    predictions_result result;
    
    
    return result;
}

std::vector<float>  
classifer::predict(const cv::Mat& img)
{
    auto input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_, 
                         input_geometry_.height, input_geometry_.width);
    net_->Reshape();
    
    channels_list_type input_channels;
    
    // Prepare system
    wrap_input_layer(input_channels);
    pre_process(img, input_channels);
    
    net_->Forward();
    
    // Copy output vector
    auto output_layer = net_->output_blobs()[0];
    auto begin = output_layer->cpu_data();
    auto end = begin + output_layer->channels();
    
    return std::vector<float>(begin, end);
}

void 
classifer::wrap_input_layer(channels_list_type& input_channels)
{
    auto input_layer = net_->input_blobs()[0];
    
    auto width = input_layer->width();
    auto height = input_layer->height();
    auto input_data = input_layer->mutable_cpu_data();
    
    for (int i = 0 ; i < input_layer->channels(); ++i) {
        input_channels.emplace_back(height, width, CV_32FC1, input_data);
        input_data += width * height;
    }
}

void 
classifer::pre_process(const cv::Mat& img, channels_list_type& input_channels)
{
    // Convert image
    cv::Mat sample;
    if (img.channels() == 3 && num_channels_ == 1) {
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY); 
    } else if (img.channels() == 4 && num_channels_ == 1) {
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    } else if (img.channels() == 4 && num_channels_ == 3) {
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    } else if (img.channels() == 1 && num_channels_ == 3) {
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    } else {
        sample = img;
    }
    
    // Resize image
    cv::Mat sample_resized;
    if (sample.size() != input_geometry_) {
        cv::resize(sample, sample_resized, input_geometry_);
    } else {
        sample_resized = sample;
    }
    
    // Float
    cv::Mat sample_float;
    if (num_channels_ == 3) {
        sample_resized.convertTo(sample_float, CV_32FC3);
    } else {
        sample_resized.convertTo(sample_float, CV_32FC1);
    }
    
    // Normalize image
    cv::Mat sample_normalize;
    cv::subtract(sample_float, mean_, sample_normalize);
    cv::split(sample_normalize, input_channels);
}

void 
classifer::load_network(
    const std::string& model_file,
    const std::string& trained_file
)
{
    net_ = std::make_shared<network>(model_file, TEST);
    net_->CopyTrainedLayersFrom(trained_file);
    
    auto input_layer = net_->input_blobs()[0];
    
    num_channels_ = input_layer->channels();
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());
}

void 
classifer::load_mean(const std::string& mean_file)
{
    BlobProto blob_proto;
    ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);
    
    Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);
    
    std::vector<cv::Mat> channels;
    auto data = mean_blob.mutable_cpu_data();
    for (int i = 0 ; i < num_channels_ ; ++i) {
        channels.emplace_back(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        data += mean_blob.height() * mean_blob.width();
    }
    
    /* Merge the separate channels into a single image. */
    cv::Mat mean;
    cv::merge(channels, mean);

    /* Compute the global mean pixel value and create a mean image
     * filled with this value. */
    auto channel_mean = cv::mean(mean);
    mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
    
}

void 
classifer::load_labels(const std::string& labels_file)
{
    std::ifstream ifs(labels_file.c_str());
    std::string line;
    while (std::getline(ifs, line)) {
        labels_.emplace_back(line);
    }
}
    
}   // namespace devfest2016