#pragma once

#include <vector>

#include <boost/system/system_error.hpp>

#define CPU_ONLY
#include <caffe/layer.hpp>
#include <caffe/blob.hpp>
#include <caffe/proto/caffe.pb.h>

namespace caffe {

template<typename Dtype>
class GrayLayer
: public Layer<Dtype>
{
public:
    using blob_vector_type = vector<Blob<Dtype>*>;

public:
    explicit GrayLayer(const LayerParameter& params)
    : Layer<Dtype>(params)
    {}

    virtual inline const char* type() const         { return "Gray"; }
    virtual inline int ExactNumBottomBlobs() const  { return 1; }
    virtual inline int MinTopBlobs() const          { return 1; }

    virtual void Reshape(const blob_vector_type& bottom, const blob_vector_type& top)
    {
        auto img_count = bottom[0]->shape(0);
        top[0]->Reshape({img_count,1,32,32});
    }

protected:
    virtual void Forward_cpu(const blob_vector_type& bottom, const blob_vector_type& top)
    {
        auto* top_data = top[0]->mutable_cpu_data();
        const auto* bottom_data = bottom[0]->cpu_data();
        const auto img_count = bottom[0]->shape(0);
        const auto data_count = bottom[0]->count(2);

        for (auto img = 0 ; img < img_count ; ++img) {
            top_data[img * ] = 
        }
    }

    virtual void Backward_cpu(
        const blob_vector_type&, 
        const std::vector<bool>&, 
        const blob_vector_type&
    )
    { NOT_IMPLEMENTED; }
};

}   // namespace caffe