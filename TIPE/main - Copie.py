import cv2
import numpy as np
import csv
import math
import os
from copy import deepcopy
import matplotlib.pyplot as plt
from scipy.fftpack import dct, idct
import time



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


def displayImage(img, name="displayImage"):
    plt.imshow(img)
    plt.title(name)
    plt.show()


def displayImageWithNeighs2(img, b):
    neighs = getNeighs(b)
    temp = deepcopy(img)
    
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocs):
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
    
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocs):
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
    
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_color = [0, 0, 0]

    temp = deepcopy(img)

    for k in range(nombre_blocs):
        x0, y0 = block2coordTopLeftCorner(k)

        if k in highlight:
            for i in range(wblock):
                temp[y0, i + x0] = [255, 0, 0]
                # temp[y0+1, i + x0] = clr_contour
                # temp[y0+hblock-2, i + x0] = clr_contour
                # temp[y0+hblock-1, i + x0] = clr_contour

            for i in range(hblock):
                temp[y0 + i, x0] = [255, 0, 0]
                # temp[y0 + i, x0+1] = clr_contour
                # temp[y0 + i, x0+wblock-2] = clr_contour
                # temp[y0 + i, x0+wblock-1] = clr_contour

        else:
            
            for i in range(wblock):
                temp[y0, i + x0] = clr_contour
            for i in range(hblock):
                pass
                temp[y0 + i, x0] = clr_contour

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.imshow(temp)

    for k in range(nombre_blocs):
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









def displayVectorMap2(vectors, img,  saving=False, output_file="output.png"):
    temp = deepcopy(img)
    
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

    
    # for k in range(nombre_blocs):
    #     x0, y0 = block2coordTopLeftCorner(k)
        
    #     for i in range(wblock):
    #         temp[y0, i + x0] = clr_contour
    #     for i in range(hblock):
    #         temp[y0 + i, x0] = clr_contour

    fig, ax = plt.subplots(figsize=(8, 6))

    ax.imshow(temp)

    for k in range(nombre_blocs):

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




def displayVectorMap(vectors, img, saving=False, output_file="vector_map"):
    fig, ax = plt.subplots(figsize=(8, 6))
    ax.imshow(img)

    for k in range(nombre_blocs):

        x0, y0 = block2coordTopLeftCorner(k)
        # Coordonnées de départ pour la flèche (au centre du rectangle)
        start = (x0, y0)

        u, v = vectors[k]

        if (u, v) == (0, 0):
            continue


        x1, y1 = x0+u, y0+v
        dx, dy = x1-x0, y1-y0

        end = (int(x0+dx), int(y0+dy))


        arrow_props = dict(facecolor='white', edgecolor='white', arrowstyle='->', shrinkA=0, lw=0.8)
        ax.annotate('', xy=end, xytext=start, arrowprops=arrow_props)

    plt.show()



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
##################### QUALITE #######################
#####################################################

def MSE(img_original, img_predicted):
    """
    Calcule Mean Squared Error (MSE)
    """

    # Normalize pixel values to the range [0, 1]
    img_original = img_original.astype(np.float32) / 255.0
    img_predicted = img_predicted.astype(np.float32) / 255.0

    mse_channel_R = np.mean((img_original[:, :, 0] - img_predicted[:, :, 0]) ** 2)
    mse_channel_G = np.mean((img_original[:, :, 1] - img_predicted[:, :, 1]) ** 2)
    mse_channel_B = np.mean((img_original[:, :, 2] - img_predicted[:, :, 2]) ** 2)

    mse_channels = [mse_channel_R, mse_channel_G, mse_channel_B]

    # Calculate overall MSE (average of channel MSEs)
    mse_global = np.mean(mse_channels)

    return mse_global, mse_channels




def PSNR(img_original, img_predicted, max_pixel_value=255):
    """
    Calcule le Peak Signal-to-Noise Ratio (PSNR)
    """
    mse_global, mse_channels = MSE(img_original, img_predicted)

    psnr_channels = (20 * np.log10(max_pixel_value / np.sqrt(mse_channels))).tolist()

    psnr_global = np.mean(psnr_channels)

    return psnr_global, psnr_channels

























































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
    i, j = b1//nombre_blocs_x, b1%nombre_blocs_x
    neigh = []

    for cote in range(block_search_radius):

        for x in [-cote, cote]:
            for y in range(-cote, cote+1):

                k = (i+x) * nombre_blocs_x + j+y
                if k >= 0 and k < nombre_blocs and k%nombre_blocs_x >= j-block_search_radius and k%nombre_blocs_x <= j+block_search_radius and k not in neigh:
                    neigh.append(k)

        for x in range(-cote, cote+1):
            for y in [-cote, cote]:

                k = (i+x) * nombre_blocs_x + j+y
                if k >= 0 and k < nombre_blocs and k%nombre_blocs_x >= j-block_search_radius and k%nombre_blocs_x <= j+block_search_radius and k not in neigh:
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

    exclus = np.zeros(nombre_blocs, dtype=np.uint8)
    vecteurs = np.zeros(nombre_blocs)


    for k in range(nombre_blocs):
        if k%100 == 0:
            print(f"vecteurs bloc {k} calculés")

        conjugue = TDL(img1, img2, k)

        exclus[conjugue] = 1

        vecteurs[k] = conjugue

    return vecteurs








