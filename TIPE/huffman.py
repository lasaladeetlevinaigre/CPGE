from huffman import HuffmanCodec

# Exemple de texte
text = "hello world"

# Compression
codec = HuffmanCodec.from_data(text)
encoded_text = codec.encode(text)
print(f"Encoded Text: {encoded_text}")

# Décompression
decoded_text = codec.decode(encoded_text)
print(f"Decoded Text: {decoded_text}")
