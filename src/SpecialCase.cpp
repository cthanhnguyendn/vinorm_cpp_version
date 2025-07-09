#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/regex.h>
#include <unicode/umachine.h>
#include "SpecialCase.h"
void SpecialCase::loadPatterns(int categories, string filename) {
    // Open file, read line by line
    string file = REGEX_FOLDER + "/" + filename;
    ICUReadFile f(file.data());
    bool readStatus = f.readFile();
    if (!readStatus) {
        cerr << "[E] Error reading pattern file: " << filename << std::endl;
        return ;
    }
    for (f.nextLine(0); f.getLineStart() < f.getFileLength(); f.nextLine(f.getLineEnd())) {
        UnicodeString line = UnicodeString(FALSE, f.getContentUChar() + f.getLineStart(), 
                f.getLineEnd()  - f.getLineStart());
        line.trim();
        patterns[categories].push_back(line);
    }
}
UnicodeString SpecialCase::stringForReplace(int categories, RegexMatcher* matcher, UErrorCode &status) {
    //cerr << "[L] Using rule " << categories << " of Special Case\n";
    switch(categories) {
        case PHONE_NUMBER:
            return regexPhoneNumber(matcher, status);
        case FOOTBALL_OTHER:
            return regexFootballOther(matcher, status);
        case FOOTBALL_UNDER:
            return regexFootballUnder(matcher, status);
        case WEBSITE:
            return regexWebsite(matcher, status);
        case EMAIL:
            return regexEmail(matcher, status);
        case ACRONYMS:
            return regexAcronyms(matcher, status);
        case TEENCODE:
            return regexTeencode(matcher, status);
        default:
            cerr << "[E] Invalid category: " << categories << '\n';
    }
    return UnicodeString();
}
UnicodeString SpecialCase::regexFootballUnder(RegexMatcher* matcher, UErrorCode &status) {
    UnicodeString result;
    UnicodeString match = matcher->group(status);
    StringCharacterIterator iter(match);
    UnicodeString number;
    ConvertingNumber numberConverter;
    int u_offset = 'u' - 'a';
    for (auto c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32()) {
        if (c == LATIN_CAPITAL_LETTER_A + u_offset || c == LATIN_SMALL_LETTER_A + u_offset)
            result.append(LATIN_SMALL_LETTER_A + u_offset);
        else if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9) {
            number += c;
        } 
    }
    result += " " + numberConverter.convertNumber(number);
    return result;
}
UnicodeString SpecialCase::regexFootballOther(RegexMatcher* matcher, UErrorCode &status) {
    UnicodeString result;
    UnicodeString match = matcher->group(status);
    ConvertingNumber numberConverter;
    UnicodeString number;
    StringCharacterIterator iter(match);
    // Separating number such as 12 and 1-2
    
    bool continuousDigits = false; 
    int i = 0;
    for (auto c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32()) {
        if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9) {
            if (continuousDigits) {
                 // append digit to number
                number += c;
            } else {
                // start new number
                number = c;
                continuousDigits = true;
            }
        } else if (c == HYPEN_MINUS || c == VERTICAL_LINE) {
            if (continuousDigits) {
                // separator here
                continuousDigits = false; 
                result += numberConverter.convertNumber(number);
                number = UnicodeString();
            }
            result += " ";
        } else {
            if (continuousDigits) {
                // separator here
                continuousDigits = false;
                result += numberConverter.convertNumber(number);
                number = UnicodeString();
            }
            result += c;
        }
        i++;
    }
    result += numberConverter.convertNumber(number);
    return result;
}
UnicodeString SpecialCase::regexWebsite(RegexMatcher* matcher, UErrorCode &status) {
    ICUMapping letterSoundVN;
    string letterSoundVNFile = MAPPING_FOLDER + "/" + F_LETTER_SOUND_VN;
    letterSoundVN.loadMappingFile(letterSoundVNFile.data());
    letterSoundVNFile = MAPPING_FOLDER + "/" + F_SYMBOL;
    letterSoundVN.loadMappingFile(letterSoundVNFile.data());
    UnicodeString match = matcher->group(status);
    UnicodeString matchLowered(match);
    matchLowered.toLower();
    ConvertingNumber converter;
    auto indexOfCom = matchLowered.indexOf(".com");
    if(indexOfCom != -1)
        indexOfCom++;
    StringCharacterIterator iter(matchLowered);
    int32_t i = 0;
    UnicodeString result;
    for (auto c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32(), i++) {
        if (indexOfCom != -1 && i >= indexOfCom && i <= indexOfCom + 2) {
            result += c;
        } else {
            
            if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9 ) {
                result += " " + converter.convertNumber(c) + " ";
            }
            else if ( UnicodeString(c) == FULL_STOP ) {
                result += UnicodeString::fromUTF8(" chấm ");
            }
            else if ( UnicodeString(c) == SOLIDUS ){
                result += UnicodeString::fromUTF8(" xuyệt ");
            }
            else result += " "+letterSoundVN.mappingOf(UnicodeString(c))+" ";
        }
        
    }
    result = " " + result +  " ";
    return result;
}
UnicodeString SpecialCase::regexEmail(RegexMatcher* matcher, UErrorCode &status) {

    ICUMapping letterSoundEN;
    string letterSoundENFile = MAPPING_FOLDER + "/" + F_LETTER_SOUND_EN;
    letterSoundEN.loadMappingFile(letterSoundENFile.data());
    letterSoundENFile = MAPPING_FOLDER + "/" + F_SYMBOL;
    letterSoundEN.loadMappingFile(letterSoundENFile.data());
    UnicodeString match = matcher->group(status);
    //if (match.length()>35)
    //    return " địa chỉ email ";

    UnicodeString matchLowered(match);
    matchLowered.toLower();
    ConvertingNumber converter;

    auto indexOfCom = matchLowered.indexOf("gmail.com");

    if(indexOfCom != -1)
        indexOfCom++;

    StringCharacterIterator iter(matchLowered);
    int32_t i = 0;
    UnicodeString result;
    for (auto c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32(), i++) {
        if (indexOfCom != -1 && i >= indexOfCom) {
            result += UnicodeString::fromUTF8(" meo chấm com ");
            break;
        } else {
            
            if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9 ) {
                result += " " + converter.convertNumber(c) + " ";
            }
            else if ( UnicodeString(c) == FULL_STOP ) {
                result += UnicodeString::fromUTF8(" chấm ");
            }
            else if ( UnicodeString(c) == SOLIDUS ){
                result += UnicodeString::fromUTF8(" xuyệt ");
            }
            else result += " "+letterSoundEN.mappingOf(UnicodeString(c))+" ";
        }
        
    }
    result = " " + result +  " ";
    return result;
}
UnicodeString SpecialCase::regexPhoneNumber(RegexMatcher* matcher, UErrorCode &status) {
    UnicodeString result;
    UnicodeString match = matcher->group(status);
    StringCharacterIterator iter(match);
    ICUMapping number;
    string numberFile = MAPPING_FOLDER + "/" + F_NUMBER_SOUND;
    number.loadMappingFile(numberFile.data());
    UChar32 c;
    for (c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32()) {
        if (c == PLUS_SIGN)
            result.append(UnicodeString::fromUTF8("cộng "));
        else if (c == FULL_STOP || c == COLON || c == HYPEN_MINUS 
                || c == LEFT_PARENTHESIS || c == RIGHT_PARENTHESIS) {
            // ignore this case
        } else if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9) {
            result.append(number.mappingOf(UnicodeString(c)));
            // appending space after number
            result.append(" ");
        } else
            result.append(c);
    }
    return result;
}
UnicodeString SpecialCase::normalizeText(const UnicodeString &input) {
    UnicodeString result;
    UnicodeString preResult(input);
    for (auto& rule : patterns) {
        int categ = rule.first;
        auto regexPatterns = rule.second;
        for (auto& pattern : regexPatterns) {
            UErrorCode status = U_ZERO_ERROR;
            UParseError parseErr;
            RegexPattern *regexPattern = RegexPattern::compile(pattern, parseErr, status);
            if (U_FAILURE(status)) {
                fprintf(stderr, "[E] Error in pattern: \"%s\" at position %d\n",
                        u_errorName(status), parseErr.offset);
                delete regexPattern;
                return UnicodeString();
            }
            RegexMatcher *matcher = regexPattern->matcher(preResult, status);
            if (U_FAILURE(status)) {
                fprintf(stderr, "[E] Error in creating RegexMatcher: \"%s\"\n",
                        u_errorName(status));
                delete matcher;
                delete regexPattern;
                return UnicodeString();
            }
            while (matcher->find(status) && U_SUCCESS(status)) {
                matcher->appendReplacement(result," " +stringForReplace(categ, matcher, status)+ " ", status);
            }
            matcher->appendTail(result);
            delete matcher;
            delete regexPattern;
            preResult = result;
            result  = UnicodeString();
            
        }
    }
    return preResult;
}

