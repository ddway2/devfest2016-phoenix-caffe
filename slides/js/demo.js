 
    function allowDrop(ev) {
      ev.preventDefault();
    }

    function drag(ev) {
      ev.dataTransfer.setData("text", ev.target.id);
    }

    function drop(ev) {
      ev.preventDefault();
      var length = ev.dataTransfer.items.length;
      for (var i = 0; i < length; i++) {
        var entry = ev.dataTransfer.items[i].getAsFile();
        console.log(entry)
        var img = document.getElementById("drag1");
        var reader = new FileReader();
        reader.onload = (function (aImg) {
          return function (e) {
            // display preview img
            aImg.src = e.target.result;
            // remove text
            var dropText = document.getElementById("dropText");
            if(dropText) dropText.remove();

            var formData = new FormData();
            formData.append('upload_file', entry);

            $.ajax({
              headers: {
                'Access-Control-Allow-Origin': 'http://localhost:5000',
                'Content-Type': undefined
              },
              url: 'http://localhost:5000/predict_from_file',
              data: formData,
              type: 'POST',
              contentType: false,
              processData: false,
              crossDomain: true,
              success: function(msg) {
              	console.log(msg)
              	var data = JSON.parse(msg)
              	data = JSON.parse(data.predictions)
              	console.log(data)
              	var myNode = document.getElementById("whatIsIt");
				while (myNode.firstChild) {
				    myNode.removeChild(myNode.firstChild);
				}
                for(var i = 0; i < data.length; i++) {
                	child = data[i];
                	var childNode = document.createElement("div");
                	document.getElementById("whatIsIt").appendChild(childNode);
                	childNode.innerText = child.name + " : " + child.prediction;
                }
              },
            })
          };
        })(img);
        reader.readAsDataURL(entry);
      }
    }
