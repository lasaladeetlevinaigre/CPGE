import cv2
import numpy as np
import csv
import math
import os
from copy import deepcopy
import matplotlib.pyplot as plt
from scipy.fftpack import dct, idct



class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


















#####################################################
##################### AFFICHAGE #####################
#####################################################

def displayBlock(b, img):
    x0, y0 = block2coordTopLeftCorner(b)

    tab = np.zeros((hblock, wblock, 3), dtype=np.uint8)
    for i in range(wblock):
        for j in range(hblock):
            tab[j, i] = img[j+y0, i+x0]

    #displayTabPixels(tab)
    plt.imshow(tab)
    plt.show()


def displayImage(img):
    plt.imshow(img)
    plt.show()


def displayImageWithNeighs2(img, b):
    neighs = getNeighs(b)
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        for i in range(wblock):
            temp[y0, i + x0] = clr_contour
        for i in range(hblock):
            temp[y0 + i, x0] = clr_contour



        if  displayBlockNumbers:
            # Ajouter le numéro du bloc à côté du rectangle
            text = str(k)

            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]

            text_x = x0 + 5  # decale chiffre vers la droite
            text_y = y0 + 10

            if k in neighs:
                cv2.putText(temp, text, (text_x, text_y), font, font_scale, [0, 0, 255], font_thickness)
            else:
                cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


    plt.imshow(temp)
    plt.show()

def displayImageWithNeighs(img, b):
    neighs = getNeighs(b)
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)


        if not (k in neighs):
            for i in range(wblock):
                temp[y0, i + x0] = clr_contour
            for i in range(hblock):
                temp[y0 + i, x0] = clr_contour
        else:
            for i in range(wblock):
                temp[y0, i + x0] = [0, 0, 255]
            for i in range(hblock):
                temp[y0 + i, x0] = [0, 0, 255]



        if k == b:        
            for i in range(wblock):
                for j in range(hblock):
                    temp[y0+j, x0+i] = [0, 0, 0]

    plt.imshow(temp)
    plt.show()











def displayImageWithBlock(img, highlight=[]):
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]

    temp = deepcopy(img)

    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)

        if k in highlight:
            clr_contour = [0, 255, 255]
            for i in range(wblock):
                temp[y0, i + x0] = clr_contour
                temp[y0+1, i + x0] = clr_contour
                temp[y0+hblock-2, i + x0] = clr_contour
                temp[y0+hblock-1, i + x0] = clr_contour

            for i in range(hblock):
                temp[y0 + i, x0] = clr_contour
                temp[y0 + i, x0+1] = clr_contour
                temp[y0 + i, x0+wblock-2] = clr_contour
                temp[y0 + i, x0+wblock-1] = clr_contour

        else:
            clr_contour = [255, 0, 0]
            for i in range(wblock):
                temp[y0, i + x0] = clr_contour
            for i in range(hblock):
                temp[y0 + i, x0] = clr_contour

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.imshow(temp)

    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        if  displayBlockNumbers:
            # Numero du bloc
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]

            text_x = x0 
            text_y = y0 + 2
            ax.text(text_x, text_y, text, color='black', fontsize=6, ha='left', va='center')
            # cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)

    plt.show()








def displayTabPixels(tableau):
    for ligne in tableau:
        ligne_str = ''
        for pixel in ligne:
            for couleur in pixel:
                ligne_str += f'{couleur:3d}.'.rjust(3)  # alignement à droite avec une largeur de 5 char par composante
            ligne_str += ''
        print(ligne_str)