UnicodeString SpecialCase::regexAcronyms(RegexMatcher* matcher, UErrorCode &status) {
    UnicodeString match = matcher->group(status);
    UnicodeString result;
    UnicodeString word, number;
    ConvertingNumber converter;
    bool continuousDigits = false;
    
    StringCharacterIterator iter(match);
    for (auto c = iter.first32(); c != StringCharacterIterator::DONE; c = iter.next32()) {
        if (DIGIT_ZERO <= c && c <= DIGIT_ZERO + 9) {
            if (continuousDigits) {
                number += c;
            } else {
                number = c;
                continuousDigits = true;
            }
        } else if (c == HYPEN_MINUS) {
            continuousDigits = false;
            // Add a space before the number
            result += word + " ";
            word = "";
        } else {
            if (continuousDigits) {
                continuousDigits = false;
                result += converter.convertNumber(number);
                number = UnicodeString();
            }
            word += c;
        }
    }
    if (word.length() > 0) {
        result += word + " ";
    }
    if (number.length() > 0) {
        result += converter.convertNumber(number);
    }
    // Collapse multiple spaces and trim
    result.findAndReplace("  ", " ");
    result.trim();
    return result;
}

UnicodeString SpecialCase::regexTeencode(RegexMatcher* matcher, UErrorCode &status) {
    UnicodeString match = matcher->group(status);
    match.toLower();
    
    // Simple teen code mapping
    if (match == "xmz") return UnicodeString::fromUTF8("xinh mắt zai");
    if (match == "xinh") return UnicodeString::fromUTF8("xinh");
    if (match == "dep") return UnicodeString::fromUTF8("đẹp");
    if (match == "ngu") return UnicodeString::fromUTF8("ngu");
    if (match == "vcl") return UnicodeString::fromUTF8("vãi cả lồn");
    if (match == "vcc") return UnicodeString::fromUTF8("vãi cả cứt");
    if (match == "dm") return UnicodeString::fromUTF8("địt mẹ");
    if (match == "clm") return UnicodeString::fromUTF8("cặc lồn mẹ");
    
    // If no specific mapping, return the original
    return match;
}

SpecialCase::SpecialCase() {
    loadPatterns(PHONE_NUMBER, F_PHONE_NUMBER);
    loadPatterns(FOOTBALL_UNDER, F_FOOTBALL_UNDER);
    loadPatterns(FOOTBALL_OTHER, F_FOOTBALL_OTHER);
    loadPatterns(WEBSITE, F_WEBSITE);
    loadPatterns(EMAIL, F_EMAIL);
    loadPatterns(ACRONYMS, F_ACRONYMS);
    loadPatterns(TEENCODE, F_TEENCODE);
}
SpecialCase::~SpecialCase() {
    u_cleanup();
}
