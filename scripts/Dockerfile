FROM ubuntu:14.04

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
        wget \
        libatlas-base-dev \
        libboost-all-dev \
        libgflags-dev \
        libgoogle-glog-dev \
        libhdf5-serial-dev \
        libleveldb-dev \
        liblmdb-dev \
        libopencv-dev \
        libprotobuf-dev \
        libsnappy-dev \
        protobuf-compiler \
        python-dev \
        python-numpy \
        python-pip \
        python-scipy && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir /src && cd /src && \
    git clone https://github.com/BVLC/caffe.git && \
    cd caffe && \
    for req in $(cat python/requirements.txt) pydot; do pip install $req; done && \
    mkdir build && cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCPU_ONLY=1 .. && \
    make -j"$(nproc)" && \
    make install && \
    rm -rf /src

RUN pip install flask && pip install flask_cors

ADD . /scripts
WORKDIR /scripts
CMD ["python", "./predict_caffe.py"]

        
