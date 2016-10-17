import cPickle as pickle
#import conv_net
import numpy as np
import json 
from PIL import Image
import sys
import os
from flask import Flask
from flask import request
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = '/tmp'

caffe_root = '/usr'
sys.path.insert(0, caffe_root + '/python')

import caffe

def loadNet():
	blob = caffe.proto.caffe_pb2.BlobProto()
	data = open( 'share/mean.binaryproto' , 'rb' ).read()
	blob.ParseFromString(data)
	arr = np.array( caffe.io.blobproto_to_array(blob) )
	net = caffe.Classifier('share/cifar10_quick.prototxt', \
		'share/cifar10_quick_iter_5000.caffemodel.h5',\
	image_dims=(32, 32), mean=arr[0],raw_scale=255)

	return(net)

net = loadNet()

@app.route("/predict_from_file", methods = ['POST'])
def predict_from_file():
	app.logger.info('Predict OK')
	if 'upload_file' not in request.files:
		flash('No file part')
		return redirect(request.url)
	file = request.files['upload_file']
	app.logger.info('file found')
	if file.filename == '':
		flash('No Selected file')
		return redirect(request.url)
	
	app.logger.info('File loaded ' + file.filename)
	filename = file.filename
	file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
	
	x   = net.predict( [ caffe.io.load_image(file) ])
	return json.dumps(x.tolist())

if __name__ == "__main__":
    app.run(host='0.0.0.0')
