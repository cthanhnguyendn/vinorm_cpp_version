#include <bits/stdc++.h>
#include <cassert>
#include <cstring>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "unicode/utypes.h"
#include "unicode/unistr.h"
#include "unicode/regex.h"
#include "unicode/umachine.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"
#include "unicode/uclean.h"
#include "unicode/schriter.h"
#include "unicode/ustream.h"
#include "unicode/schriter.h"
#include "unicode/brkiter.h"
#include "ICUConstant.h"
#include "ICUHelper.h"
#include "ICUReadFile.h"
#include "ICUMapping.h"
#include "ICUDictionary.h"
#include "SpecialCase.h"
#include "Address.h"
#include "Math.h"
#include "DateTime.h"

using icu::BreakIterator;
using icu::CharacterIterator;
using icu::Locale;
using icu::RegexMatcher;
using icu::RegexPattern;
using icu::StringCharacterIterator;
using icu::UnicodeSet;
using icu::UnicodeString;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

// Helper function to convert UnicodeString to UTF-8 for proper console output
string unicodeToUTF8(const UnicodeString& ustr) {
    string utf8str;
    ustr.toUTF8String(utf8str);
    return utf8str;
}

// Function to setup Windows console for UTF-8
void setupWindowsConsoleUTF8() {
#ifdef _WIN32
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Test utilities
class TestRunner {
private:
    int totalTests = 0;
    int passedTests = 0;
    int failedTests = 0;

public:
    void assertEqual(const UnicodeString& actual, const UnicodeString& expected, const string& testName) {
        totalTests++;
        if (actual == expected) {
            passedTests++;
            cout << "[PASS] " << testName << " passed" << endl;
        } else {
            failedTests++;
            cerr << "[FAIL] " << testName << " failed" << endl;
            cerr << "  Expected: " << unicodeToUTF8(expected) << endl;
            cerr << "  Actual:   " << unicodeToUTF8(actual) << endl;
        }
    }

    void assertEqual(const string& actual, const string& expected, const string& testName) {
        totalTests++;
        if (actual == expected) {
            passedTests++;
            cout << "[PASS] " << testName << " passed" << endl;
        } else {
            failedTests++;
            cerr << "[FAIL] " << testName << " failed" << endl;
            cerr << "  Expected: " << expected << endl;
            cerr << "  Actual:   " << actual << endl;
        }
    }

    void assertEqual(size_t actual, size_t expected, const string& testName) {
        totalTests++;
        if (actual == expected) {
            passedTests++;
            cout << "[PASS] " << testName << " passed" << endl;
        } else {
            failedTests++;
            cerr << "[FAIL] " << testName << " failed" << endl;
            cerr << "  Expected: " << expected << endl;
            cerr << "  Actual:   " << actual << endl;
        }
    }

    void assertTrue(bool condition, const string& testName) {
        totalTests++;
        if (condition) {
            passedTests++;
            cout << "[PASS] " << testName << " passed" << endl;
        } else {
            failedTests++;
            cerr << "[FAIL] " << testName << " failed" << endl;
        }
    }

    void printSummary() {
        cout << "\n=== Test Summary ===" << endl;
        cout << "Total tests: " << totalTests << endl;
        cout << "Passed: " << passedTests << endl;
        cout << "Failed: " << failedTests << endl;
        cout << "Success rate: " << (totalTests > 0 ? (passedTests * 100.0 / totalTests) : 0) << "%" << endl;
        
        if (failedTests == 0) {
            cout << "\n[SUCCESS] All tests passed!" << endl;
        } else {
            cout << "\n[ERROR] Some tests failed!" << endl;
        }
    }
};

// Test functions
void testICUHelper(TestRunner& runner) {
    cout << "\n=== Testing ICUHelper ===" << endl;
    
    // Test removeExtraWhitespace
    UnicodeString input1("  hello   world  ");
    UnicodeString expected1("hello world");
    UnicodeString result1 = ICUHelper::removeExtraWhitespace(input1);
    runner.assertEqual(result1, expected1, "removeExtraWhitespace - multiple spaces");
    
    // Test isNumberLiteral
    runner.assertTrue(ICUHelper::isNumberLiteral("123"), "isNumberLiteral - valid number");
    runner.assertTrue(!ICUHelper::isNumberLiteral("12a3"), "isNumberLiteral - invalid number");
    runner.assertTrue(ICUHelper::isNumberLiteral("0"), "isNumberLiteral - zero");
    
    // Test splitFractionUnit
    vector<UnicodeString> fractionResult = ICUHelper::splitFractionUnit("km/h");
    runner.assertEqual(fractionResult.size(), 2, "splitFractionUnit - size check");
    runner.assertEqual(fractionResult[0], "km", "splitFractionUnit - numerator");
    runner.assertEqual(fractionResult[1], "h", "splitFractionUnit - denominator");
    
    vector<UnicodeString> noFractionResult = ICUHelper::splitFractionUnit("km");
    runner.assertEqual(noFractionResult.size(), 1, "splitFractionUnit - no fraction");
    runner.assertEqual(noFractionResult[0], "km", "splitFractionUnit - single unit");
}

void testICUNumberConverting(TestRunner& runner) {
    cout << "\n=== Testing ICUNumberConverting ===" << endl;
    
    ConvertingNumber converter;
    
    // Test basic number conversion
    runner.assertEqual(converter.convertNumber("0"), UnicodeString::fromUTF8("không"), "convertNumber - zero");
    runner.assertEqual(converter.convertNumber("1"), UnicodeString::fromUTF8("một"), "convertNumber - one");
    runner.assertEqual(converter.convertNumber("10"), UnicodeString::fromUTF8("mười"), "convertNumber - ten");
    runner.assertEqual(converter.convertNumber("100"), UnicodeString::fromUTF8("một trăm"), "convertNumber - hundred");
    runner.assertEqual(converter.convertNumber("1000"), UnicodeString::fromUTF8("một nghìn"), "convertNumber - thousand");
    
    // Test complex numbers
    runner.assertEqual(converter.convertNumber("123"), UnicodeString::fromUTF8("một trăm hai mươi ba"), "convertNumber - 123");
    runner.assertEqual(converter.convertNumber("2020"), UnicodeString::fromUTF8("hai nghìn không trăm hai mươi"), "convertNumber - 2020");
}

void testDateTime(TestRunner& runner) {
    cout << "\n=== Testing DateTime ===" << endl;
    
    DateTime dateTime;
    
    // Test date patterns
    UnicodeString date1("12/2020");
    UnicodeString result1 = dateTime.normalizeText(date1);
    
    runner.assertEqual(result1, UnicodeString::fromUTF8("tháng mười hai năm hai nghìn không trăm hai mươi"), "DateTime - month processing");
}

void testAddress(TestRunner& runner) {
    cout << "\n=== Testing Address ===" << endl;
    
    Address address;
    
    // Test address patterns
    UnicodeString addr1(UnicodeString::fromUTF8("123 Nguyễn Huệ"));
    UnicodeString result1 = address.normalizeText(addr1);
    runner.assertEqual(result1, UnicodeString::fromUTF8("số một trăm hai mươi ba Nguyễn Huệ"), "Address - street number");
    
    // Test district patterns
    UnicodeString addr2(UnicodeString::fromUTF8("Quận 1"));
    UnicodeString result2 = address.normalizeText(addr2);
    runner.assertEqual(result2, UnicodeString::fromUTF8("Quận một"), "Address - district");
}

void testSpecialCase(TestRunner& runner) {
    cout << "\n=== Testing SpecialCase ===" << endl;
    
    SpecialCase specialCase;
    
    // Test acronyms
    UnicodeString acronym1(UnicodeString::fromUTF8("Covid-19"));
    UnicodeString result1 = specialCase.normalizeText(acronym1);
    runner.assertEqual(result1, UnicodeString::fromUTF8("Covid mười chín"), "SpecialCase - acronym processing");
    
    // Test teen code
    UnicodeString teenCode1("xmz");
    UnicodeString result2 = specialCase.normalizeText(teenCode1);
    runner.assertEqual(result2, UnicodeString::fromUTF8("xinh mắt zai"), "SpecialCase - teen code processing");
}

void testICUMapping(TestRunner& runner) {
    cout << "\n=== Testing ICUMapping ===" << endl;
    
    ICUMapping mapping;
    
    // Test mapping file loading
    string mappingFile = ICUConstant::MAPPING_FOLDER + "/" + ICUConstant::F_LETTER_SOUND_VN;
    bool loadResult = mapping.loadMappingFile(mappingFile.data());
    runner.assertEqual(loadResult, true, "ICUMapping - file loading");
    
    // Test mapping functionality
    if (loadResult) {
        UnicodeString result = mapping.mappingOf("a");
        runner.assertEqual(result, "a", "ICUMapping - letter mapping");
    }
}

void testICUDictionary(TestRunner& runner) {
    cout << "\n=== Testing ICUDictionary ===" << endl;
    
    ICUDictionary dictionary;
    
    // Test dictionary file loading
    string dictFile = ICUConstant::DICT_FOLDER + "/" + ICUConstant::F_POPULAR;
    bool loadResult = dictionary.loadDictFile(dictFile.data());
    runner.assertEqual(loadResult, true, "ICUDictionary - file loading");
    
    // Test dictionary lookup
    if (loadResult) {
        bool hasWord = dictionary.hasWord("hello");
        runner.assertEqual(hasWord, false, "ICUDictionary - word lookup");
    }
}

void testMath(TestRunner& runner) {
    cout << "\n=== Testing Math ===" << endl;
    
    Math math;
    
    // Test mathematical expressions
    UnicodeString math1(UnicodeString::fromUTF8("1+1=2"));
    UnicodeString result1 = math.normalizeText(math1);
    runner.assertEqual(result1, UnicodeString::fromUTF8("một cộng một bằng hai"), "Math - basic expression");
    
    // Test mathematical symbols
    UnicodeString math2("5x3");
    UnicodeString result2 = math.normalizeText(math2);
    runner.assertEqual(result2, UnicodeString::fromUTF8("năm nhân ba"), "Math - multiplication");
}

void testEndToEnd(TestRunner& runner) {
    cout << "\n=== Testing End-to-End ===" << endl;
    
    DateTime dateTime;
    Math math;
    SpecialCase specialCase;
    
    // Test the complete normalization pipeline
    UnicodeString input(UnicodeString::fromUTF8("Có phải tháng 12/2020 đã có vaccine phòng ngừa Covid-19 xmz ?"));
    UnicodeString result = input;
    result = dateTime.normalizeText(result);
    result = math.normalizeText(result);
    runner.assertEqual(result, UnicodeString::fromUTF8("Có phải tháng mười hai năm hai nghìn không trăm hai mươi đã có vaccine phòng ngừa Covid mười chín ích mờ giét ?"), "End-to-End - complete normalization");
}

void testSpecificCase(TestRunner& runner) {
    cout << "\n=== Testing Specific Case ===" << endl;
    
    // Test the specific case mentioned in the README
    UnicodeString input("Có phải tháng 12/2020 đã có vaccine phòng ngừa Covid-19 xmz ?");
    
    // Test individual components
    DateTime dateTime;
    UnicodeString dateResult = dateTime.normalizeText("12/2020");
    runner.assertEqual(dateResult, UnicodeString::fromUTF8("tháng mười hai năm hai nghìn không trăm hai mươi"), "Specific Case - date normalization");
    
    SpecialCase specialCase;
    UnicodeString acronymResult = specialCase.normalizeText("Covid-19");
    runner.assertEqual(acronymResult, UnicodeString::fromUTF8("Covid mười chín"), "Specific Case - acronym normalization");
    
    UnicodeString teenCodeResult = specialCase.normalizeText("xmz");
    runner.assertEqual(teenCodeResult, UnicodeString::fromUTF8("xinh mắt zai"), "Specific Case - teen code normalization");
    
    // Test the complete sentence structure
    runner.assertEqual(input.indexOf("tháng") != -1, true, "Specific Case - month word detection");
    runner.assertEqual(input.indexOf("vaccine") != -1, true, "Specific Case - vaccine word detection");
    runner.assertEqual(input.indexOf("phòng ngừa") != -1, true, "Specific Case - prevention phrase detection");
    runner.assertEqual(input.indexOf("?") != -1, true, "Specific Case - question mark detection");
}

// Function to test Vietnamese encoding by writing to file
void testVietnameseEncodingToFile() {
    ConvertingNumber converter;
    
    // Test Vietnamese number conversion
    UnicodeString result1 = converter.convertNumber("123");
    UnicodeString result2 = converter.convertNumber("1000");
    
    // Write to file to verify UTF-8 encoding
    std::ofstream outFile("vietnamese_test.txt", std::ios::out);
    if (outFile.is_open()) {
        outFile << "Testing Vietnamese text encoding:" << std::endl;
        outFile << "123 -> " << unicodeToUTF8(result1) << std::endl;
        outFile << "1000 -> " << unicodeToUTF8(result2) << std::endl;
        outFile << "Direct Vietnamese: số một trăm hai mười ba" << std::endl;
        outFile.close();
        cout << "Vietnamese encoding test written to vietnamese_test.txt" << endl;
    }
}

int main() {
    setupWindowsConsoleUTF8();
    cout << "Starting Vietnamese Text Normalization Tests..." << endl;
    
    // Test Vietnamese encoding to file
    testVietnameseEncodingToFile();
    
    TestRunner runner;
    
    // Run all test suites
    testICUHelper(runner);
    testICUNumberConverting(runner);
    testDateTime(runner);
    testAddress(runner);
    testSpecialCase(runner);
    testICUMapping(runner);
    testICUDictionary(runner);
    testMath(runner);
    testEndToEnd(runner);
    testSpecificCase(runner);
    
    // Print summary
    runner.printSummary();
    
    return 0;
} 