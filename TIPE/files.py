import cv2



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

