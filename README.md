# image-steganography
An image steganography console application project for PJC course at PJAIT, written in C++. It supports ```.ppm``` and ```.bmp``` files.

# Usage
```image_steganography.exe [option] [image] [message]```
## Options
```
-i, --info      View information about the image
-e, --encrypt   Encrypt a message in a supported image format
-d, --decrypt   Decrypt a message from a supported image format
-c, --check     Check if your message can be encrypted in a supported image format
-h, --help      View help information
```

## Example
```image_steganography.exe -e "smallFile.bmp" "my secret message"```
(note that the path to an image must be absolute)

# Building
```
git clone https://github.com/Spacoon/image-steganography
mkdir build && cd build
cmake ../image-steganography
cmake --build .
```
You may need to set ```image-steganography``` as a startup project in your IDE.
