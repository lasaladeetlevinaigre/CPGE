from copy import deepcopy
import cv2


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
    cv2.imshow(f"bloc {b}", tab)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def displayImage(img):
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def displayImageWithNeighs(img, b):
    neighs = getNeighs(b)
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.3
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


    cv2.imshow("image", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def displayImageWithBlock(img, saving=False, output_file="output.png"):
    temp = deepcopy(img)
    clr_contour = [255, 0, 0]
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.3
    font_color = [0, 0, 0]
    
    for k in range(nombre_blocks):
        x0, y0 = block2coordTopLeftCorner(k)
        
        for i in range(wblock):
            temp[y0, i + x0] = clr_contour
        for i in range(hblock):
            temp[y0 + i, x0] = clr_contour



        if  displayBlockNumbers:
            # Ajouter le num du bloc à côté du rectangle
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            text_x = x0 + 5
            text_y = y0 + 10
            cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)

    cv2.imshow("image", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    if saving:
        cv2.imwrite(output_file, temp)


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
    font_scale = 0.5
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



        if  displayBlockNumbers:
            # Numero du bloc
            text = str(k)
            text_size = cv2.getTextSize(text, font, font_scale, font_thickness)[0]
            text_x = x0 + 5 
            text_y = y0 + 20
            cv2.putText(temp, text, (text_x, text_y), font, font_scale, font_color, font_thickness)


        # Coordonnées de départ pour la flèche (au centre du rectangle)
        arrow_start = (x0 + wblock // 2, y0 + 6 + hblock // 2)

        if vectors[k] == -1 or vectors[k] == k:
            arrow_end = (x0 + wblock // 2, y0 + 6 + hblock // 2)

        else:
            x1, y1 = block2coordTopLeftCorner(k)
            x2, y2 = block2coordTopLeftCorner(vectors[k])
            dx, dy = x2-x1, y2-y1

            arrow_end = (int(x0+dx) + wblock // 2, int(y0+dy) - 6 + hblock // 2)

        cv2.arrowedLine(temp, arrow_start, arrow_end, colors[k % len(colors)], thickness=1, tipLength=0.)


    cv2.imshow("vecteurs", temp)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    if saving:
        cv2.imwrite(output_file, temp)











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

