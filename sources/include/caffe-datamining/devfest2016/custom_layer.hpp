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
    {}

protected:
    virtual void Forward_cpu(const blob_vector_type& bottom, const blob_vector_type& top)
    {}

    virtual void Backward_cpu(
        const blob_vector_type&, 
        const std::vector<bool>&, 
        const blob_vector_type&
    )
    { NOT_IMPLEMENTED; }
};

}   // namespace caffe