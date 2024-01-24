import cv2
import numpy as np
import csv
import math
import os

from copy import deepcopy
import matplotlib.pyplot as plt













#####################################################
###################### FICHIERS #####################
#####################################################

def extractFramesOfVideo(path, interval=10):
    """
    Sort deux images à {interval}frames d'intervalle
    """
    capture = cv2.VideoCapture(path)

    matrice1 = []

    #timestamp = capture.get(cv2.CAP_PROP_POS_MSEC)
    capture.set(cv2.CAP_PROP_POS_FRAMES, 0)

    _, matrice1 = capture.read()

    capture.set(cv2.CAP_PROP_POS_FRAMES, interval)

    _, matrice2 = capture.read()

    capture.release()

    return matrice1, matrice2


def readImg(path):
    return cv2.imread(path)


def images2Video(folder_path, fps=2):
    """
    Converti toutes les images .png dans un dossier (DANS L ORDRE) en une video mp4
    """
    images_folder = folder_path

    image_files = sorted([os.path.join(images_folder, img) for img in os.listdir(images_folder) if img.endswith(".png")])
    print(image_files)

    # Lecture première image -> obtenir les dimensions
    first_image = cv2.imread(image_files[0])
    height, width, _ = first_image.shape


    output_video = cv2.VideoWriter(f"{folder_path}.mp4", cv2.VideoWriter_fourcc(*'mp4v'), fps, (width, height))

    # Ajout des images à la vidéo
    for image_path in image_files:
        img = cv2.imread(image_path)
        output_video.write(img)

    output_video.release()


def writeImage(tab, name):
    width, height = len(tab[0]), len(tab)
    image = np.zeros((height, width, 3), dtype=np.uint8)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[y, x]

    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée (writeImage 100% des points) ({width}x{height})")


def writeImage2(tab, name):
    width, height = len(tab[0])//2, len(tab)//2
    image = np.zeros((height, width, 3), dtype=np.uint8)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[2*y, 2*x]
    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée (writeImage2 50% des points) ({width}x{height})")















#####################################################
###################### VECTEURS #####################
#####################################################

def block2coordTopLeftCorner(block):
    x = (block*wblock)%wframe
    y = ((block*wblock)//wframe)*hblock
    return x, y


def blockAverageColor(b, img):
    x0, y0 = block2coordTopLeftCorner(b)
    med = [0, 0, 0]
    for i in range(wblock):
        for j in range(hblock):
            x = i+x0-1
            y = j+y0-1

            med[0] += int(img[y, x][0])
            med[1] += int(img[y, x][1])
            med[2] += int(img[y, x][2])

    size = wblock*hblock
    return [med[0]/size, med[1]/size, med[2]/size]


def getNeighs(b1):
    i, j = b1//nombre_blocks_x, b1%nombre_blocks_x
    neigh = []

    for cote in range(block_search_radius):

        for x in [-cote, cote]:
            for y in range(-cote, cote+1):

                k = (i+x) * nombre_blocks_x + j+y
                if k >= 0 and k < nombre_blocks and k%nombre_blocks_x >= j-block_search_radius and k%nombre_blocks_x <= j+block_search_radius and k not in neigh:
                    neigh.append(k)

        for x in range(-cote, cote+1):
            for y in [-cote, cote]:

                k = (i+x) * nombre_blocks_x + j+y
                if k >= 0 and k < nombre_blocks and k%nombre_blocks_x >= j-block_search_radius and k%nombre_blocks_x <= j+block_search_radius and k not in neigh:
                    neigh.append(k)
    return neigh

"""


    for a in range(-block_search_radius, block_search_radius+1):
        for b in range(-block_search_radius, block_search_radius+1):

            k = (i+a)*nombre_blocks_x + j+b

            x, y = k//nombre_blocks_x, k%nombre_blocks_x

            if( k >= 0 and k < nombre_blocks and y >= j-block_search_radius and y <= j+block_search_radius) :
                neigh.append(k)
"""         


diffs_calulees = []
def calculateBlockDifference(b1, img1, b2, img2):
    x_ref1, y_ref1 = block2coordTopLeftCorner(b1)
    x_ref2, y_ref2 = block2coordTopLeftCorner(b2)

    diff = [0, 0, 0]
    size = wblock*hblock

    for i in range(wblock//pas_calcul_difference):
        for j in range(hblock//pas_calcul_difference):
            x1 = i*pas_calcul_difference+x_ref1
            y1 = j*pas_calcul_difference+y_ref1

            x2 = i*pas_calcul_difference+x_ref2
            y2 = j*pas_calcul_difference+y_ref2

            diff[0] += pow( ( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) ), 2)
            diff[1] += pow( ( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) ), 2)
            diff[2] += pow( ( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) ), 2)

            #print(diff)

    diff = diff[0]/size + diff[1]/size + diff[2]/size
    #a, b, c = np.sqrt(diff)

    diffs_calulees.append(diff)
    return diff


def blockPlusRessemblant(b1, img1, img2, exclus):
    min_diff = math.inf
    b2 = -1

    for k in getNeighs(b1):
        if exclus[k] == 0:


            diff = calculateBlockDifference(b1, img1, k, img2)
            #print(f" {k} libre -- {diff}")

            if diff < diff_minimale:
                if b2 == -1:
                    b2 = k
                if diff < min_diff:
                    min_diff = diff
                    b2 = k
    return b2



