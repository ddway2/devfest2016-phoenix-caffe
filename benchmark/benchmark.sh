#!/bin/bash
ME=$(basename $0)
MYDIR=$(dirname $0)
MYDIR=$(cd $MYDIR && pwd)

ab -n 200 -c 8 -p data.post -H "Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryx6EUBFb8zVKN4FNP" http://127.0.0.1:8081/predict_from_file

