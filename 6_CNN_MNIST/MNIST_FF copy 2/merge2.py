from tensorflow.python.platform import flags
import pickle
import data
import saab_compact as saab
import numpy as np
import keras
import sklearn
from matplotlib.image import imread
from PIL import Image
import math

flags.DEFINE_string("output_path", "/Users/jamieg/Downloads/Interpretable_CNN-master/MNIST_FF/Parameters", "The output dir to save params")
flags.DEFINE_string("use_classes", "0-9", "Supported format: 0,1,5-9")
flags.DEFINE_string("kernel_sizes", "4,4", "Kernels size for each stage. Format: '3,3'")
flags.DEFINE_string("num_kernels", "5,50", "Num of kernels for each stage. Format: '4,10'")
flags.DEFINE_float("energy_percent", None, "Energy to be preserved in each stage")
flags.DEFINE_integer("use_num_images", 10000, "Num of images used for training")
FLAGS = flags.FLAGS


def main():
    # read data
    train_images, train_labels, test_images, test_labels, class_list = data.import_data(FLAGS.use_classes)
    print('Training image size:', train_images.shape)
    print('Testing_image size:', test_images.shape)
    print ('Training images.dtype ', train_images.dtype)

    kernel_sizes = saab.parse_list_string(FLAGS.kernel_sizes)
    if FLAGS.num_kernels:
        num_kernels = saab.parse_list_string(FLAGS.num_kernels)
    else:
        num_kernels = None
    energy_percent = FLAGS.energy_percent
    use_num_images = FLAGS.use_num_images
    print('Parameters:')
    print('use_classes:', class_list)
    print('Kernel_sizes:', kernel_sizes)
    print('Number_kernels:', num_kernels)
    print('Energy_percent:', energy_percent)
    print('Number_use_images:', use_num_images)
    train_images=np.moveaxis(train_images, 3, 1)


    # multi_Saab_transform process
    num_total_images = train_images.shape[0]
    if use_num_images < num_total_images and use_num_images > 0:
        sample_images, selected_labels = saab.select_balanced_subset(train_images, train_labels, use_num_images, class_list)
    else:
        sample_images = train_images

    num_samples = sample_images.shape[0]
    num_layers = len(kernel_sizes)
    pca_params = {}
    pca_params['num_layers'] = num_layers
    pca_params['kernel_size'] = kernel_sizes
    pca = []
    means = []
    k = []
    mu = []
    for i in range(num_layers):
        print('--------stage %d --------' % i)
        print(kernel_sizes)
        sample_patches = saab.window_process2(sample_images, kernel_sizes[i], kernel_sizes[i])  # overlapping
        h = sample_patches.shape[1]
        w = sample_patches.shape[2]
        # Flatten
        sample_patches = sample_patches.reshape([-1, sample_patches.shape[-1]])

        # Remove patch mean
        training_data = sample_patches
        print ('training_data.shape: {}'.format(training_data.shape)) #(640000, 16)

        # Compute PCA kernel
        if not num_kernels is None:
            num_kernel = num_kernels[i]
        kernels, mean, pcaa = saab.find_kernels_pca(training_data, num_kernel, energy_percent)
        k.append(kernels)
        pca.append(pcaa)
        means.append(mean)
        num_channels = sample_patches.shape[1]

        if i == 0:
            # Transform to get data for the next stage
            transformed = pca[0].transform(sample_patches)
        else:
            # Transform to get data for the next stage
            transformed = pca[1].transform(sample_patches)

        # Reshape: place back as a 4-D feature map
        sample_images = transformed.reshape(num_samples, h, w, -1)
        sample_images = np.moveaxis(sample_images, 3,1)
        print('sample_images :', sample_images.shape)
    # end

    fw = open('pca_params_compact.pkl', 'wb')
    pickle.dump(pca_params, fw)
    fw.close()

    # print(pca_params)







    im1 = imread('1.png')
    im2 = imread('2.png')
    im3 = imread('3.png')
    im4 = imread('4.png')

    train_images = np.zeros((4,32,32,1))
    train_images[0,:,:,0] = im1
    train_images[1,:,:,0] = im2
    train_images[2,:,:,0] = im3
    train_images[3,:,:,0] = im4
    #train_images=train_images/255.
    train_images_ori=train_images
    train_images=np.float32(train_images)
    # Training
    print('--------Training--------')

    train_images=np.moveaxis(train_images, 3, 1)  #####!!!!!!!!!

    num_layers = pca_params['num_layers'] #2
    kernel_sizes = pca_params['kernel_size'] #4

    # initialize
    for i in range(num_layers):
        print('--------stage %d --------' % i)
        # Extract parameters
        #feature_expectation = pca_params['Layer_%d/feature_expectation' % i].astype(np.float32)
        #kernels = pca_params['Layer_%d/kernel' % i].astype(np.float32)
        #k
        # mean = pca_params['Layer_%d/pca_mean' % i].astype(np.float32)
        print( 'sample images shape: {}'.format(train_images.shape))
        # Create patches
        # print 'sample_images {}: {}', sample_images.shape, sample_images[-2:]
        if i==0:
            # print('test%d' % i, train_images[:,:,14:18,:])
            # print('test%d shape' % i, train_images.shape)
            sample_patches = saab.window_process2(train_images, kernel_sizes[i], kernel_sizes[i])  # overlapping
            #print( 'PPPPa0: ', sample_patches[1,0:2,:,:])
            print ('sample_patches1: {}'.format(sample_patches.shape))
        elif i==1:

            sample_patches=saab.window_process2(train_images, kernel_sizes[i], kernel_sizes[i])
            print ('sample_patches1: {}'.format(sample_patches.shape))

        h = sample_patches.shape[1] #first time = 8, 2
        w = sample_patches.shape[2] # 8, 2
        # Flatten
        # (n*output_h*output_w, c*kernel_size*kernel_size)

        sample_patches = sample_patches.reshape([-1, sample_patches.shape[-1]])
        print ('sample_patches2: {}'.format(sample_patches.shape))



        # Remove feature mean (Set E(X)=0 for each dimension)
        # feature after removing mean, mean
        # sample_patches_centered, feature_expectation = remove_mean(sample_patches, axis=0)
        #######sample_patches_centered = sample_patches - feature_expectation
        # sample_patches_centered = sample_patches

         # sample_patches_centered=sample_patches-feature_expectation

        # Remove patch mean
        # training_data, dc = remove_mean(sample_patches_centered, axis=1)

        # num_channels = sample_patches.shape[-1]
        if i == 0:
            # Transform to get data for the next stage
            # print 'train data:{}'.format(sample_patches_centered[:5])
            # print 'sample shape: {}'.format(sample_patches_centered.shape)
            # print 'data shape: {}'.format(sample_patches_centered.shape)
            # print 'kernels: {}, shape:{}'.format(kernels, kernels.shape)
            # print 'kernel: {} /{}'.format(kernels, kernels.dtype)
            #####transformed = np.matmul(sample_patches_centered, np.transpose(kernels))
            transformed = pca[0].transform(sample_patches)
            mu.append(np.mean(sample_patches, axis=0))
            #print('TTTTT: ', transformed[10:30,:])
            # temp=transformed.reshape(10000, 28, 28, 6)
            # temp=np.moveaxis(temp, 3, 1)
            # print 'layer conv1 output: {}, shape: {}'.format( temp[:1].shape, temp[:1])
        # elif i==1:
        #     transformed= np.matmul(sample_patches_centered, np.transpose(kernels))
        #     temp=transformed.reshape(10000, 10, 10, 16)
        #     temp=np.moveaxis(temp, 3,1)
        #     print 'conv2  {},output: {}'.format(temp[:1].shape, temp[:1])
        else:
            # weight=(kernels)
            # print 'conv2 temp_bias: ', np.matmul(-1*feature_expectation, weight)
            # print 'conv2 kernel: {}, shape: {}'.format(kernels, kernels.shape)
            # print 'sample_patches_centered: ',sample_patches_centered.shape
            ##bias = pca_params['Layer_%d/bias' % i].astype(np.float32)
            # Add bias
            # print 'num_channels: {}'.format(num_channels)
            ##sample_patches_centered_w_bias = sample_patches_centered + 1 / np.sqrt(96) * bias
            # bias1_tmp=np.zeros(150)+1/np.sqrt(num_channels)* bias
            # print 'conv2 bias1: ', np.matmul(bias1_tmp, weight)
            # Transform to get data for the next stage
            #####print ('test: ', sample_patches_centered_w_bias)
            #print ('test2: ', kernels)
            #transformed = np.matmul(sample_patches_centered_w_bias, np.transpose(kernels))

            #transformed = pca[i].transform(sample_patches_centered_w_bias)
            #print('test%d' % i, sample_patches)
            transformed = pca[1].transform(sample_patches)

            mu.append(np.mean(sample_patches, axis=0))

            # Remove bias
            # e = np.zeros((1, kernels.shape[0]-1),dtype=np.float32)
            # e[0, 0] = 1
            #
            # transformed -= bias * e # 16*16
            # print 'conv2 bias2: ', -bias*e
            # print 'bias: {}'.format(bias)
            # temp=transformed.reshape(10000, 10, 10, 16)
            # temp=np.moveaxis(temp, 3, 1)
            # print 'conv2_2 shape {}, output: {}'.format(temp[:1].shape, temp[:1])

        # Reshape: place back as a 4-D feature map
        num_samples = train_images.shape[0]
        # print('test%d' % i, transformed)
        train_images = transformed.reshape(num_samples, h, w, -1)
        # print('train_images', train_images)
        train_images=np.moveaxis(train_images, 3, 1)
        # sample_images = transformed.reshape(num_samples, -1, h, w)
        # print 'sample_images1.shape: {}'.format(sample_images.shape)

        # Maxpooling
    # end
    ##feature = saab.initialize(train_images, pca_params, pca)
    feature = train_images

    feature = feature.reshape(feature.shape[0], -1)
    ##print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    print ('feature.dtype: {}'.format(feature.dtype))
    feat = {}
    feat['feature'] = feature

    # save data
    fw = open('feat_compact.pkl', 'wb')
    pickle.dump(feat, fw)
    fw.close()

    ############################################################################

    inver_input = train_images
    for i in range(num_layers-1,-1,-1):
        print('--------inv  stage %d --------' % i)
        train_images=np.moveaxis(train_images, 1, 3)
        # print('train_images', train_images)
        transformed = train_images.reshape(-1,train_images.shape[-1])
        # print('test%d' % i, transformed)
        print('transformed: ', transformed.shape)



        if i == 0:
            #transformed_inv = np.dot(transformed, pca[0].components_[:num_kernels[0],:])
            transformed_inv = pca[0].inverse_transform(transformed)
            #transformed_inv += mu[0]
        else:
            #transformed_inv = np.dot(transformed, pca[1].components_[:num_kernels[1],:])
            transformed_inv = pca[1].inverse_transform(transformed)
            print('test%d' % i, transformed_inv)
            print('transformed_inv: ', transformed_inv.shape) # 4, 80
            #transformed_inv += mu[1]



        last_size = np.sqrt(transformed_inv.shape[0]/4) #
        print('last_size: ', last_size)
        last_size = last_size.astype(np.int64)
        sample_patches = transformed_inv.reshape([4,last_size,last_size, transformed_inv.shape[-1]]) #1 2 2
        # print('test%d' % i, sample_patches)
        # print('sample_patches: ', sample_patches.shape)
        last_dim = (int)(sample_patches.shape[-1]/kernel_sizes[i]/kernel_sizes[i]) #
        print('last_dim: ', last_dim)
        result = np.zeros((4,last_dim,last_size*kernel_sizes[i],last_size*kernel_sizes[i])) # 4 6 8 8
        print('result ', result.shape)
        # sample_patches = sample_patches.reshape([4, last_size*kernel_sizes[i],last_size*kernel_sizes[i],last_dim])
        for m in range(4):
            for n in range(last_dim):
                for p in range(last_size*kernel_sizes[i]):
                    for q in range(last_size*kernel_sizes[i]):
                        result[m,n,p,q] = sample_patches[m,p//4,q//4,16*n*i+(p%4)*4+(q%4)]
        train_images = result
        # print('test%d' % i, train_images[:,:,14:18,:])
        # print('test%d shape' % i, train_images.shape)

    train_images=np.moveaxis(train_images, 1, 3)
    # print('test' , train_images[:,14:18,:,:])
    # print('test shape' , train_images.shape)


    # PSNR
    psnr4 = []
    for m in range(4):
        MSE = 0
        for n in range(32):
            for i in range(32):
                MSE += (train_images[m,n,i,0]-train_images_ori[m,n,i,0])**2
        MSE = MSE/1024.
        # temp = train_images_ori[m,:,:,:]
        # temp = temp.reshape(1024,1)
        # MAX = max(temp)
        MAX = 1
        psnr4.append(round(10*math.log10(MAX*MAX/MSE),2))
    print('psnr: ',psnr4)




    train_images=train_images*255
    for m in range(4):
        for i in range(32):
            for j in range(32):
                if train_images[m,i,j,0]>255:
                    train_images[m,i,j,0]=255
                elif train_images[m,i,j,0]<0:
                    train_images[m,i,j,0]=0
    img = Image.fromarray(np.uint8(train_images[0,:,:,0]) , 'L')
    #img.show()
    img.save("1_%d_%d_%d.jpeg" % (num_kernels[0],num_kernels[1],round(psnr4[0]*100)))
    img2 = Image.fromarray(np.uint8(train_images[1,:,:,0]) , 'L')
    #img2.show()
    img2.save("2_%d_%d_%d.jpeg" % (num_kernels[0],num_kernels[1],round(psnr4[1]*100)))
    img3 = Image.fromarray(np.uint8(train_images[2,:,:,0]) , 'L')
    #img3.show()
    img3.save("3_%d_%d_%d.jpeg" % (num_kernels[0],num_kernels[1],round(psnr4[2]*100)))
    img4 = Image.fromarray(np.uint8(train_images[3,:,:,0]) , 'L')
    #img4.show()
    img4.save("4_%d_%d_%d.jpeg" % (num_kernels[0],num_kernels[1],round(psnr4[3]*100)))


if __name__ == '__main__':
    main()
