import libCBS
import numpy as np
import cv2 as cv

print("Currently 3 DAO(a popular video game) Maps are available")
print("Enter which map to convert to compatible formats for testing(1-3):")
map_id = int(input())
libCBS.convert_Map(map_id)
print("Do you want to view the converted map?[Y/n]:")
resp = str(input())
if(resp.upper() == 'Y' or resp == ''):
    data = np.load("assets/map"+str(map_id)+".npz")
    cv.imshow("Map Image", data["map_image"])
    cv.waitKey(0)
    cv.destroyAllWindows()
