pooling <- function(m){
	t = c()
	for(i in seq(1,dim(m)[1],2)){
		for(j in seq(1,dim(m)[1],2)){
			t = c(t,max(max(m[i:(i+1),j:(j+1)])))
		}
	}
	return(matrix(t,nrow=sqrt(length(t)),byrow=TRUE))

}

img = readPNG("/tmp/cat.png");
img = resize(img,h=100,w=100);
img = img[,,1]*0.7 + img[,,2]*0.23 + img[,,3]*0.07

writePNG(img, "/tmp/img.png")

img_conv1 = applyFilter(img, convKernel(k="gaussian",sigma=1))
img_conv1 = applyFilter(img_conv1, convKernel(k="sobel"))
img_conv2 = applyFilter(img, convKernel(k="gaussian",sigma=5))
img_conv2 = applyFilter(img_conv2, convKernel(k="sobel"))

img_conv3 = applyFilter(img, convKernel(k="gaussian",sigma=1))
img_conv3 = applyFilter(img_conv3, convKernel(k="emboss"))
img_conv4 = applyFilter(img, convKernel(k="gaussian",sigma=2))
img_conv4 = applyFilter(img_conv4, convKernel(k="emboss"))

img_conv1[img_conv1>1] = 1
img_conv1[img_conv1<0] = 0
img_conv2[img_conv2>1] = 1
img_conv2[img_conv2<0] = 0
img_conv3[img_conv3>1] = 1
img_conv3[img_conv3<0] = 0
img_conv4[img_conv4>1] = 1
img_conv4[img_conv4<0] = 0

writePNG(img_conv1, "/tmp/img_conv1.png")
writePNG(img_conv2, "/tmp/img_conv2.png")
writePNG(img_conv3, "/tmp/img_conv3.png")
writePNG(img_conv4, "/tmp/img_conv4.png")

img_conv1_pool = pooling(img_conv1)
img_conv2_pool = pooling(img_conv2)
img_conv3_pool = pooling(img_conv3)
img_conv4_pool = pooling(img_conv4)

writePNG(img_conv1_pool, "/tmp/img_conv1_pool.png")
writePNG(img_conv2_pool, "/tmp/img_conv2_pool.png")
writePNG(img_conv3_pool, "/tmp/img_conv3_pool.png")
writePNG(img_conv4_pool, "/tmp/img_conv4_pool.png")

img_conv12 = applyFilter(img_conv1_pool, convKernel(k="gaussian",sigma=1))
img_conv12 = applyFilter(img_conv12, convKernel(k="sobel"))
img_conv22 = applyFilter(img_conv2_pool, convKernel(k="gaussian",sigma=5))
img_conv22 = applyFilter(img_conv22, convKernel(k="sobel"))

img_conv32 = applyFilter(img_conv3_pool, convKernel(k="gaussian",sigma=1))
img_conv32 = applyFilter(img_conv32, convKernel(k="emboss"))
img_conv42 = applyFilter(img_conv4_pool, convKernel(k="gaussian",sigma=5))
img_conv42 = applyFilter(img_conv42, convKernel(k="emboss"))

img_conv12[img_conv12>1] = 1
img_conv12[img_conv12<0] = 0
img_conv22[img_conv22>1] = 1
img_conv22[img_conv22<0] = 0
img_conv32[img_conv32>1] = 1
img_conv32[img_conv32<0] = 0
img_conv42[img_conv42>1] = 1
img_conv42[img_conv42<0] = 0

img_conv12 = pooling(img_conv12)
img_conv22 = pooling(img_conv22)
img_conv32 = pooling(img_conv32)
img_conv42 = pooling(img_conv42)

writePNG(img_conv12, "/tmp/img_conv12.png")
writePNG(img_conv22, "/tmp/img_conv22.png")
writePNG(img_conv32, "/tmp/img_conv32.png")
writePNG(img_conv42, "/tmp/img_conv42.png")

