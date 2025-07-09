#!/bin/bash

echo "=== Vietnamese Text Normalization Test Suite ==="
echo ""

# Set library path
export LD_LIBRARY_PATH=../lib

# Clean previous builds
echo "Cleaning previous builds..."
make clean

# Build the test executable
echo "Building test executable..."
make test

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build successful!"
echo ""

# Run the unit tests
echo "Running unit tests..."
./test

if [ $? -ne 0 ]; then
    echo "❌ Unit tests failed!"
    exit 1
fi

echo ""
echo "=== Integration Tests ==="

# Test the main program with different options
echo "Testing main program with default options..."
cp test_input.txt input.txt
./main
if [ $? -eq 0 ]; then
    echo "✅ Default normalization test passed"
    echo "Output:"
    head -5 output.txt
else
    echo "❌ Default normalization test failed"
fi

echo ""
echo "Testing main program with lowercase option..."
./main -lower
if [ $? -eq 0 ]; then
    echo "✅ Lowercase normalization test passed"
    echo "Output:"
    head -5 output.txt
else
    echo "❌ Lowercase normalization test failed"
fi

echo ""
echo "Testing main program with unknown word handling..."
./main -unknown
if [ $? -eq 0 ]; then
    echo "✅ Unknown word handling test passed"
    echo "Output:"
    head -5 output.txt
else
    echo "❌ Unknown word handling test failed"
fi

echo ""
echo "Testing main program with punctuation preservation..."
./main -punc
if [ $? -eq 0 ]; then
    echo "✅ Punctuation preservation test passed"
    echo "Output:"
    head -5 output.txt
else
    echo "❌ Punctuation preservation test failed"
fi

echo ""
echo "Testing main program with regex-only mode..."
./main -rule
if [ $? -eq 0 ]; then
    echo "✅ Regex-only mode test passed"
    echo "Output:"
    head -5 output.txt
else
    echo "❌ Regex-only mode test failed"
fi

echo ""
echo "=== Test Summary ==="
echo "All tests completed!"
echo "Check the output files for detailed results."
echo ""
echo "Generated files:"
ls -la output.txt input.txt 2>/dev/null || echo "No output files found"

# Clean up
echo ""
echo "Cleaning up..."
make clean

echo "✅ Test suite completed!" 