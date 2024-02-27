# Fonction pour récupérer la table des codes et le texte compressé
def compress(text):
    # Définition de la classe NodeTree
    class NodeTree(object):
        def __init__(self, left=None, right=None):
            self.left = left
            self.right = right

        def children(self):
            return (self.left, self.right)

        def nodes(self):
            return (self.left, self.right)

        def __str__(self):
            return '%s_%s' % (self.left, self.right)

    # Fonction pour créer l'arbre de Huffman
    def huffman_code_tree(node, left=True, binString=''):
        if type(node) is str:
            return {node: binString}
        (l, r) = node.children()
        d = dict()
        d.update(huffman_code_tree(l, True, binString + '0'))
        d.update(huffman_code_tree(r, False, binString + '1'))
        return d

    # Calcul des fréquences
    freq = {}
    for c in text:
        if c in freq:
            freq[c] += 1
        else:
            freq[c] = 1

    freq = sorted(freq.items(), key=lambda x: x[1], reverse=True)
    nodes = freq

    # Construction de l'arbre de Huffman
    while len(nodes) > 1:
        (key1, c1) = nodes[-1]
        (key2, c2) = nodes[-2]
        nodes = nodes[:-2]
        node = NodeTree(key1, key2)
        nodes.append((node, c1 + c2))
        nodes = sorted(nodes, key=lambda x: x[1], reverse=True)

    # Création de la table des codes de Huffman
    huffmanCode = huffman_code_tree(nodes[0][0])

    # Encodage du texte
    compressed_text = ''.join([huffmanCode[char] for char in text])

    return huffmanCode, compressed_text

# Fonction pour décompresser à partir du code Huffman
def decompress(huffmanCode, compressed_text):
    decoded_text = ''
    current_code = ''
    for bit in compressed_text:
        current_code += bit
        for char, code in huffmanCode.items():
            if code == current_code:
                decoded_text += char
                current_code = ''
                break

    return decoded_text



if __name__ == "__main__":
    # Exemple d'utilisation
    text = 'BCAADDDCCACACAC'
    huffmanCode, compressed_text = compress(text)
    
    print('Char | Huffman code')
    print('----------------------')
    for char, code in huffmanCode.items():
        print('%-4r |%12s' % (char, code))

    # print('\nOriginal Text:', text)
    # print('\nOriginal Text Bin:', ''.join(format(ord(char), '08b') for char in text))
    # print('Compressed Text Bin:', compressed_text)

    # Décompression
    decoded_text = decompress(huffmanCode, compressed_text)
    print('Decoded Text:', decoded_text)