def displayVectorMap(vectors, img,  saving=False, output_file="output.png"):
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]

    colors = [
        [255, 0, 0],    # Rouge
        [0, 255, 0],    # Vert
        [0, 0, 255],    # Bleu
        [255, 255, 0],  # Jaune
        [128, 0, 0],    # Marron
        [128, 0, 128],  # Violet
        [255, 165, 0],  # Orange
        [0, 255, 127],  # Vert printemps
        [255, 215, 0],  # Or jaune
        [70, 130, 180], # Acier bleu
        [255, 20, 147], # Rose profond
    ]

    colors = [ [0, 0, 0] ]

    
    # for k in range(nombre_blocks):
    #     x0, y0 = block2coordTopLeftCorner(k)
        
    #     for i in range(wblock):
    #         temp[y0, i + x0] = clr_contour
    #     for i in range(hblock):
    #         temp[y0 + i, x0] = clr_contour

    fig, ax = plt.subplots(figsize=(8, 6))

    ax.imshow(temp)

    for k in range(nombre_blocks):

        if blockAverageColor(k, img1) == [255, 255, 255] and blockAverageColor(k, img2) == [255,255,255]:
            continue

        x0, y0 = block2coordTopLeftCorner(k)
        # Coordonnées de départ pour la flèche (au centre du rectangle)
        start = (x0 + wblock // 2, y0 + 5 + hblock // 2)

        if vectors[k] == -1 or vectors[k] == k:
            end = start
        else:
            x1, y1 = block2coordTopLeftCorner(vectors[k])
            dx, dy = x1-x0, y1-y0

            end = (int(x0+dx) + wblock // 2, int(y0+dy) - 5 + hblock // 2)


        arrow_props = dict(facecolor='black', edgecolor='black', arrowstyle='->', shrinkA=0, lw=0.2)
        ax.annotate('', xy=end, xytext=start, arrowprops=arrow_props)


        if  displayBlockNumbers:
            # Numero du bloc
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]

            text_x = x0 + 2
            text_y = y0 + 4
            ax.text(text_x, text_y, text, color='black', fontsize=6, ha='left', va='center')
            # cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)



    plt.show()

    return


def getDCTcoeffs(image, display=False):
    """
    A partir d'une image RGB !! 
    """
    # Convert the image to float32
    image_float32 = image.astype(np.float32) / 255.0

    # Display the original RGB image
    if display:
        plt.subplot(3, 3, 4)
        plt.imshow(image)
        plt.title('Original RGB Image')

    # Apply 2D DCT to each color channel
    dct_image = np.zeros_like(image_float32)
    for i in range(3):  # Loop over R, G, B channels
        dct_image[:, :, i] = dct(dct(image_float32[:, :, i], axis=0, norm='ortho'), axis=1, norm='ortho')

    dct_image2 = np.round(dct_image/pas_quantization).astype(int)

    # Display the DCT coefficients for each channel
    if display:
        RGB = ["R", "G", "B"]
        reconstructed_image = np.zeros_like(image)

        for i in range(3):  # Boucler sur les canaux R, G, B
            plt.subplot(3, 3, i*3+2)
            plt.imshow(np.log(np.abs(dct_image[:, :, i]) + 1), cmap='gray')  # Log-scaled pour une meilleure visualisation
            plt.title(f'DCT Coefficients (Canal {RGB[i]})')


            reconstructed_image[:, :, i] = idct(idct(dct_image[:, :, i], axis=0, norm='ortho'), axis=1, norm='ortho')

        # Clipper les valeurs pour les ramener dans la plage [0, 1]
        reconstructed_image = np.clip(reconstructed_image, 0, 1)*255

        plt.subplot(3, 3, 6)
        plt.imshow(reconstructed_image)
        plt.title('Reconstructed RGB Image')

        plt.show()

    return dct_image2



































#####################################################
####################### STATS #######################
#####################################################



























































#####################################################
###################### FICHIERS #####################
#####################################################

def extractFramesOfVideo(path, offset=0, nombre=2, pas=1):
    """
    Retourne {nombre} images successives de la vidéo mp4
    """
    capture = cv2.VideoCapture(path)

    if not capture.isOpened():
        # Vérifier si la vidéo est ouverte avec succès
        print("Erreur lors de l'ouverture de la vidéo.")
        return None

    # Initialiser l'array pour stocker les frames
    frames = np.empty((nombre,) + (int(capture.get(4)), int(capture.get(3)), 3), dtype=np.uint8)

    for i in range(nombre):
        capture.set(cv2.CAP_PROP_POS_FRAMES, i * pas + offset)
        ret, frame = capture.read()

        if not ret:
            # La lecture a échoué
            print(f"Échec de la lecture de la trame {i * pas + offset}.")
            break

        frames[i] = frame

    capture.release()

    return frames


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



def ecrire_csv(nom_fichier, tableau_numpy):
    with open(nom_fichier, 'w', newline='') as fichier_csv:
        writer = csv.writer(fichier_csv)
        writer.writerow(tableau_numpy)


def lire_csv(nom_fichier):
    with open(nom_fichier, 'r') as fichier_csv:
        reader = csv.reader(fichier_csv)
        # On suppose qu'il y a une seule ligne dans le fichier CSV
        ligne = next(reader)
        # Convertir les valeurs en float et créer un tableau numpy
        tableau_numpy = np.array([float(valeur) for valeur in ligne])
    
    return tableau_numpy




def BGR2RGB(image):
    return cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

def RGB2YCrCb(image):
    return cv2.cvtColor(image, cv2.COLOR_RGB2YCrCb)



def YCrCb2RGB(image):
    return cv2.cvtColor(image, cv2.COLOR_YCrCb2RGB)


def RGB2GRAY(image):
    return cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)




