def isBlockOutside(k):
    return (k < 0) or (k >= nombre_blocs)

# Two Dimensional Logarithmic Search
def TDL(img1, img2, bref, display=False):
    p = block_search_radius
    centre = bref
    step = 0

    if display:
        print(f"centre initial : {bref} = ({block2coordTopLeftCorner(bref)})")

    while p > 1:
        # Nord - Sud - CENTRE - Ouest - Est
        points = {
            centre+p*nombre_blocs_x: float("+inf"),
            centre-p*nombre_blocs_x: float("+inf"),
            centre:                   float("+inf"),
            centre-p:                 float("+inf"),
            centre+p:                 float("+inf"),
        }

        if (centre + p*nombre_blocs_x)%nombre_blocs_x != (centre%nombre_blocs_x) or (isBlockOutside(centre + p*nombre_blocs_x)):
            del points[centre + p*nombre_blocs_x]

        if (centre - p*nombre_blocs_x)%nombre_blocs_x != (centre%nombre_blocs_x) or (isBlockOutside(centre - p*nombre_blocs_x)):
            del points[centre - p*nombre_blocs_x]

        if ((centre-p)//nombre_blocs_x != centre//nombre_blocs_x) or isBlockOutside(centre-p):
            del points[centre-p]
        if ((centre+p)//nombre_blocs_x != centre//nombre_blocs_x) or isBlockOutside(centre+p):
            del points[centre+p]

        # Ajoute les 9 cases voisines du centre
        if step == 0:
            for k in [centre-nombre_blocs_x-1, centre-nombre_blocs_x, centre-nombre_blocs_x+1, centre-1, centre, centre+1, centre+nombre_blocs_x-1, centre+nombre_blocs_x, centre+nombre_blocs_x+1 ]:
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
            print(f"p = {p}")
            print(points)
            print(f"centre choisi: {smallest_pt} = ({block2coordTopLeftCorner(smallest_pt)})")
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

    for k in [centre-nombre_blocs_x-1, centre-nombre_blocs_x, centre-nombre_blocs_x+1, centre-1, centre, centre+1, centre+nombre_blocs_x-1, centre+nombre_blocs_x, centre+nombre_blocs_x+1 ]:
        if not isBlockOutside(k):
            diff = SAD(bref, img1, k, img2)

            if(diff < smallest):
                smallest = diff
                smallest_pt = k

    if display:
        print(f"choisi: {smallest_pt}")
        displayImageWithBlock(img2, [centre-nombre_blocs_x-1, centre-nombre_blocs_x, centre-nombre_blocs_x+1, centre-1, centre, centre+1, centre+nombre_blocs_x-1, centre+nombre_blocs_x, centre+nombre_blocs_x+1 ])

    return smallest_pt



def testTDL(img1, img2):
    vecteurs = -np.ones(nombre_blocs)
    for k in range(nombre_blocs):
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

            if x1 < 0 or x1 > wframe or y1 < 0 or y1 > hframe:
                continue
            if x2 < 0 or x2 > wframe or y2 < 0 or y2 > hframe:
                continue

            diff[0] += abs( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) )
            diff[1] += abs( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) )
            diff[2] += abs( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) )

            # print(diff)

    diff = diff[0] + diff[1] + diff[2]

    return diff





