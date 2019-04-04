# LSBSteg
LSB Steganography with C++

## Requirements
CMake
OpenCV

## Building
While in the LSBSteg directory
```
cmake .
make
```

## Usage
```
./main.exe encode -i <input> -o <output> -f <file>
./main.exe decode -i <input> -o <output>
```
Example:
```
./main.exe encode -i image.png -o imageOut.png -f message.txt
./main.exe decode -i imageOut.png -o output.txt
```

# Note on Implementation
The image will accept as much of the message as it can and simply drop the rest.
