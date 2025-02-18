# Crasher

## Overview
Crasher is a cryptographic toolkit designed for hash decryption and wordlist generation. It provides tools for breaking common hash algorithms such as MD5, SHA1, and SHA256 using multiple attack methods, including ASCII-based, random list, and wordlist approaches.

## Features
- **Hash Decryption**: Supports MD5, SHA1, and SHA256 hashes.
- **Multiple Attack Modes**: ASCII brute force, random list, and wordlist-based cracking.
- **Custom Wordlist Generator**: Generates wordlists tailored for password cracking.
- **Modular Structure**: Organized source code for easy modifications and extensions.

## Project Structure
```
crasher/
├── crasherSrc/                 # Main decryption and hash-cracking module
│   ├── src/                    # Source code directory
│   │   ├── DeHash/             # Core hash-cracking functions
│   │   │   ├── DeHash.c        # DeHash implementation
│   │   │   ├── DeHash.h        # DeHash header
│   │   │   ├── HashTypes/      # Hash algorithm types (e.g., MD5, SHA1)
│   │   │   │   ├── md5Hash.c   # MD5 hashing implementation
│   │   │   │   ├── SHA1Hash.c  # SHA1 hashing implementation
│   │   │   │   └── SHA256Hash.c # SHA256 hashing implementation
│   │   │   │
│   │   │   ├── Options/        # Options for cracking techniques
│   │   │   │   ├── ASCII.c     # ASCII cracking options
│   │   │   │   ├── Randomlist.c # Random list cracking options
│   │   │   │   └── Wordlist.c  # Wordlist cracking options
│   │   │   │
│   │   ├── Include.h            # Global header file for inclusion
│   │   ├── Logo/                # Logo display related code
│   │   │   ├── Logo.c           # Logo display implementation
│   │   │   └──Logo.h           # Logo header
│   │   └── Main.c               # Main program entry point
│   │
├── generatorSrc/                # Source code for generating wordlists
│   ├── src/                     # Generator source code
│   │   ├── main.c               # Main wordlist generation file
│   │   ├── wordlist.c           # Wordlist creation logic
│   │   └──wordlist.h           # Wordlist header file
│
├── Logo/                        # Logo text file
│   └── logo.txt                 # Contains the logo in text format
│
├── Makefile                     # Build automation file
└── readme.md                    # Project documentation
```

## Installation
```bash
git clone https://github.com/XpertLambda/crasher.git
cd crasher
make
```

## Usage
### Hash Decryption
```bash
./crasher
```

### Wordlist Generator
```bash
./generator
```

## License
This project is licensed under **Creative Commons Attribution-NonCommercial (CC BY-NC 4.0)**.

## Contributions
Contributions are welcome! However, please note that forks should not be publicly redistributed.

## Author
XpertLambda