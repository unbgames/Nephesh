import cv2
import numpy as np
import glob
import os
from os import path



def main():
    scale = 2
    newfolder = "assets"
    for filename in glob.iglob('original_images/**', recursive=True):
        img = cv2.imread(filename, cv2.IMREAD_UNCHANGED)
        if img is not None:
            height, width = img.shape[:2]
            img2 = cv2.resize(img, (scale*width, scale*height), interpolation=cv2.INTER_NEAREST)

            if os.name == 'nt':
                splited = filename.split('\\')
                splited[0] = newfolder
                dirPath = splited[:-1]
                dirPath = '\\'.join(dirPath) + '\\'
                os.makedirs(dirPath, exist_ok=True)
                newPath = '\\'.join(splited)
            else:
                splited = filename.split('/')
                splited[0] = newfolder
                dirPath = splited[:-1]
                dirPath = '/'.join(dirPath) + '/'
                os.makedirs(dirPath, exist_ok=True)
                newPath = '/'.join(splited)
            print(newPath)
            cv2.imwrite(newPath, img2)

            # cv2.imshow('image' ,img2)
            # key = cv2.waitKey(0)
            #     #if the 'q' key is pressed, stop the loop
            # if key & 0xFF == ord("q"):
            #     break

            # cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
