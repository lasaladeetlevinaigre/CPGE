import cv2
import numpy as np
import csv
import math
import os

from copy import deepcopy
import matplotlib.pyplot as plt



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


















def image2Tab(path):
    img = cv2.imread(path)
    return img

    '''
    hauteur, largeur, _ = img.shape
    print(hauteur, largeur)

    if hauteur < hframe or largeur < wframe:
        raise ValueError("Les dimensions de l'image sont plus petites que les dimensions du tableau")
    
    tab = np.zeros((wframe, hframe, 3), dtype=np.uint8)
    
    for i in range(wframe):
        for j in range(hframe):
            tab[i][j] = img[j][i]
            
    return tab
    '''








#####################################################
############ DIVISION PAR BLOCK #####################
#####################################################


wframe = 1280
hframe = 720
wblock = 16*4
hblock = 9*8

#nombre_blocks = 1600
nombre_blocks = int((wframe/wblock) * (hframe/hblock))
nombre_blocks_x = int((wframe/wblock))
nombre_blocks_y = int((hframe/hblock))

print(f"Nombre de block: {nombre_blocks} ({nombre_blocks_x}x{nombre_blocks_y})")






def block2coordTopLeftCorner(block):
    x = (block*wblock)%wframe
    y = ((block*wblock)//wframe)*hblock
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


def calculateBlockAverageColor(b, img):
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



def calculateBlockDifference(b1, img1, b2, img2):
    x_ref1, y_ref1 = block2coordTopLeftCorner(b1)
    x_ref2, y_ref2 = block2coordTopLeftCorner(b2)
    diff = [0, 0, 0]
    pas = 4
    for i in range(wblock//pas):
        for j in range(hblock//pas):
            x1 = i*pas+x_ref1
            y1 = j*pas+y_ref1

            x2 = i*pas+x_ref2
            y2 = j*pas+y_ref2

            diff[0] += pow( ( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) ), 2)
            diff[1] += pow( ( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) ), 2)
            diff[2] += pow( ( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) ), 2)

            #print(diff)

    size = wblock*hblock
    diff = np.array([diff[0]/size, diff[1]/size, diff[2]/size])
    a, b, c = np.sqrt(diff)
    return a+b+c






def blockPlusRessemblant(b1, img1, img2, exclus):
    min_diff = math.inf
    b2 = -1
    for k in range(nombre_blocks):
        if not exclus[k]:
            #print(calculateBlockAverageColor(k, img1), f" -- {k}")
            diff = calculateBlockDifference(b1, img1, k, img2)
            if diff < min_diff:
                min_diff = diff
                b2 = k
    return b2

def blockPlusRessemblantDeterminist(b1, img1, img2, exclus):
    if b1 == 20:
        return 21
    elif b1 == 21:
        return 20
    else:
        return b1



def testcalculateBlockDifference(img1, img2):
    b1 = 1375
    b2 = 1850

    print(calculateBlockAverageColor(b1, img1))
    print(calculateBlockAverageColor(b2, img1))

    displayBlock(b1, img1)
    displayBlock(b2, img2)


    print(f"diff {b1}-{b2} = {calculateBlockDifference(b1, img1, b2, img2)}")

    conjugue = blockPlusRessemblant(b1, img1, img2)
    #print(f"blockPlusRessemblant de {b1} ---> {conjugue}")

    displayImageWithBlock(img1)
    displayImageWithBlock(img2)

    displayBlock(conjugue, img2)





def buildMotionVector(img1, img2):
    vecteurs = np.zeros((nombre_blocks, 2), dtype=np.float32)
    exclus = np.zeros(nombre_blocks, dtype=np.uint8)

    for k in range(nombre_blocks):
        conjugue = blockPlusRessemblant(k, img1, img2, exclus)
        exclus[conjugue] = 1

        x_a, y_a = block2coordTopLeftCorner(k)
        x_b, y_b = block2coordTopLeftCorner(conjugue)
        
        dx = x_b - x_a
        dy = y_b - y_a

        vecteurs[k] = [dx, dy]

        print(f"{k} --> {conjugue} //// {x_a, y_a} --> {x_a + dx, y_a + dy}")

    return vecteurs


def buildIntermediateFrame(vectors, img_ref, nb_frame):

    for t in range(nb_frame):
        frame = np.zeros((hframe, wframe, 3), dtype=np.uint8)

        for k in range(nombre_blocks):
            x0, y0 = block2coordTopLeftCorner(k)
            for i in range(wblock):
                for j in range(hblock):
                    x = i+x0
                    y = j+y0

                    x_prim = x + int(vectors[k][0] * ( (t+1)/(nb_frame+1) ) )
                    y_prim = y + int(vectors[k][1] * ( (t+1)/(nb_frame+1) ) )

                    frame[y_prim, x_prim] = img1[y, x]
            #print(f"frame{t}-block{k}genere")


        #displayImageWithBlock(frame)
        writeImage(frame, f"circle/f{t}")
        pass

    return



def testbuildMotionVector(img1, img2):

    vecteurs = buildMotionVector(img1, img2)

    displayVectorMap(vecteurs, img1)

    displayImageWithBlock(img1)
    #displayImageWithBlock(img2)

    #displayVectorMap(vecteurs, img1)

    #buildIntermediateFrame(vecteurs, img1, 30)

    #images2Video('circle')













