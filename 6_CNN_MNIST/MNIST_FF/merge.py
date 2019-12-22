from tensorflow.python.platform import flags
import pickle
import data
import saab_compact as saab
import numpy as np
import keras
import sklearn
from matplotlib.image import imread

flags.DEFINE_string("output_path", "/Users/jamieg/Downloads/Interpretable_CNN-master/MNIST_FF/Parameters", "The output dir to save params")
flags.DEFINE_string("use_classes", "0-9", "Supported format: 0,1,5-9")
flags.DEFINE_string("kernel_sizes", "4,4", "Kernels size for each stage. Format: '3,3'")
flags.DEFINE_string("num_kernels", "5,15", "Num of kernels for each stage. Format: '4,10'")
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
    pca_params, pca = saab.multi_Saab_transform(train_images, train_labels,
                                           kernel_sizes=kernel_sizes,
                                           num_kernels=num_kernels,
                                           energy_percent=energy_percent,
                                           use_num_images=use_num_images,
                                           use_classes=class_list)
    # save data
    fw = open('pca_params_compact.pkl', 'wb')
    pickle.dump(pca_params, fw)
    fw.close()

    print(pca_params)









    fr = open('pca_params_compact.pkl', 'rb')
    pca_params = pickle.load(fr)
    fr.close()

    # read data
    # train_images, train_labels, test_images, test_labels, class_list = data.import_data("0-9")
    # print('Training image size: dtype: ', train_images.shape, train_images.dtype)
    # print('Testing_image size:', test_images.shape)
    im1 = imread('1.png')
    im2 = imread('2.png')
    im3 = imread('3.png')
    im4 = imread('4.png')

    train_images = np.zeros((4,32,32,1))
    train_images[0,:,:,0] = im1
    train_images[1,:,:,0] = im2
    train_images[2,:,:,0] = im3
    train_images[3,:,:,0] = im4
    train_images=train_images/255.
    train_images=np.float32(train_images)
    # Training
    print('--------Training--------')
    #print("IIIIIII:", train_images[1,0:16,0:16,:])
    train_images=np.moveaxis(train_images, 3, 1)
    #print("IIIIII2:", train_images[1,:,0:16,:])
    feature = saab.initialize(train_images, pca_params, pca)
    # 60000x400 (16*5*5)
    feature = feature.reshape(feature.shape[0], -1)
    print("S4 shape:", feature.shape)
    print('--------Finish Feature Extraction subnet--------')
    print ('feature.dtype: {}'.format(feature.dtype))
    feat = {}
    feat['feature'] = feature

    # save data
    fw = open('feat_compact.pkl', 'wb')
    pickle.dump(feat, fw)
    fw.close()














if __name__ == '__main__':
    main()
