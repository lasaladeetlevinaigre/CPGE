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



def extract_frames(path, interval=500):
    capture = cv2.VideoCapture(path)

    matrice1 = []

    timestamp = capture.get(cv2.CAP_PROP_POS_MSEC)
    capture.set(cv2.CAP_PROP_POS_FRAMES, 0)

    ret, matrice1 = capture.read()

    capture.set(cv2.CAP_PROP_POS_FRAMES, interval)

    ret, matrice2 = capture.read()

    capture.release()

    return matrice1, matrice2







def writeImage(tab, name):
    width, height = len(tab[0]), len(tab)
    image = np.zeros((height, width, 3), dtype=np.uint8)  # Créer une image noire (fond)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[y, x]

    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée (writeImage 100%)")





def writeImage2(tab, name):
    width, height = len(tab[0])//2, len(tab)//2
    image = np.zeros((height, width, 3), dtype=np.uint8)  # Créer une image noire (fond)
    for x in range(width-1):
        for y in range(height-1):
            image[y, x] = tab[2*y, 2*x]
    cv2.imwrite(f'{name}.png', image)
    print(f"{name} générée (writeImage2 50%)")










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


diffs = []

def calculateBlockDifference(b1, img1, b2, img2):
    x_ref1, y_ref1 = block2coordTopLeftCorner(b1)
    x_ref2, y_ref2 = block2coordTopLeftCorner(b2)
    diff = [0, 0, 0]
    for i in range(wblock//pas_difference):
        for j in range(hblock//pas_difference):
            x1 = i*pas_difference+x_ref1
            y1 = j*pas_difference+y_ref1

            x2 = i*pas_difference+x_ref2
            y2 = j*pas_difference+y_ref2

            diff[0] += pow( ( int(img1[y1, x1][0]) - int(img2[y2, x2][0]) ), 2)
            diff[1] += pow( ( int(img1[y1, x1][1]) - int(img2[y2, x2][1]) ), 2)
            diff[2] += pow( ( int(img1[y1, x1][2]) - int(img2[y2, x2][2]) ), 2)

            #print(diff)

    size = wblock*hblock
    diff = diff[0]/size + diff[1]/size + diff[2]/size
    #a, b, c = np.sqrt(diff)

    diffs.append(diff)
    return diff




def show_diffs(liste):
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



    
def closestNeighs(b1):
    i, j = b1//nombre_blocks_x, b1%nombre_blocks_x
    neigh = []

    for a in range(-block_search_radius, block_search_radius+1):
        for b in range(-block_search_radius, block_search_radius+1):

            k = (i+a)*nombre_blocks_x + j+b

            x, y = k//nombre_blocks_x, k%nombre_blocks_x

            if( k >= 0 and k < nombre_blocks and y >= j-block_search_radius and y <= j+block_search_radius) :
                neigh.append(k)
            
    return neigh


def blockPlusRessemblant(b1, img1, img2, exclus):
    min_diff = math.inf
    b2 = -1
    for k in closestNeighs(b1):
        if not exclus[k]:
            diff = calculateBlockDifference(b1, img1, k, img2)
            if diff < diff_minimale:
                if b2 == -1:
                    b2 = k
                if diff < min_diff:
                    min_diff = diff
                    b2 = k
    return b2


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
    conjugues = np.zeros(nombre_blocks)

    for k in range(nombre_blocks):
        if k%100 == 0:
            print("vecteurs calculé du bloc ", k)

        conjugue = blockPlusRessemblant(k, img1, img2, exclus)

        """
        conjugue = k + nombre_blocks_x + 1 

        if ((k+1) % nombre_blocks_x) == 0:
            conjugue = -1

        if k//nombre_blocks_x == (nombre_blocks_y-1):
            conjugue = k

        """
        exclus[conjugue] = -1

        conjugues[k] = conjugue

        x_a, y_a = block2coordTopLeftCorner(k)
        x_b, y_b = block2coordTopLeftCorner(conjugue)
        
        dx = x_b - x_a
        dy = y_b - y_a

        vecteurs[k] = [dx, dy]

        #print(f"{k} --> {conjugue} //// {x_a, y_a} --> {x_a + dx, y_a + dy}")

    return vecteurs, conjugues






def buildIntermediateFrame2(vectors, conjugues, img_ref, path, nb_frame, d=True):
    if not os.path.exists(path):
        os.makedirs(path)

    for t in range(nb_frame):
        frame = np.zeros((hframe, wframe, 3), dtype=np.uint8)

        for k in range(nombre_blocks):
            x0, y0 = block2coordTopLeftCorner(k)
            for i in range(wblock):
                for j in range(hblock):
                    x = i+x0
                    y = j+y0


                    if False: #conjugues[k] == -1:
                        frame[x, y] = img1[y, x] # le bloc n'a pas d'image, on prend par transparence, le bloc originel
                    else:
                        x_prim = x + int(vectors[k][0] * ( t/nb_frame ) )
                        y_prim = y + int(vectors[k][1] * ( t/nb_frame ) )

                        frame[y_prim, x_prim] = img1[y, x]
            #print(f"frame{t}-block{k}genere")


        #displayImageWithBlock(frame)
        writeImage(frame, f"{path}/f{t}")
        pass

    return




def buildIntermediateFrame(vectors, conjugues, img_ref, path, nb_frame, display=False):
    if not os.path.exists(path):
        os.makedirs(path)

    for t in range(nb_frame+1):
        if (t != 0):
            previous_frame = deepcopy(frame)
        else:
            previous_frame = img_ref

        frame = np.ones((hframe, wframe, 3), dtype=np.uint8) * 255

        for k in range(nombre_blocks):
            j, i = k//nombre_blocks_x, k%nombre_blocks_x

            x0, y0 = block2coordTopLeftCorner(k)

            if conjugues[k] != -1:
                #print("k", k, "i", i, "j", j)

                j_prim, i_prim = conjugues[k]//nombre_blocks_x, conjugues[k]%nombre_blocks_x
                dx, dy = i_prim-i, j_prim-j


                block_a_remplir = (i + round(dx * t/nb_frame)) + (j + round(dy * t/nb_frame)) * nombre_blocks_x
                #print(k,"conjugeu à ", conjugues[k], f"mvnt (dx{dx},dy{dy}) donc on rempli ", block_a_remplir)
                #print("t", t)


                x1, y1 = block2coordTopLeftCorner(block_a_remplir)

                for i in range(wblock):
                    for j in range(hblock):
                        frame[y1 + j, x1 + i] = img_ref[y0 + j, x0 + i]


            else:
                #print(k,"conjugeu à -1")
                for i in range(wblock):
                    for j in range(hblock):
                        frame[y0 + j, x0 + i] = previous_frame[y0 + j, x0 + i] # le bloc n'a pas d'image, on prend par transparence, le bloc originel
                        #frame[y0 + j, x0 + i] = [255, 255, 255]


        if display:
            displayImageWithBlock(frame)
        writeImage(frame, f"{path}/f{t}")
        pass

    return




def afficher_doublons(tableau):
    elements_vus = set()
    doublons = set()

    for element in tableau:
        if element in elements_vus and element != -1:
            doublons.add(element)
        else:
            elements_vus.add(element)

    return list(doublons)








def displayVectorMap(vecteurs, conjugues, img):
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

            # Ajouter le numéro du bloc à côté du rectangle
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            text_x = x0 + 5  # Décaler le texte vers la droite du bloc
            text_y = y0 + 20
            cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


        # Coordonnées de départ pour la flèche (au centre du rectangle)
        arrow_start = (x0 + wblock // 2, y0 + 6 + hblock // 2)
        if conjugues[k] == -1 or conjugues[k] == k:
            arrow_end = (x0 + wblock // 2, y0 + 6 + hblock // 2)
        else:
            arrow_end = (int(x0+vecteurs[k][0]) + wblock // 2, int(y0+vecteurs[k][1]) - 6 + hblock // 2)

        cv2.arrowedLine(temp, arrow_start, arrow_end, colors[k % len(colors)], thickness=1, tipLength=0.04)




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




def displayImageWithNeighs(img, neighs):
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
            # Ajouter le numéro du bloc à côté du rectangle
            text = str(k)

            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]

            text_x = x0 + 5  # Décaler le texte vers la droite du bloc
            text_y = y0 + 10

            if k in neighs:
                cv2.putText(temp, text, (text_x, text_y), font, font_scale, [0, 0, 255], font_thickness)
            else:
                cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


    cv2.imshow("image", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def displayImageWithBlock(img):
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.3
    font_thickness = 1
    font_color = [0, 255, 0]
    
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
    fps = 4
    output_video = cv2.VideoWriter(f"{folder_path}.mp4", cv2.VideoWriter_fourcc(*'mp4v'), fps, (width, height))

    # Ajout des images à la vidéo
    for image_path in image_files:
        img = cv2.imread(image_path)
        output_video.write(img)

    # Libération des ressources
    output_video.release()





















if __name__ == "__main__":


    wframe = 1920 #1280
    hframe = 1080 #720
    wblock = 4*4
    hblock = 9*2

    #nombre_blocks = 1600
    nombre_blocks = int((wframe/wblock) * (hframe/hblock))
    nombre_blocks_x = int((wframe/wblock))
    nombre_blocks_y = int((hframe/hblock))

    print(f"Nombre de block: {nombre_blocks} ({nombre_blocks_x}x{nombre_blocks_y})")


    #block_search_radius = 3
    block_search_radius = round(wframe*0.15)//wblock
    print(f"block_search_radius {block_search_radius}")


    pas_difference = 2
    diff_minimale = 430



    # main()
    # img1 = image2Tab("circle1.png")
    # print(img1)
    # img2 = image2Tab("circle3.png")


    name_test = "shore2"
    img1, img2 = extract_frames("sea_shore.mp4", 10)

    displayImageWithBlock(img1)
    displayImageWithBlock(img2)



    if not os.path.exists(f"out/{name_test}"):
        os.makedirs(f"out/{name_test}")
    writeImage(img1, f"out/{name_test}/test1")
    writeImage(img2, f"out/{name_test}/test2")


    vecteurs, conjugues = buildMotionVector(img1, img2)

    for k in range(nombre_blocks):
        pass
        print(f"{k} --> {conjugues[k]}")



    #displayVectorMap(vecteurs, conjugues, img1)
    displayVectorMap(vecteurs, conjugues, img2)

    show_diffs(diffs)

    buildIntermediateFrame(vecteurs, conjugues, img1, f"out/{name_test}", 12, False)
    images2Video(f'out/{name_test}')
