import cv2
import numpy as np
import csv
import math
import os
from copy import deepcopy
import matplotlib.pyplot as plt




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
            clr_contour[0] = 0
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
            clr_contour[0] = 255
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
            # text_x = x0 + 5 
            # text_y = y0 + 20
            text_x = x0 + 10
            text_y = y0 + 5
            ax.text(text_x, text_y, text, color='black', fontsize=6, ha='center', va='center')
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

    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        for i in range(wblock):
            temp[y0, i + x0] = clr_contour
        for i in range(hblock):
            temp[y0 + i, x0] = clr_contour

    fig, ax = plt.subplots(figsize=(8, 6))

    ax.imshow(temp)

    for k in range(nombre_blocks):

        if blockAverageColor(k, img2) == [255, 255, 255]:
            continue

        x0, y0 = block2coordTopLeftCorner(k)
        # Coordonnées de départ pour la flèche (au centre du rectangle)
        start = (x0 + wblock // 2, y0 + 6 + hblock // 2)

        if vectors[k] == -1 or vectors[k] == k:
            end = (x0 + wblock // 2, y0 + 6 + hblock // 2)
        else:
            x1, y1 = block2coordTopLeftCorner(k)
            x2, y2 = block2coordTopLeftCorner(vectors[k])
            dx, dy = x2-x1, y2-y1

            end = (int(x0+dx) + wblock // 2, int(y0+dy) - 6 + hblock // 2)
        arrow_props = dict(facecolor='black', edgecolor='black', arrowstyle='->', shrinkA=0, lw=1)
        ax.annotate('', xy=start, xytext=end, arrowprops=arrow_props)


        if  displayBlockNumbers:
            # Numero du bloc
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            # text_x = x0 + 5 
            # text_y = y0 + 20
            text_x = x0 + 10
            text_y = y0 + 5
            ax.text(text_x, text_y, text, color='black', fontsize=6, ha='center', va='center')
            # cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)



    plt.show()

    return

































#####################################################
####################### STATS #######################
#####################################################

def show_diffs_calulees(liste):
    l_max = liste[0]
    for element in liste:
        if l_max <= element:
            l_max = element


    occurrences = np.array([0 for k in range(math.ceil(l_max / 10)+1)])

    for element in liste:
        occurrences[math.ceil(element / 10)] += 1


    print("\n\n")
    print("*"*35)
    print("********* Analyse des différences *********")
    print("*"*35)

    print(f"{len(liste)} éléments")
    print(f"{np.mean(liste):.2f} en moyenne")

    print(occurrences)

    print("\n\n")

    
    plt.figure(figsize=(8, 6))
    plt.bar(np.arange(len(occurrences)), occurrences, align='center', alpha=0.7)
    plt.xlabel('Centaines')
    plt.ylabel('Occurrences')
    plt.title('Occurrences par intervalle de 10')
    plt.grid(True)
    plt.show()







































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
def calculateBlockDifferenceSquare(b1, img1, b2, img2):
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

    diff = diff[0] + diff[1] + diff[2]
    #a, b, c = np.sqrt(diff)

    diffs_calulees.append(diff)
    return diff




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





def blockPlusRessemblant(b1, img1_avg, img2_avg, exclus):
    min_diff = math.inf
    b2 = -1

    for k in getNeighs(b1):
        if exclus[k] == 0:
            diff = abs( img1_avg[b1][0] - img2_avg[k][0] ) + abs( img1_avg[b1][1] - img2_avg[k][1] ) +abs( img1_avg[b1][2] - img2_avg[k][2] )
            if diff < min_diff:
                min_diff = diff
                b2 = k
    return b2




def blockPlusRessemblant2(b1, img1, img2, exclus):
    # exclus = np.zeros(nombre_blocks)

    min_diff = math.inf
    b2 = -1

    for k in getNeighs(b1):
        if exclus[k] == 0:
            diff = SAD(b1, img1, k, img2)
            if diff < min_diff:
                min_diff = diff
                b2 = k
    return b2

def blockPlusRessemblant3(b1, img1, img2, exclus = None):
    if exclus == None:
        exclus = np.zeros(nombre_blocks)
    min_diff = math.inf
    b2 = -1

    for k in getNeighs(b1):
        if exclus[k] == 0:


            diff = SAD(b1, img1, k, img2)

            if diff < diff_minimale:
                if b2 == -1:
                    b2 = k
                if diff < min_diff:
                    min_diff = diff
                    b2 = k
    return b2

def blockPlusRessemblant4(b1, img1, img2, exclus):

    for k in getNeighs(b1):
        if exclus[k] == 0:
            diff = SAD(b1, img1, k, img2)
            if diff < diff_minimale:
                return k

    return -1




def buildMotionVector(img1, img2):

    exclus = np.zeros(nombre_blocks, dtype=np.uint8)
    vecteurs = np.zeros(nombre_blocks)

    img1_avg = np.array( [blockAverageColor(k, img1) for k in range(0, nombre_blocks)] )
    img2_avg = np.array( [blockAverageColor(k, img2) for k in range(0, nombre_blocks)] )



    for k in range(nombre_blocks):
        if k%100 == 0:
            print(f"vecteurs bloc {k} calculés")

        #conjugue = blockPlusRessemblant(k, img1_avg, img2_avg, exclus)
        conjugue = TDL(img1, img2, k)

        exclus[conjugue] = 1

        vecteurs[k] = conjugue

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













def isBlockOutside(k):
    return (k < 0) or (k >= nombre_blocks)

# Two Dimensional Logarithmic Search
def TDL(img1, img2, bref):
    p = block_search_radius
    centre = bref

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

        if isBlockOutside(centre-p):
            del points[centre-p]
        if isBlockOutside(centre+p):
            del points[centre+p]


        for pt, val in points.items():
            points[pt] = SAD(bref, img1, pt, img2)

        # step 1
        smallest = points[centre]
        smallest_pt = centre

        for pt, val in points.items():
            if points[pt] < smallest: # ineg stricte pour garder le centre en smallest si il y a egalite de minimum
                smallest = val
                smallest_pt = pt

        # print(points)
        # print("centre", smallest_pt)
        # displayImageWithBlock(img2, list(points.keys()))
        # displayImageWithBlock(img2, [smallest_pt])

        # step 2
        if smallest_pt == centre:
            p = p//2
        else:
            centre = smallest_pt


    # step 3
    # on cherche parmi les neuf blocs autour du centre

    smallest = SAD(bref, img1, centre, img2)
    smallest_pt = centre

    for k in [centre-nombre_blocks_y-1, centre-nombre_blocks_y, centre-nombre_blocks_y+1, centre-1, centre, centre+1, centre+nombre_blocks_y-1, centre-nombre_blocks_y, centre+nombre_blocks_y+1 ]:
        if not isBlockOutside(k):
            diff = SAD(bref, img1, k, img2)

            if(diff < smallest):
                smallest = diff
                smallest_pt = k


    return smallest_pt






























if __name__ == "__main__":

    print("")
    print("******"*10)
    print("******"*10)
    print("")

    wframe = 512
    hframe = 256
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
    #block_search_radius = 4
    print(f"block_search_radius={block_search_radius}")

    font_thickness = 1
    font_scale = 0.2
    displayBlockNumbers = False
    if nombre_blocks < 2500 and hframe > 10:
        displayBlockNumbers = True


    pas_calcul_difference = 1
    diff_minimale = 10000

    print("")
    print("******"*10)
    print("******"*10)
    print("")


    displayBlockNumbers = True

    name_test = "rond"

    #img1, img2 = extractFramesOfVideo("sea_shore.mp4", 10)
    img1 = readImg("tests/frame3.png")
    img2 = readImg("tests/frame4.png")
    # displayImageWithBlock(img1, [206, 211])

    # displayImageWithBlock(img1, [374, 379])

    # print(TDL(img1, img2, 138))

    vecteurs = buildMotionVector(img1, img2)
    displayVectorMap(vecteurs, img2)

    # ecrire_csv("tests/vectorsTDL.csv", np.array(vecteurs))


    # for k in range(nombre_blocks):
    #     pass
    #     print(f"{k} --> {vecteurs[k]}")

    # displayVectorMap(vecteurs, img1, True, "vectors.png")



    # show_diffs_calulees(diffs_calulees)

    # buildIntermediateFrame(vecteurs, img1, f"out/{name_test}", 10, False)
    # images2Video(f"out/{name_test}")