#####################################################
###################### VECTEURS #####################
#####################################################

def block2coordTopLeftCorner(block):
    x = (block*wblock)%wframe
    y = ((block*wblock)//wframe)*hblock
    return x, y

def coordTopLeftCorner2block(x, y):
    block_col = x // wblock
    block_row = y // hblock
    block = block_col + block_row * (wframe // wblock)
    return block


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


def SAD(b1, img1, b2, img2):
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

            diff[0] += abs( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) )
            diff[1] += abs( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) )
            diff[2] += abs( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) )

            # print(diff)

    diff = diff[0] + diff[1] + diff[2]

    return diff




def buildMotionVector(img1, img2):

    exclus = np.zeros(nombre_blocks, dtype=np.uint8)
    vecteurs = np.zeros(nombre_blocks)


    for k in range(nombre_blocks):
        if k%100 == 0:
            print(f"vecteurs bloc {k} calculés")

        conjugue = TDL(img1, img2, k)

        exclus[conjugue] = 1

        vecteurs[k] = conjugue

    return vecteurs








def isBlockOutside(k):
    return (k < 0) or (k >= nombre_blocks)

# Two Dimensional Logarithmic Search
def TDL(img1, img2, bref, display=False):
    p = block_search_radius
    centre = bref
    step = 0

    while p > 1:
        # Nord - Sud - CENTRE - Ouest - Est
        points = {
            centre+p*nombre_blocks_x: float("+inf"),
            centre-p*nombre_blocks_x: float("+inf"),
            centre:                   float("+inf"),
            centre-p:                 float("+inf"),
            centre+p:                 float("+inf"),
        }

        if (centre + p*nombre_blocks_x)%nombre_blocks_x != (centre%nombre_blocks_x) or (isBlockOutside(centre + p*nombre_blocks_x)):
            del points[centre + p*nombre_blocks_x]

        if (centre - p*nombre_blocks_x)%nombre_blocks_x != (centre%nombre_blocks_x) or (isBlockOutside(centre - p*nombre_blocks_x)):
            del points[centre - p*nombre_blocks_x]

        if ((centre-p)//nombre_blocks_x != centre//nombre_blocks_x) or isBlockOutside(centre-p):
            del points[centre-p]
        if ((centre+p)//nombre_blocks_x != centre//nombre_blocks_x) or isBlockOutside(centre+p):
            del points[centre+p]

        # Ajoute les 9 cases voisines du centre
        if step == 0:
            for k in [centre-nombre_blocks_x-1, centre-nombre_blocks_x, centre-nombre_blocks_x+1, centre-1, centre, centre+1, centre+nombre_blocks_x-1, centre+nombre_blocks_x, centre+nombre_blocks_x+1 ]:
                if not isBlockOutside(k):
                    points.update({k: float("+inf")})


        for pt, val in points.items():
            points[pt] = SAD(bref, img1, pt, img2)

        # step 1
        smallest = points[centre]
        smallest_pt = centre

        for pt, val in points.items():
            if points[pt] < smallest: # ineg stricte pour garder le centre en smallest si il y a egalite de minimum
                smallest = val
                smallest_pt = pt

        if display:
            print(points)
            print("centre", smallest_pt)
            displayImageWithBlock(img2, list(points.keys()))
            # displayImageWithBlock(img2, [smallest_pt])

        # step 2
        if smallest_pt == centre:
            p = p//2
        else:
            centre = smallest_pt

        step += 1


    # step 3
    # on cherche parmi les neuf blocs autour du centre

    smallest = SAD(bref, img1, centre, img2)
    smallest_pt = centre

    for k in [centre-nombre_blocks_x-1, centre-nombre_blocks_x, centre-nombre_blocks_x+1, centre-1, centre, centre+1, centre+nombre_blocks_x-1, centre+nombre_blocks_x, centre+nombre_blocks_x+1 ]:
        if not isBlockOutside(k):
            diff = SAD(bref, img1, k, img2)

            if(diff < smallest):
                smallest = diff
                smallest_pt = k

    if display:
        print(f"choisi: {smallest_pt}")
        displayImageWithBlock(img2, [centre-nombre_blocks_x-1, centre-nombre_blocks_x, centre-nombre_blocks_x+1, centre-1, centre, centre+1, centre+nombre_blocks_x-1, centre+nombre_blocks_x, centre+nombre_blocks_x+1 ])

    return smallest_pt



def testTDL(img1, img2):
    vecteurs = -np.ones(nombre_blocks)
    for k in range(nombre_blocks):
        if blockAverageColor(k, img1) != [255,255,255]:
            vecteurs[k] = TDL(img1, img2, k)
            if k%100 == 1:
                print(f"{k}")
            # print(f"{k} --> {vecteurs[k]}")

    displayVectorMap(vecteurs, img2)
    return






def SADAnchors(img1, img2, anchor1, anchor2):
    x_ref1, y_ref1 = anchor1
    x_ref2, y_ref2 = anchor2

    diff = [0, 0, 0]
    size = wblock*hblock

    for i in range(wblock//pas_calcul_difference):
        for j in range(hblock//pas_calcul_difference):

            x1 = i*pas_calcul_difference+x_ref1
            y1 = j*pas_calcul_difference+y_ref1

            x2 = i*pas_calcul_difference+x_ref2
            y2 = j*pas_calcul_difference+y_ref2

            diff[0] += abs( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) )
            diff[1] += abs( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) )
            diff[2] += abs( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) )

            # print(diff)

    diff = diff[0] + diff[1] + diff[2]

    return diff


def getVector(img1, img2, b_ref, display=False):
    radius = 20

    u, v = 0, 0
    mini = float("+inf")


    for i in range(-radius, radius+1):
        for j in range(-radius, radius+1):

            sad = SADAnchors(img1, img2, block2coordTopLeftCorner(b_ref), (i, j))
            if(sad < overall):
                print(sad)
            if sad < mini:
                mini = sad
                u, v = i, j



    fig, ax = plt.subplots(figsize=(8, 6))
    ax.imshow(img2)

    x0, y0 = block2coordTopLeftCorner(b_ref)
    # Coordonnées de départ pour la flèche (au centre du rectangle)
    start = (x0, y0)

    x1, y1 = x0+u, y0+v
    dx, dy = x1-x0, y1-y0

    end = (int(x0+dx), int(y0+dy))


    arrow_props = dict(facecolor='white', edgecolor='white', arrowstyle='->', shrinkA=0, lw=0.8)
    ax.annotate('', xy=end, xytext=start, arrowprops=arrow_props)

    plt.show()



    return u, v












if __name__ == "__main__":

    # img1 = readImg("tests/car1.png")
    # img2 = readImg("tests/car2.png")
    img1, img2 = extractFramesOfVideo("/users/escud/Desktop/crossing.mp4", offset=0, nombre=2, pas=2)
    # img1, img2 = extractFramesOfVideo("/users/escud/Desktop/sea_shore.mp4", offset=0, nombre=2, pas=1)

    img1,img2 = BGR2RGB(img1), BGR2RGB(img2)

    print("")
    print("******"*10)
    print("******"*10)
    print("")

    hframe, wframe, channels = img1.shape

    # wframe = 256
    # hframe = 512
    wblock = 4*2
    hblock = 4*2

    #nombre_blocks = 1600
    nombre_blocks = int((wframe/wblock) * (hframe/hblock))
    nombre_blocks_x = int((wframe/wblock))
    nombre_blocks_y = int((hframe/hblock))

    print(f"Taille image : {wframe}x{hframe}px")
    print(f"Nombre de block: {nombre_blocks} ({nombre_blocks_x}x{nombre_blocks_y})")
    print(f"Taille block : {wblock}x{hblock}px")

    block_search_radius = round(wframe*0.15)//wblock
    # block_search_radius = 10
    print(f"block_search_radius={block_search_radius}")

    font_thickness = 1
    font_scale = 0.2
    displayBlockNumbers = False
    if nombre_blocks < 2500 and hframe > 10:
        displayBlockNumbers = True


    pas_calcul_difference = 1

    pas_quantization = 1


    print("")
    print("******"*10)
    print("******"*10)
    print("")







    displayBlockNumbers = False
    name_test = "crossing"

    # displayImage(img1)
    # displayImage(img2)
    residu = img2 - img1
    displayImage(residu)

    # print(getDCTcoeffs(residu, True))