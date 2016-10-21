import numpy as np
import requests
import os
import random
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import json
import time
import StringIO
import matplotlib.image as mpimg
import pandas as pd
import skimage.io

# vieille function pour afficher les images dans zeppelin

def show(p):
    img = StringIO.StringIO()
    p.savefig(img, format='svg')
    img.seek(0)
    print "%html <div style='width:600px;height:600px'>" + img.buf + "</div>"

# endroit où sont stockées les images & les métadonnées correspondantes

data_dir = "/home/expand/devfest/data"
data_dir_cifar10 = os.path.join(data_dir, "cifar-10-batches-py")
path = os.path.join(data_dir_cifar10, "test_batch")
batch = np.load(path)
class_names_cifar10 = np.load(os.path.join(data_dir_cifar10, "batches.meta"))

# différentes images de test

img_url = "/tmp/airplane3.png"
img_url = "http://i2.cdn.turner.com/cnnnext/dam/assets/141027105451-pkg-petroff-ferrari-sergio-elite-car-00014417-story-top.jpg"
img_url = "http://buzzsharer.com/wp-content/uploads/2015/06/beautiful-running-horse.jpg"
img_url = "https://upload.wikimedia.org/wikipedia/commons/9/95/MSMajestyOfTheSeasEdit1.JPG"
img_url = "http://media.moddb.com/images/members/1/193/192959/StarshipB.jpg"
img_url = "http://pop.h-cdn.co/assets/15/53/980x490/landscape-1451338932-milleniumfalcon.jpg"
img_url = "http://cdn.wegotthiscovered.com/wp-content/uploads/MillenniumFalcon-SWE-505x360.jpg"
img_url = "http://printf.eu/wp-content/uploads/2013/02/Xwing.jpg"
img_url = "http://dreamatico.com/data_images/deer/deer-3.jpg"
img_url = "http://i2.cdn.turner.com/cnnnext/dam/assets/141027105451-pkg-petroff-ferrari-sergio-elite-car-00014417-story-top.jpg"
img_url = "http://buzzsharer.com/wp-content/uploads/2015/06/beautiful-running-horse.jpg"
img_url = "http://fleetowner.com/site-files/fleetowner.com/files/archive/blog.fleetowner.com/trucks_at_work/wp-content/uploads/2009/09/fo09flhd.jpg"
img_url = "http://animalia-life.com/data_images/frog/frog5.jpg"

# la request vers le serveur en elle même 
# y a la fonction predict_one qui load directement une image à partir d'une url
# il y en a une autre à refaire sans doute : predict_batch à qui on enverra directement X images

r = requests.post('http://localhost:5000/predict_one',json = json.dumps(img_url))

print(pd.DataFrame({'label':class_names_cifar10['label_names'], 'prob':r.json()[0]}).sort_values('prob',ascending=False))



