import cv2
import numpy as np
import csv
import math
import os
from copy import deepcopy
import matplotlib.pyplot as plt



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
