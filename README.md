# QR Code Generator

## Overview
This project is a **QR Code Generator** written in C++. It provides functionality to create QR codes and export them in various formats, such as PNG images. The project is designed for simplicity and efficiency, making it easy to integrate into other applications.

## Features
- Generate QR codes from input data.
- Export QR codes as PNG files.
- Adjustable QR code scaling for better resolution.
- Efficient memory handling and performance optimization.

## Requirements
To build and run this project, ensure you have the following dependencies installed:
- **C++ Compiler (G++ or Clang)**
- **CMake** (for building the project)
- **libpng** (for exporting QR codes as PNG images)

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/hassanbassam2005/QRCODE.git
   cd QRCODE
   ```
2. Build the project using CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the QR Code Generator:
   ```sh
   ./qrcode_generator
   ```

## Usage
To generate a QR code, run the program and provide input data:
```sh
./qrcode_generator "Hello, World!"
```

This will generate a QR code image named `qrcode.png` containing the text "Hello, World!".

## Code Structure
- **`src/`** - Contains the source code files.
- **`include/`** - Header files for the project.
- **`build/`** - Directory for compiled files.
- **`CMakeLists.txt`** - CMake configuration file.
- **`README.md`** - Documentation for the project.

## Contributing
Contributions are welcome! Feel free to fork the repository, create a feature branch, and submit a pull request.

## License
This project is licensed under the MIT License. See `LICENSE` for more details.

## Contact
For any inquiries or issues, contact [Hassan Bassam](https://github.com/hassanbassam2005).

