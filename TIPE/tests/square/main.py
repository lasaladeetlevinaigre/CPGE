import cv2
import numpy as np
import csv


def main():
    # Ouvrir la vidéo à partir d'un fichier
    capture = cv2.VideoCapture('crossing.mp4')

    if not capture.isOpened():
        print("Erreur lors de l'ouverture de la vidéo.")
        return -1

    frame_number = 0

    matrices = []

    while frame_number < 5:

        print(f"Lecture image {frame_number}")
        timestamp = capture.get(cv2.CAP_PROP_POS_MSEC)
        print(f"Timestamp de la frame : {timestamp} ms")

        capture.set(cv2.CAP_PROP_POS_FRAMES, frame_number*20+10) #10micromana


        ret, frame = capture.read()

        if not ret:
            break


        matrices.append(frame)
        #writeImage(frame, f"frame{frame_number}")

        frame_number += 1


    capture.release()

    for i in range(len(matrices)):
        writeImage(matrices[i], f"frame{i}")
        #tableau_to_csv(matrices[i], f"frame{i}.csv")
        pass








def writeImage(tab, name):
    width, height = len(tab[0]), len(tab)
    image = np.zeros((height, width, 3), dtype=np.uint8)  # Créer une image noire (fond)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[y, x]
    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée")


def writeImage2(tab, name):
    width, height = len(tab[0])//2, len(tab)//2
    image = np.zeros((height, width, 3), dtype=np.uint8)  # Créer une image noire (fond)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[2*y, 2*x]
    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée")







def tableau_to_csv(tableau_pixels, nom_fichier_csv):
    with open(nom_fichier_csv, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        for row in tableau_pixels:
            writer.writerow(row)

def csv_to_tableau(nom_fichier_csv):
    tableau_pixels = []
    with open(nom_fichier_csv, 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            tableau_pixels.append([int(x) for x in row])
    return np.array(tableau_pixels, dtype=np.uint8)




#####################################################
############ DIVISION PAR BLOCK #####################
#####################################################


wframe = 1920
hframe = 1020
wblock = 192
hblock = 102

def block2coordTopLeftCorner(block):
    x = (block['id']*wblock)%wframe
    y = ((block['id']*wblock)//wframe)*hblock
    #print(f"bloc TLC: x:{x}, y:{y}")
    return x, y


def calculateBlockMediumColor(b, img):
    x0, y0 = block2coordTopLeftCorner(b)

    sr, sg, sb = 0, 0, 0
    k = 0
    for i in range(wblock):
        for j in range(hblock):
            r, g, b = img[j+y0, i+x0]
            sr += r
            sg += g
            sb += b
            k += 1
    return(sr/k, sg/k, sb/k)


def calculateBlockDifference(b1, img1, b2, img2):
    diff = 0
    xtl1, ytl1 = block2coordTopLeftCorner(b1)
    xtl2, ytl2 = block2coordTopLeftCorner(b2)

    for i in range(wblock//2-1):
        for j in range(hblock//2-1):
            x1 = i*2+xtl1
            y1 = j*2+ytl1

            x2 = i*2+xtl2
            y2 = i*2+ytl2

            #print(x1, y1, x2, y2)
            diff += abs(img1[y1, x1][0] - img2[y2, x2][0])
            diff += abs(img1[y1, x1][1] - img2[y2, x2][1])
            diff += abs(img1[y1, x1][2] - img2[y2, x2][2])
    return diff


def displayBlock(b, img):
    x0, y0 = block2coordTopLeftCorner(b)

    tab = np.zeros((hblock, wblock, 3), dtype=np.uint8)
    for i in range(wblock):
        for j in range(hblock):
            tab[j, i] = img[j+y0, i+x0]

    cv2.imshow(f"bloc {b['id']}", tab)
    cv2.waitKey(0)  # Attendre indéfiniment jusqu'à ce qu'une touche soit enfoncée
    cv2.destroyAllWindows()






if __name__ == "__main__":
    # main()
    img1 = cv2.imread("test/square1.png")
    #print(img1)
    img2 = cv2.imread("test/square2.png")
    b1 = {'id': 0}

    mini = 0
    for k in range(99):
        diff = calculateBlockDifference({'id': 12}, img1, {'id': k}, img2)
        if diff < mini:
            print(diff)
            mini = diff
        #displayBlock({'id': x}, img1)
        #print(calculateBlockMediumColor({'id': x}, img1))
