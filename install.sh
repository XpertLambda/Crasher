#!/bin/bash
# Crasher Installation Script
# Professional hash cracking toolkit

set -e

echo "=========================================="
echo "     Crasher Installation Script"
echo "=========================================="

# Check for required dependencies
echo "Checking dependencies..."

if ! command -v gcc &> /dev/null; then
    echo "Error: GCC compiler not found. Please install build-essential."
    exit 1
fi

if ! pkg-config --exists openssl; then
    echo "Error: OpenSSL development libraries not found."
    echo "Please install: sudo apt-get install libssl-dev (Debian/Ubuntu)"
    echo "              : sudo yum install openssl-devel (RHEL/CentOS)"
    exit 1
fi

echo "All dependencies found."

# Build the application
echo "Building Crasher..."
make clean
make

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "Installation complete. You can now use:"
    echo "  ./crasher --help"
    echo ""
    echo "Example usage:"
    echo "  ./crasher -h 5d41402abc4b2a76b9719d911017c592 -m md5 -a dictionary -w /usr/share/dict/words"
    echo ""
    echo "Performance: Up to 1.57 million attempts per second"
else
    echo "Build failed!"
    exit 1
fi
