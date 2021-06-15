# Bi-level Image Labeling (connected-component labeling)

## Introduction
Image labeling is used for object detection, semantic segmentation, and image classification/extraction.

For example:

![image](https://user-images.githubusercontent.com/28287818/122023484-2fa82980-cdfa-11eb-8a2a-678420329aff.png)

But this program is simple. 

It performs labeling in bi-level images (Before => After)

![image](https://user-images.githubusercontent.com/28287818/122023949-96c5de00-cdfa-11eb-8f85-39af516fbc69.png)
![image](https://user-images.githubusercontent.com/28287818/122023960-99c0ce80-cdfa-11eb-98e8-99b560d6d580.png)


## Program

This program is console-based. To use this, you have to type ```label <image-file.pbm>.```
If you don't trust my .exe file (you should be!), you can use my code and compile it on your own.

This program will only accept images in PBM format. PBM image format is a bi-level format (documentation [here](http://netpbm.sourceforge.net/doc/pbm.html)) where every bit of a byte is a pixel. To generate your own PBM format, you can use Photoshop or GIMP.

abc.pbm:

![image](https://user-images.githubusercontent.com/28287818/122030478-62edb700-ce00-11eb-9180-46bbdb1b47d0.png)

Example 1: label-philo abc.pbm

![image](https://user-images.githubusercontent.com/28287818/122030604-80bb1c00-ce00-11eb-9c43-d6ed16f086a6.png)

Example 2: label-philo car.pbm

![image](https://user-images.githubusercontent.com/28287818/122030925-c841a800-ce00-11eb-9339-c7b6c820704a.png)

Example 3: label-philo halloween.pbm

![image](https://user-images.githubusercontent.com/28287818/122031019-df809580-ce00-11eb-9ff4-67c243fb9bd9.png)

I uploaded different kinds of pbm files for you to try.

## Algorithm
The algorithm I used here is Depth-First Search (DFS) and callstack as my stack (recursion). Since the maximum dimension of the image is 60x60, I think recusion using callstack is not that bad. It is still fast and the code looks sleek.

There are other ways of approach to code like two-pass algorithm (also known as Hoshen–Kopelman algorithm). It is also fast and generally better if you want to increase the maximum dimension. 



## Reference
Connected-component labeling: https://en.wikipedia.org/wiki/Connected-component_labeling

Program Specification: [2018YearEndProgramingContest.pdf](https://github.com/pkdimpas/Image-Labeling-connected-component-labeling-/files/6654375/2018YearEndProgramingContest.pdf)
