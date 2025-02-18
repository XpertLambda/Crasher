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
├── crasherSrc/          # Main decryption and hash-cracking module
│   ├── DeHash/         # Core decryption logic
│   │   ├── HashTypes/  # Supported hash types (MD5, SHA1, SHA256)
│   │   ├── Options/    # Attack mode options
│   ├── Include.h       # Header files
│   ├── Main.c          # Main execution file
│   ├── src/            # Additional source files
├── generatorSrc/       # Wordlist generator module
│   ├── src/            # Wordlist logic
│   │   ├── wordlist.c  # Wordlist creation
│   │   ├── wordlist.h  # Header file for wordlist handling
│   ├── Makefile        # Build script
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
./crasher <hash> [options]
```
- `<hash>`: The hash to be cracked.
- `[options]`: Choose attack mode (`--wordlist`, `--ascii`, `--random`).

### Wordlist Generator
```bash
./generator -o output.txt -l 8
```
- `-o output.txt`: Specifies output file.
- `-l 8`: Sets word length to 8.

## License
This project is licensed under **Creative Commons Attribution-NonCommercial (CC BY-NC 4.0)**.

## Contributions
Contributions are welcome! However, please note that forks should not be publicly redistributed.

## Author
XpertLambda