def blockPlusRessemblant2(b1, img1, img2, exclus):

    for k in getNeighs(b1):
        if exclus[k] == 0:
            diff = calculateBlockDifference(b1, img1, k, img2)
            if diff < diff_minimale:
                return k

    return -1




def buildMotionVector(img1, img2):

    exclus = np.zeros(nombre_blocks, dtype=np.uint8)
    vecteurs = np.zeros(nombre_blocks)

    for k in range(nombre_blocks):
        if k%100 == 0:
            print(f"vecteurs bloc {k} calculés")

        conjugue = blockPlusRessemblant(k, img1, img2, exclus)

        """
        conjugue = k + nombre_blocks_x + 1 

        if ((k+1) % nombre_blocks_x) == 0:
            conjugue = -1

        if k//nombre_blocks_x == (nombre_blocks_y-1):
            conjugue = k

        """
        exclus[conjugue] = 1

        vecteurs[k] = conjugue

        """
        x_a, y_a = block2coordTopLeftCorner(k)
        x_b, y_b = block2coordTopLeftCorner(conjugue)
        
        dx = x_b - x_a
        dy = y_b - y_a


        #print(f"{k} --> {conjugue} //// {x_a, y_a} --> {x_a + dx, y_a + dy}")
        """

    return vecteurs





def buildIntermediateFrame(vectors, img_ref, path, nb_frame, display=False):
    if not os.path.exists(path):
        os.makedirs(path)


    frame = img_ref
    previous_frame = img_ref


    for t in range(nb_frame+1):

        # Fond blanc
        # frame = np.ones((hframe, wframe, 3), dtype=np.uint8) * 255

        for k in range(nombre_blocks):
            j, i = k//nombre_blocks_x, k%nombre_blocks_x

            x0, y0 = block2coordTopLeftCorner(k)

            if vectors[k] != -1:
                # Le bloc source possède un bloc image 

                j_prim, i_prim = vectors[k]//nombre_blocks_x, vectors[k]%nombre_blocks_x
                dx, dy = i_prim-i, j_prim-j


                block_a_remplir = (i + round(dx * t/nb_frame)) + (j + round(dy * t/nb_frame)) * nombre_blocks_x

                #print(f"bloc {k} conjugué au bloc {vectors[k]} -> mvnt(dx:{dx},dy:{dy}) donc sur frame {t} on rempli le bloc {block_a_remplir}")


                x1, y1 = block2coordTopLeftCorner(block_a_remplir)

                for i in range(wblock):
                    for j in range(hblock):
                        frame[y1 + j, x1 + i] = img_ref[y0 + j, x0 + i]

            else:
                # Le bloc n'a pas d'image
                # On prend (par transparence) le bloc a la meme place dans l'image precedente
                for i in range(wblock):
                    for j in range(hblock):
                        frame[y0 + j, x0 + i] = previous_frame[y0 + j, x0 + i]
                        #frame[y0 + j, x0 + i] = [255, 255, 255]


        if display:
            displayImageWithBlock(frame)

        writeImage(frame, f"{path}/f{t}")
        pass

    return





















if __name__ == "__main__":


    wframe = 1920 #1280
    hframe = 1080 #720
    wblock = 4*4
    hblock = 9*4

    #nombre_blocks = 1600
    nombre_blocks = int((wframe/wblock) * (hframe/hblock))
    nombre_blocks_x = int((wframe/wblock))
    nombre_blocks_y = int((hframe/hblock))

    print(f"Nombre de block: {nombre_blocks} ({nombre_blocks_x}x{nombre_blocks_y})")


    block_search_radius = round(wframe*0.15)//wblock
    #block_search_radius = 4
    print(f"block_search_radius={block_search_radius}")


    font_thickness = 1
    displayBlockNumbers = True
    if nombre_blocks < 2500:
        displayBlockNumbers = True




    pas_calcul_difference = 4
    diff_minimale = 4000


    print(f"{nombre_blocks * (nombre_blocks / block_search_radius) * (1 / pas_calcul_difference) * wblock * hblock:.0f} calculs")






    name_test = "square4"

    #img1, img2 = extractFramesOfVideo("in/sea_shore.mp4", 10)
    img1 = readImg("in/square1.png")
    img2 = readImg("in/square2.png")
    displayImageWithBlock(img1)
    displayImageWithBlock(img2)


    #print(blockPlusRessemblant(431, img1, img2, np.zeros(nombre_blocks)))
    #displayImageWithNeighs(img1, 431)
    #displayImageWithBlock(img2)


    vecteurs = buildMotionVector(img1, img2)

    for k in range(nombre_blocks):
        pass
        print(f"{k} --> {vecteurs[k]}")

    displayVectorMap(vecteurs, img1, True, "vectors.png")
    displayVectorMap(vecteurs, img2)

    show_diffs_calulees(diffs_calulees)

    buildIntermediateFrame(vecteurs, img1, f"out/{name_test}", 10, False)
    images2Video(f"out/{name_test}")
