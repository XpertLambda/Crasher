# Crasher Professional

**Advanced Hash Cracking Toolkit**

A high-performance hash cracking utility supporting MD5, SHA-1, and SHA-256 algorithms with multiple attack methods. Optimized for speed and reliability with professional-grade architecture.

## Features

- **Hash Support**: MD5, SHA-1, SHA-256 with automatic detection
- **Attack Methods**: Dictionary, Brute Force, Random, Auto (intelligent sequence)
- **High Performance**: 1.5+ million attempts/second brute force, 1.3+ million random attempts/second
- **Dual Interface**: Professional CLI and GTK+ graphical interface (when available)
- **Target Types**: Password cracking and PIN recovery with optimized character sets
- **Memory Safe**: Secure memory management with proper cleanup
- **Optimized**: Native architecture tuning and efficient algorithms

## Requirements

### Essential Dependencies
- OpenSSL development libraries (`libssl-dev`)
- GCC compiler with standard libraries
- Standard C library (libc)

### Optional Dependencies
- GTK+ 3.0 development libraries (`libgtk-3-dev`) - for GUI interface
- pkg-config - for automatic GTK+ detection

## Installation

### Quick Start (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get install build-essential libssl-dev

# Optional: Install GTK+ for GUI support
sudo apt-get install libgtk-3-dev pkg-config

# Build the project
make

# Test the application
./crasher --help
```

### Advanced Build Options
```bash
make            # Optimized release build
make clean      # Clean build files
```

## Usage

### Command Line Interface
```bash
# Interactive mode (default)
./crasher

# Explicit CLI mode
./crasher --cli

# GUI mode (requires GTK+)
./crasher --gui

# Help and version info
./crasher --help
./crasher --version
```

### Attack Methods

1. **Dictionary Attack**: Fast wordlist-based attack using files in specified directory
2. **Brute Force**: Systematic enumeration of all possible combinations up to specified length
3. **Random Attack**: High-speed random password generation and testing
4. **Auto Attack**: Intelligent multi-phase approach (Dictionary → Random → Brute Force)

### Target Types

- **Password**: Full character set including letters, numbers, and special characters
- **PIN**: Numeric-only codes for faster processing

### Performance Characteristics

- **Brute Force**: 1.5+ million attempts/second with systematic coverage
- **Random Attack**: 1.3+ million attempts/second with probabilistic approach  
- **Dictionary**: 95k+ attempts/second with real-world password lists
- **Memory Usage**: Efficient with minimal footprint and proper cleanup

## Examples

### Basic Usage
```bash
# Start the application
./crasher

# Example session:
Enter the hash to crack: 098f6bcd4621d373cade4e832627b4f6
Detected: MD5 hash

# Select Password target and Brute Force method
# Result: Password "test" found in 7.2M attempts (4.57 seconds)
```

### Sample Test Hashes
```
MD5('hello'):    5d41402abc4b2a76b9719d911017c592
MD5('test'):     098f6bcd4621d373cade4e832627b4f6
SHA-1('test'):   a94a8fe5ccb19ba61c4c0873d391e987982fbbd3
SHA-256('123'):  a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
```

### Realistic Performance Examples
```bash
# Dictionary attack: 247 attempts in <0.01 seconds
# Random attack: 1,000,000 attempts in 0.76 seconds  
# Brute force: 7,209,328 attempts in 4.57 seconds (found "test")
```

## Performance

### Benchmarked Results
- **Brute Force**: 1.57 million attempts/second sustained performance
- **Random Attack**: 1.32 million attempts/second with probabilistic coverage
- **Dictionary**: 95,000+ attempts/second with file I/O optimization
- **Hash Detection**: Automatic MD5/SHA-1/SHA-256 recognition by length

### Technical Specifications
- **Memory Efficient**: Optimized context reuse and minimal allocations
- **Stable Execution**: Memory-safe implementation with proper cleanup
- **Progress Tracking**: Real-time attempt counting and rate calculation
- **Cross-Platform**: Standard C11 with POSIX compatibility

## Architecture

### Project Structure
```
src/
├── main.c                 # Application entry point and argument parsing
├── common/
│   ├── types.h           # Core type definitions and enumerations
│   └── utils.c/h         # Utility functions and memory management
├── crypto/
│   └── hash_engine.c/h   # OpenSSL-based hash computation engine
├── engine/
│   └── attack_engine.c/h # Attack orchestration and algorithms
├── ui/
│   └── ui.c/h            # Professional command-line interface
└── gui/
    └── gui.c/h           # GTK+ graphical interface (optional)
```

### Key Components
- **Hash Engine**: OpenSSL EVP interface for secure hash computation
- **Attack Engine**: Modular attack implementations with shared result structures  
- **UI Layer**: Clean separation between CLI and GUI interfaces
- **Memory Management**: Safe allocation/deallocation with leak prevention

## Security Notice

**FOR EDUCATIONAL AND AUTHORIZED TESTING ONLY**

This tool is designed for legitimate security purposes:
- Educational and learning environments
- Authorized penetration testing with proper permissions
- Password strength assessment for owned systems
- Security research and vulnerability assessment

**Legal Responsibility**: Users must ensure compliance with all applicable laws and regulations. Unauthorized use against systems you do not own is illegal and unethical.

## Technical Details

### Supported Algorithms
- **MD5**: 128-bit hash (32 hex characters)
- **SHA-1**: 160-bit hash (40 hex characters)  
- **SHA-256**: 256-bit hash (64 hex characters)

### Character Sets
- **PIN Mode**: Digits 0-9 (10 characters)
- **Password Mode**: a-z, A-Z, 0-9 (62 characters)
- **Password + Special**: Above plus symbols (94 characters)

## License

This project is licensed under the GNU GPL v3.0. See [LICENSE](LICENSE).

This tool is provided for educational and research purposes. Users must comply with all applicable laws and regulations.

---

**Crasher Professional** - Advanced Hash Cracking Toolkit  
*High-performance • Memory-safe • Educational*
