import pickle
import numpy as np
import data
import saab_compact as saab
import keras
import sklearn
from matplotlib.image import imread

def main():
    # load data
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
    feature = saab.initialize(train_images, pca_params)
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