def displayVectorMap(vecteurs, img):
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.5
    font_thickness = 1
    font_color = [0, 0, 0]

    colors = [
        [255, 0, 0],    # Rouge
        [0, 255, 0],    # Vert
        [0, 0, 255],    # Bleu
        [255, 255, 0],  # Jaune
        [128, 0, 0],    # Marron
        [0, 128, 0],    # Vert foncé
        [0, 0, 128],    # Bleu foncé
        [128, 128, 0],  # Olive
        [128, 0, 128],  # Violet
        [0, 128, 128],  # Sarcelle
        [255, 165, 0],  # Orange
        [139, 69, 19],  # Brun
        [255, 140, 0],  # Orange foncé
        [0, 255, 127],  # Vert printemps
        [218, 112, 214],# Orchidée
        [255, 215, 0],  # Or
        [70, 130, 180], # Acier bleu
        [0, 128, 128],  # Vert foncé cyan
        [255, 20, 147], # Rose profond
        [240, 230, 140],# Kaki
        [0, 0, 128]     # Bleu marine
    ]

    colors = [0, 0, 0]

    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        for i in range(wblock):
            temp[y0, i + x0] = clr_contour
        for i in range(hblock):
            temp[y0 + i, x0] = clr_contour



        if nombre_blocks < 2000:
            avg = calculateBlockAverageColor(k, img)
            if(avg[0] != 255 and avg[1] != 255):
                #print(f"block {k}: clr moyenne : {avg}")
                pass
            # Ajouter le numéro du bloc à côté du rectangle
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            text_x = x0 + 5  # Décaler le texte vers la droite du bloc
            text_y = y0 + 20
            cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


        # Coordonnées de départ pour la flèche (au centre du rectangle)
        arrow_start = (x0 + wblock // 2, y0 + hblock // 2)

        arrow_end = (int(x0+vecteurs[k][0]) + wblock // 2, int(y0+vecteurs[k][1]) + hblock // 2)

        cv2.arrowedLine(temp, arrow_start, arrow_end, colors[k % len(colors)], thickness=2, tipLength=0.08)




    cv2.imshow("image", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()









def displayBlock(b, img):
    x0, y0 = block2coordTopLeftCorner(b)

    tab = np.zeros((hblock, wblock, 3), dtype=np.uint8)
    for i in range(wblock):
        for j in range(hblock):
            tab[j, i] = img[j+y0, i+x0]

    #displayTabPixels(tab)
    cv2.imshow(f"bloc {b}", tab)
    cv2.waitKey(0)  # Attendre indéfiniment jusqu'à ce qu'une touche soit enfoncée
    cv2.destroyAllWindows()



def displayImage(img):
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def displayImageWithBlock(img):
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.3
    font_thickness = 1
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        for i in range(wblock):
            temp[y0, i + x0] = clr_contour
        for i in range(hblock):
            temp[y0 + i, x0] = clr_contour



        if nombre_blocks < 2000:
            avg = calculateBlockAverageColor(k, img)
            if(avg[0] != 255 and avg[1] != 255):
                #print(f"block {k}: clr moyenne : {avg}")
                pass
            # Ajouter le numéro du bloc à côté du rectangle
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            text_x = x0 + 5  # Décaler le texte vers la droite du bloc
            text_y = y0 + 10
            cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)

    cv2.imshow("image", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()



def displayTabPixels(tableau):
    for ligne in tableau:
        ligne_str = ''
        for pixel in ligne:
            for couleur in pixel:
                ligne_str += f'{couleur:3d}.'.rjust(3)  # Alignement à droite avec une largeur de 5 caractères par composante
            ligne_str += ''  # Espacement entre les pixels
        print(ligne_str)





def images2Video(folder_path):
    # Répertoire contenant les images
    images_folder = folder_path

    # Liste des noms de fichiers d'images (assurez-vous qu'elles sont ordonnées)
    image_files = sorted([os.path.join(images_folder, img) for img in os.listdir(images_folder) if img.endswith(".png")])
    print(image_files)

    # Lecture de la première image pour obtenir les dimensions
    first_image = cv2.imread(image_files[0])
    height, width, _ = first_image.shape

    # Création de l'objet VideoWriter
    output_video = cv2.VideoWriter('circle.mp4', cv2.VideoWriter_fourcc(*'mp4v'), 8, (width, height))

    # Ajout des images à la vidéo
    for image_path in image_files:
        img = cv2.imread(image_path)
        output_video.write(img)

    # Libération des ressources
    output_video.release()





def est_aubord(k):
    """
    a = (k-nombre_blocks_x < 0) # cote nord
    b = (k%nombre_blocks_x == 0) #cote est
    c = (k%nombre_blocks_x == nombre_blocks_x-1) # cote ouest
    d = (k+nombre_blocks_x >= nombre_blocks) # cote sud
    """
    return (k-nombre_blocks_x < 0) or (k%nombre_blocks_x == 0) or (k%nombre_blocks_x == nombre_blocks_x-1) or (k+nombre_blocks_x >= nombre_blocks)


def testClosestNeigh(b):

    directions = [[True, 0, 1], [True, 1, 1], [True, 1, 0], [True, 1, -1], [True, 0, -1], [True, -1, -1], [True, -1, 0], [True, -1, 1]]

        
    depth = 0
    while depth < nombre_blocks:
        depth += 1

        for direction in directions:
            if direction[0]:
                pass

            k = b + depth * direction[1] + depth * direction[2]*nombre_blocks_x

            print(k)

            if est_aubord(k):
                direction[0] = False







if __name__ == "__main__":
    # main()
    img1 = image2Tab("circle1.png")
    #print(img1)
    img2 = image2Tab("circle3.png")


    #testbuildMotionVector(img1, img2)

    testClosestNeigh(5)

    #images2Video('circle')