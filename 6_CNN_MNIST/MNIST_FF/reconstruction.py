from PIL import Image
import numpy as np
import pickle


# def Gauss(mx,mr,n=100):
#     if len(mx) == len(mr):  #若mx和mr长度相等则开始迭代 否则方程无解
#         x = [] #迭代初值 初始化为单行全0矩阵
#         for i in range(len(mr)):
#             print(mr)
#             x.append([0])
#         count = 0 #迭代次数计数
#         while count < n:
#             for i in range(len(x)):
#                 nxi = mr[i][0]
#                 for j in range(len(mx[i])):
#                     if j!=i:
#                         nxi = nxi+(-mx[i][j])*x[j][0]
#                 nxi = nxi/mx[i][i]
#                 x[i][0] = nxi
#             count = count + 1
#         return x
#     else:
#         return False


def main():
    # load data
    fr = open('pca_params_compact.pkl', 'rb')
    pca_params = pickle.load(fr)
    fr.close()

    fr = open('feat_compact.pkl', 'rb')
    feat = pickle.load(fr)
    fr.close()
    feature = feat['feature']

    print ('feature shape: ', feature.shape)
    sample_images=feature.reshape(4, 16, 2, 2)



    num_layers = pca_params['num_layers'] # 2
    kernel_sizes = pca_params['kernel_size'] # 4, 4

    for i in range(num_layers-1,-1,-1):
        print('--------stage %d --------' % i)
        # Extract parameters
        feature_expectation = pca_params['Layer_%d/feature_expectation' % i].astype(np.float32)
        kernels = pca_params['Layer_%d/kernel' % i].astype(np.float32)
        mean = pca_params['Layer_%d/pca_mean' % i]


        print('sample_images: ',sample_images.shape)
        sample_images = np.moveaxis(sample_images,1,3)

        transformed = sample_images.reshape(-1,sample_images.shape[-1])

        print('transformed: ',transformed.shape)
        print('kernel: ',kernels.shape)

        if i == 0:
            #print('transformed: ',transformed[:,0:2])
            #print('kernel: ',kernels[:,:])
            # inver = np.dot(np.transpose(kernels),kernels)
            # inver = np.linalg.inv(inver)
            # inver1 = np.matmul(np.transpose(kernels),kernels)
            # print('iiiiiii: ', inver1)
            # inver2 = np.linalg.pinv(inver1)
            # print('iiiiiii: ', inver2)
            # inver3 = np.dot(inver1,inver2)
            #print('iiiiiii: ', inver3)
            sample_patches_centered = np.dot(transformed, kernels)
            #print ('test2: ', kernels[:,:])
            #sample_patches_centered= np.dot(sample_patches_centered, inver)
            #sample_patches_centered += mean

        else:
            bias = pca_params['Layer_%d/bias' % i].astype(np.float32)
            e = np.zeros((1,kernels.shape[0]),dtype=np.float32)
            print('bias: ',bias)
            e[0,0] = 1

            transformed += bias * e

            print('transformed: ',transformed)

            # inver1 = np.matmul(np.transpose(kernels),kernels)
            # #print('iiiiiii: ', inver1)
            # inver2 = np.linalg.inv(inver1)
            # #print('iiiiiii: ', inver2    )
            # inver3 = np.dot(inver1,inver2)
            # #print('iiiiiii: ', inver3)

            # a = np.array([[1,2,3],[2,1,4],[3,4,1]])
            # b = np.linalg.inv(a)
            # print(np.dot(a,b))

            # mx = [[8,-3,2],[4,11,-1],[6,3,12]]
            # mr = [[20],[33],[36]]
            # print(Gauss(mx,mr,20))

            kernel = kernels[2:,:]
            transformed_w = transformed[:,2:]
            sample_patches_centered_w_bias = np.matmul(transformed_w, kernel)
            #sample_patches_centered_w_bias += mean
            #sample_patches_centered_w_bias = np.matmul(sample_patches_centered_w_bias, inver2)
            #print ('test mean: ', mean)

            sample_patches_centered_w_bias += mean
            #print('mean???: ', mean)
            sample_patches_centered = sample_patches_centered_w_bias - 1 / np.sqrt(96) * bias

        print('sample_patches_centered: ',sample_patches_centered.shape) # 16*96
        #print('sample_patches_centered: ',sample_patches_centered.shape)

        sample_patches = sample_patches_centered + feature_expectation # 16*96,  256 *16
        print ('test2: ', sample_patches)


        last_size = np.sqrt(sample_patches.shape[0]/4) #2, 8
        last_size = last_size.astype(np.int64)
        sample_patches = sample_patches.reshape([4,last_size,last_size, sample_patches.shape[-1]]) #4 2 2 96, 4 8 8 16
        print ('sample_patches1: {}'.format(sample_patches.shape))

        last_dim = (int)(sample_patches.shape[-1]/kernel_sizes[i]/kernel_sizes[i]) #6, 1
        result = np.zeros((4,last_dim,last_size*kernel_sizes[i],last_size*kernel_sizes[i])) # 4 6 8 8
        # sample_patches = sample_patches.reshape([4, last_size*kernel_sizes[i],last_size*kernel_sizes[i],last_dim])
        for m in range(4):
            for n in range(last_dim):
                for p in range(last_size*kernel_sizes[i]):
                    for q in range(last_size*kernel_sizes[i]):
                        result[m,n,p,q] = sample_patches[m,p//4,q//4,16*n*i+(p%4)*4+(q%4)]
        sample_images = result
        #print('sample_image_last: ', sample_images)

    # print(sample_images[0,:,:,0] * 255)
    # print(sample_images[1,:,:,0] * 255)
    # print(sample_images[2,:,:,0] * 255)
    # print(sample_images[3,:,:,0] * 255)

    img = Image.fromarray(np.uint8(sample_images[0,0,:,:] * 255) , 'L')
    #img.show()
    img = Image.fromarray(np.uint8(sample_images[1,0,:,:] * 255) , 'L')
    #img.show()
    img = Image.fromarray(np.uint8(sample_images[2,0,:,:] * 255) , 'L')
    #img.show()
    img = Image.fromarray(np.uint8(sample_images[3,0,:,:] * 255) , 'L')
    #img.show()


if __name__ == '__main__':
    main()
