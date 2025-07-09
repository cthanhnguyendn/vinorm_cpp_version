# Vietnamese Text Normalization Test Suite

This directory contains comprehensive tests for the Vietnamese text normalization project.

## Test Files

- `test.cpp` - Main test file containing unit tests for all components
- `test_input.txt` - Sample input text for integration tests
- `run_tests.sh` - Linux/Unix test runner script
- `run_tests.bat` - Windows test runner script
- `TEST_README.md` - This file

## Test Coverage

The test suite covers the following components:

### 1. ICUHelper Tests
- `removeExtraWhitespace()` - Tests whitespace normalization
- `isNumberLiteral()` - Tests number validation
- `splitFractionUnit()` - Tests unit fraction parsing

### 2. ICUNumberConverting Tests
- Basic number conversion (0-1000)
- Complex number conversion (123, 2020)
- Vietnamese number word generation

### 3. DateTime Tests
- Date pattern processing (12/2020)
- Time pattern processing (14:30)
- Vietnamese date/time word generation

### 4. Address Tests
- Street number processing (123 Nguyễn Huệ)
- District processing (Quận 1)
- Vietnamese address normalization

### 5. SpecialCase Tests
- Acronym processing (Covid-19)
- Teen code processing (xmz)
- Special word handling

### 6. ICUMapping Tests
- Mapping file loading
- Letter-to-sound mapping
- Vietnamese phonetic rules

### 7. ICUDictionary Tests
- Dictionary file loading
- Word lookup functionality
- Popular word handling

### 8. Math Tests
- Mathematical expression processing
- Mathematical symbol handling
- Vietnamese math terminology

### 9. End-to-End Tests
- Complete normalization pipeline
- Input validation
- Pattern detection

### 10. Specific Case Tests
- Tests the example from README: "Có phải tháng 12/2020 đã có vaccine phòng ngừa Covid-19 xmz ?"
- Date normalization (12/2020)
- Acronym processing (Covid-19)
- Teen code handling (xmz)
- Medical terminology (vaccine)
- Question structure validation

## Running Tests

### On Windows
```cmd
cd cpp_src/src
run_tests.bat
```

### On Linux/Unix
```bash
cd cpp_src/src
chmod +x run_tests.sh
./run_tests.sh
```

### Manual Testing
```bash
# Build the test executable
make test

# Run unit tests
./test

# Build the main executable
make main

# Test with sample input
cp test_input.txt input.txt
./main
./main -lower
./main -unknown
./main -punc
./main -rule
```

## Test Input Examples

The `test_input.txt` file contains various test cases:

- **Main Example**: "Có phải tháng 12/2020 đã có vaccine phòng ngừa Covid-19 xmz ?"
- **Numbers**: "5 quả táo và 3 quả cam"
- **Dates**: "12/2020", "25/12/2023"
- **Times**: "14:30"
- **Addresses**: "123 Nguyễn Huệ, Quận 1"
- **Acronyms**: "Covid-19", "iPhone 14 Pro Max"
- **Units**: "5km/h", "50cc", "256GB"
- **Technical terms**: "WiFi 5GHz", "USB 3.0", "HDMI 2.1"

## Expected Output

The tests verify that:
1. Numbers are converted to Vietnamese words
2. Dates and times are properly formatted
3. Addresses are normalized
4. Acronyms are handled correctly
5. Units are converted to Vietnamese
6. Special cases are processed appropriately

## Troubleshooting

### Build Issues
- Ensure ICU libraries are properly installed
- Check that `LD_LIBRARY_PATH` is set correctly
- Verify all required header files are present

### Runtime Issues
- Check that all dictionary and mapping files exist
- Ensure proper file permissions
- Verify ICU library compatibility

### Test Failures
- Review the test output for specific failure details
- Check that expected Vietnamese text matches actual output
- Verify that all required files are in the correct locations

## Adding New Tests

To add new tests:

1. Add test functions to `test.cpp`
2. Call them from the `main()` function
3. Update this README with new test descriptions
4. Add sample input to `test_input.txt` if needed

## Test Results

The test suite provides:
- Individual test pass/fail status
- Detailed error messages for failures
- Summary statistics
- Integration test results
- Output file generation for verification 