def getVector(img1, img2, b_ref, vectors=[], display=False):
    if display:
        print("\n"*2)
        print("******"*10)
        print(f"{bcolors.HEADER}        TDL bloc_ref: {b_ref} | coord: {block2coordTopLeftCorner(b_ref)}{bcolors.ENDC}")    
        print("\n")


    median_vector = [float("+inf"), float("+inf")]

    if len(vectors) != 0:
        # haut gauche - haut - haut droite - gauche
        points = {b_ref-nombre_blocs_x-1, b_ref-nombre_blocs_x, b_ref-nombre_blocs_x+1, b_ref-1}

        # sur la premiere ligne
        if b_ref < nombre_blocs_x:
            points.discard(b_ref-nombre_blocs_x-1)
            points.discard(b_ref-nombre_blocs_x)
            points.discard(b_ref-nombre_blocs_x+1)

        # sur la premiere colonne
        if b_ref%nombre_blocs_x == 0:
            points.discard(b_ref-nombre_blocs_x-1)
            points.discard(b_ref-1)

        # sur la derniere ligne
        if b_ref%nombre_blocs_x == nombre_blocs_x-1:
            points.discard(b_ref-nombre_blocs_x+1)


        if len(points) != 0:
            median_vector = [0, 0]
            for k in points:
                median_vector[0] = vectors[k][0]
                median_vector[1] = vectors[k][1]
            median_vector[0] = round(median_vector[0]/len(points))
            median_vector[1] = round(median_vector[1]/len(points))
        else:
            median_vector = [float("+inf"), float("+inf")]

    if display:
        print(f"median_vector: {median_vector}")





    x0, y0 = block2coordTopLeftCorner(b_ref)

    # center_origin = x0+median_vector[0], y0+median_vector[1]
    center_origin = (x0, y0)

    p = search_radius
    center = center_origin
    pos = [center_origin] # historique des centre de passage
    while p > 1:

        mini = float("+inf")
        x, y = center

        pos.append(center)

        # (dx, dy, poids)
        relatives = [
            (0, -p, 1.), # haut
            (0, +p, 1.), # bas
            (0, 0, 1.2),  # centre
            (-p, 0, 1.), # gauche
            (+p, 0, 1.), # droite

            # (-p, -p, 1.), # haut gauche
            # (+p, -p, 1.), # haut droite
            # (-p, +p, 1.), # bas gauche
            # (+p, +p, 1.), # bas droite
        ]

        if len(pos) == 2 and median_vector[0] != float("+inf"):
            # on ajoute le vecteur median
            dx, dy = median_vector
            relatives.append((dx, dy, 1.5))

        if len(pos) == 2:
            # on cherche parmi les neuf blocs autour du centre
            for i in range(-1, 2):
                for j in range(-1, 2):
                    relatives.append((i, j, 1.2))

        best_i, best_j = 0, 0
        for (i, j, weight) in relatives:
            if (x+i) < 0 or (x+i+wblock) > wframe or (y+j) < 0 or (y+j+hblock) > hframe:
                continue

            # print(x, y, i, j, weight)
            sad = SADAnchors(img1, img2, (x0, y0), (i+x, j+y)) / weight
            if sad < mini:
                mini = sad
                x = i+x
                y = j+y
                best_i, best_j = i, j


        if display:
            print(f"{bcolors.OKBLUE}pas: p={p} {bcolors.ENDC}")
            print(f"centre: {center}")
            print(f"positions comparées: (dx, dy, weight) \n{np.array(relatives)}")
            print(f"meilleure correspondance si centre = {x, y}, obtenu avec {best_i, best_j}")
            print("--")


        # step 2
        if (x, y) == center or (x, y) in pos:
            p = p//2 # si le centre n a pas evolue ou sil oscille

        center = x, y




    # step 3
    # on cherche parmi les neuf blocs autour du centre

    best = float("+inf")
    u, v = 0, 0
    for i in range(-1, 2):
        for j in range(-1, 2):

            if (x+i) < 0 or (x+i+wblock) > wframe or (y+j) < 0 or (y+j+hblock) > hframe:
                continue
            sad = SADAnchors(img1, img2, (x0, y0), (i+x, j+y))
            if sad < best:
                best = sad
                u, v = (x+i)-x0, (y+j)-y0



    if display:
        print(f"vecteur trouvé: {u, v}")
        print(f"centres visités: {pos}")
        fig, ax = plt.subplots(figsize=(8, 6))
        ax.imshow(img2)

        start = (x0, y0)

        x1, y1 = x0+u, y0+v
        dx, dy = x1-x0, y1-y0

        end = (int(x0+dx), int(y0+dy))


        arrow_props = dict(facecolor='white', edgecolor='white', arrowstyle='->', shrinkA=0, lw=0.8)
        ax.annotate('', xy=end, xytext=start, arrowprops=arrow_props)

        plt.show()


    return u, v









