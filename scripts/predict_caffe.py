import numpy as np
import json 
import pandas as pd
#from PIL import Image
import sys
import os
from flask import Flask
from flask import request, Response
from flask_cors import CORS, cross_origin
import skimage.io



app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = '/tmp/'
CORS(app)
cors = CORS(app, resources={r"*": {"origins": "*"}})

caffe_root = '/usr/lib/caffe'
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

def get_file(filename): 
    try:
        src = os.path.join(filename)
        return open(src).read()
    except IOError as exc:
        return str(exc)

@app.route("/demo", methods = ['GET'])
def index():
	print("hello")
	#return app.send_static_file('/home/expand/devfest2016-phoenix-caffe/slides/demo.html')
	content = get_file('/home/expand/devfest2016-phoenix-caffe/slides/demo.html')
	return Response(content)

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
	
	#x          = net.predict( [ caffe.io.load_image(file) ])
	img         = skimage.img_as_float(skimage.io.imread(os.path.join(app.config['UPLOAD_FOLDER'], filename), as_grey=False)).astype(np.float32)
	class_names = ["airplane","automobile","bird","cat","deer","dog","frog","horse","ship","truck"]

	x   = net.predict([img]).tolist()[0]
	x   = [ '%.2f' % elem for elem in x ]
	ret = pd.DataFrame({'name':class_names, 'prediction':x}).sort_values('prediction',ascending=False).to_json(orient="records")
	return json.dumps({"predictions":ret})

if __name__ == "__main__":
    app.run(host='0.0.0.0')
