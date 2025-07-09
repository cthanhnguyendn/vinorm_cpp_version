#pragma once
#include "unicode/utypes.h"
#include "unicode/ustring.h"
#include "unicode/regex.h"
#include "unicode/ustream.h"
#include "unicode/ucnv.h"
#include "unicode/uclean.h"
#include "unicode/schriter.h"
#include <bits/stdc++.h>
#include "ICUReadFile.h"
#include "ICUConstant.h" 
using std::vector;
using std::cerr;
using icu::UnicodeString;
using icu::RegexPattern;
using icu::RegexMatcher;
using ICUConstant::DIGIT_ZERO;
using ICUConstant::LATIN_CAPITAL_LETTER_A;
using icu::StringCharacterIterator;
class ConvertingNumber{
    private:
        vector<UnicodeString> CHU_SO = {UnicodeString::fromUTF8("không"), 
            UnicodeString::fromUTF8("một"), UnicodeString::fromUTF8("hai"),
            UnicodeString::fromUTF8("ba"), UnicodeString::fromUTF8("bốn"), 
            UnicodeString::fromUTF8("năm"), UnicodeString::fromUTF8("sáu"), 
            UnicodeString::fromUTF8("bảy"), UnicodeString::fromUTF8("tám"),
            UnicodeString::fromUTF8("chín")};
        const UnicodeString MUOI_F = UnicodeString::fromUTF8("mười");
        const UnicodeString MUOI = UnicodeString::fromUTF8("mươi");
        const UnicodeString HUNDRED = UnicodeString::fromUTF8("trăm");
        const UnicodeString THOUSAND = UnicodeString::fromUTF8("nghìn");
        const UnicodeString MILLION = UnicodeString::fromUTF8("triệu");
        const UnicodeString BILLION = UnicodeString::fromUTF8("tỷ");
        const UnicodeString MOTS = UnicodeString::fromUTF8("mốt");
        const UnicodeString TUW = UnicodeString::fromUTF8("tư");
        const UnicodeString LAWM = UnicodeString::fromUTF8("lăm");
        const UnicodeString LINH = UnicodeString::fromUTF8("linh");
        const UnicodeString SPACE = UnicodeString::fromUTF8(" ");
        const UnicodeString COMMA = UnicodeString::fromUTF8(", ");
        const UnicodeString NGHIN = UnicodeString::fromUTF8("nghìn");
        const UnicodeString TRIEU = UnicodeString::fromUTF8("triệu");
        UnicodeString convertNumberLTHundred(const UnicodeString& num);
        UnicodeString convertNumberLTThousand(const UnicodeString& num); 
        UnicodeString convertNumberLTMillion(const UnicodeString& num); 
        UnicodeString convertNumberArbitrary(const UnicodeString& num);
        UnicodeString stripZeros(const UnicodeString& num, int32_t z = 0); 
        void printUnicodeString(const UnicodeString &s) {
            char charBuf[1000];
            s.extract(0, s.length(), charBuf, sizeof(charBuf)-1, 0);   
            charBuf[sizeof(charBuf)-1] = 0;          
            printf("%s", charBuf);
        }
    public:
        UnicodeString convertNumber(const UnicodeString &num);
        UnicodeString decimalToRoman(int number);
        UnicodeString romanToDecimal(const UnicodeString &roman);
        void unitTest();
};