def getVectorsField(img1, img2, display=False):
    start_time = time.time()

    vectors = np.zeros((nombre_blocs, 2)).tolist()

    for k in range(nombre_blocs-nombre_blocs_x):
        vectors[k] = getVector(img1, img2, k, vectors)

    print(f"temps calcul vecteurs: {time.time()-start_time}")

    if display:
        start_time = time.time()
        fig, ax = plt.subplots(figsize=(8, 6))
        ax.imshow(img2)

        for k in range(nombre_blocs):

            start = block2coordTopLeftCorner(k)
            x0, y0 = start
            u, v = vectors[k]
            dx, dy = u, v

            end = (int(x0+dx), int(y0+dy))


            arrow_props = dict(facecolor='white', edgecolor='white', arrowstyle='->', shrinkA=0, lw=0.8)
            ax.annotate('', xy=end, xytext=start, arrowprops=arrow_props)

        print(f"temps affichage vecteurs: {time.time()-start_time}")

        plt.savefig("vector_field.png")
        plt.show()


    return vectors




def reconstructImage(img1, vectors):
    # Mettre le fond en jaune
    # reconstructed_image = np.zeros((hframe, wframe, 3), dtype=int)
    # reconstructed_image[:, :, 0] = 255  # Canal rouge
    # reconstructed_image[:, :, 1] = 255  # Canal vert

    # Mettre le fond sur l'image precedente
    reconstructed_image = deepcopy(img1)

    for k in range(nombre_blocs):
        x1, y1 = block2coordTopLeftCorner(k)
        dx, dy = vectors[k]

        for i in range(wblock):
            for j in range(hblock):

                x, y = int(x1+dx+i), int(y1+dy+j)

                if x < 0 or x >= wframe or y < 0 or y >= hframe:
                    continue

                reconstructed_image[y][x] = img1[y1+j][x1+i]

    return reconstructed_image











if __name__ == "__main__":


    # img1 = readImg("tests/car1.png")
    # img2 = readImg("tests/car2.png")
    img1, img2 = extractFramesOfVideo("/users/escud/Desktop/f1.mp4", offset=180, nombre=2, pas=1)
    # img1, img2 = extractFramesOfVideo("/users/escud/Desktop/crossing.mp4", offset=180*2, nombre=2, pas=1)

    img1,img2 = BGR2RGB(img1), BGR2RGB(img2)

    hframe, wframe, channels = img1.shape

    # wframe = 256
    # hframe = 512
    wblock = 4*4
    hblock = 4*4

    #nombre_blocs = 1600
    nombre_blocs = int((wframe/wblock) * (hframe/hblock))
    nombre_blocs_x = int((wframe/wblock))
    nombre_blocs_y = int((hframe/hblock))


    block_search_radius = 16 # !!!! plus d'usage
    search_radius = 16

    font_thickness = 1
    font_scale = 0.2
    displayBlockNumbers = False
    if nombre_blocs < 2500 and hframe > 10:
        displayBlockNumbers = True


    pas_calcul_difference = 4

    pas_quantization = 1

    clr_contour = [255, 255, 255]


    print("******"*10)
    print("******"*10)
    print(f"        Taille image : {wframe}x{hframe}px")
    print(f"        Nombre de block: {nombre_blocs} ({nombre_blocs_x}x{nombre_blocs_y})")
    print(f"        Taille block : {wblock}x{hblock}px")
    print(f"        search_radius={search_radius}")
    print(f"        pas_calcul_difference={pas_calcul_difference}")
    print("******"*10)
    print("******"*10)
    print("\n")











    displayBlockNumbers = False
    name_test = "crossing"

    ref = coordTopLeftCorner2block(894, 293)
    ref = coordTopLeftCorner2block(1035, 356) # voiture phare arriere
    ref = 3540

    conj = ref
    # print(f"conjugué de {ref} est {conj}")
    # print(f"conjugué de {block2coordTopLeftCorner(ref)} est {block2coordTopLeftCorner(conj)}")
    print(f"SUIVI: bloc {ref} ({block2coordTopLeftCorner(ref)})")



    # displayImageWithBlock(img1, [ref])
    # displayImageWithBlock(img2, [ref, conj])

    
    # for _ in range(8100):

    # u, v = getVector(img1, img2, ref, [], True)

    

    vectors = getVectorsField(img1, img2, display=True)
    # vectors = np.ones((nombre_blocs, 2))*8

    reconstructed_image = reconstructImage(img1, vectors)


    displayImage(reconstructed_image, [ref])
    
    residu = img2-reconstructed_image

    displayImage(residu)

    print(f"MSE du résidu: {MSE(img2, residu)[0]}")
    print(f"PSNR du résidu: {PSNR(img2, residu)[0]}")

    displayImage(reconstructed_image+residu)




    print("\n"*3)