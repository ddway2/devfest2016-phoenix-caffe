#include <devfest2016/custom_layer.hpp>

namespace caffe {

void 
GrayLayer<Dtype>::Reshape(const blob_vector_type& bottom, const blob_vector_type& top)
{
    // TODO reshape (i dont understand)
}

template<typename Dtype>
void 
GrayLayer<Dtype>::Forward_cpu(const blob_vector_type& bottom, const blob_vector_type& top)
{
    for (size_t i = 0 ; i < top.size() ; ++i) {
        //top[i]->SharedData();
     }
}

INSTANTIATE_CLASS(GrayLayer);
REGISTER_LAYER_CLASS(Gray);

}   // namespace caffe
