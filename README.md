=> Overview

This project implements Image Steganography, the technique of hiding secret information inside an image without visibly altering it.
It allows users to encode (hide) messages inside image files and later decode (extract) the hidden information.

This project demonstrates practical concepts of data security, cryptography, and image processing.

---

=> Features

Hide secret text inside an image
Extract hidden text from a steganographed image
Uses LSB (Least Significant Bit) method for data hiding
Supports lossless image formats (e.g., BMP)
Simple CLI (Command Line Interface) for encoding/decoding

---

=> Technologies Used

Programming Language: C 
Libraries: Standard file I/O and image handling libraries
Concepts: Steganography, Bit Manipulation, File Handling

---

=> Project Structure

Image-Steganography/
│── test_encode.c     # Entry point for the program
│── types.h           # main header file
│── common.h          # header file
│── encode.h          # Header file for function encode
│── encode.c       ,  # Functions to encode (hide) data
│── decode.h          # Header file for function decode
│── decode.c          # Functions to decode (extract) data
│── beautiful.bmp     # Original cover image
│── stego.bmp         # Image with hidden message
│── secret.txt        # Sample secret message
│── output.txt        # Decoded message file

---

=> How to Run

1. Clone the repository:

git clone https://github.com/<your-username>/Image-Steganography.git
cd Image-Steganography


2. Compile the program:

gcc main.c encode.c decode.c -o stego


3. To encode a secret message:

./stego -e  beautiful.bmp secret.txt stego.bmp


4. To decode and reveal hidden message:

./stego -d stego.bmp output.txt

---

=> Learning Outcomes

Understanding of steganography and data security
Hands-on practice with bitwise operations
Experience in file handling & image processing
Practical implementation of information hiding techniques
