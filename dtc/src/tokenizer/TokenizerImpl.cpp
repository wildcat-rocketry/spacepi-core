#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>
#include <spacepi/dtc/tokenizer/TokenizerImpl.hpp>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>

using namespace std;
using namespace spacepi::dtc::tokenizer;
using namespace spacepi::dtc::diagnostics;

bool TokenizerImpl::readFile(const string &filename, vector<Token> &tokens) noexcept {
    // TODO
	const vector<string> lines;
	const shared_ptr<SourceFile> inFile(new SourceFile(filename, lines));
	//int currPos[] = {1, 1};
	int lineNo = 1;
	int colNo = 1;
	char temp;
	int tokenBegin;
	bool alreadyASpace;
	
	ifstream str(filename, ifstream::in);
	
	try {
		while (true) {
			char s[255];
			str.getline(s, 255);
			lines.push_back(s);
		}
	}
	catch (ios_base::failure&) {
		char s[255];
		str.getline(s, 255, EOF);
		lines.push_back(s);
	}
	
	enum tokenStates {NORMAL, COMMENT_BEGIN_WAIT, LINE_COMMENT, BLOCK_COMMENT, COMMENT_END_WAIT, STRING, INVALID,
						IGNORE, SINGLE} tokenState = NORMAL;
						
	// DiagnosticReporter.instance.warn(sourceLocation)  // Replace every instance of "cout" with this after testing.
	
	string invalid("!$%'()*\\^`|~");
	string singleTokens(":;<>[]{}");
	//bool isString = false;
	for (auto it = begin(lines); it != end(lines); it++) {
		colNo = 1;
		do {
			temp = (*it)[colNo - 1];  // B'cuz strings are zero-indexed.
			switch (tokenState) {
				case NORMAL:
					if (temp == '/') tokenState = COMMENT_BEGIN_WAIT;
					else if (temp == '\"') {
						tokenState = STRING;
						tokenBegin = colNo;
						alreadyASpace = true;
					}
					else if (invalid.find(temp) != string::npos || temp > '~')
						cout << "Warning: skipped invalid character \'" << temp << "\' at line " << lineNo << endl;
					else if (temp <= ' ') {
						//tokenState = IGNORE;
						if (!alreadyASpace) {  // I will only do the following if temp is the first space (or whatever) I've seen since the last solid block that isn't already tokenized.
							// Not adding one to (colNo - tokenBegin) because I don't want the current character to be included. tokenBegin on the left gets one subtracted because it is one-indexed.
							tokens.emplace_back(SourceLocation(inFile, lineNo, tokenBegin, colNo), it->substr(tokenBegin - 1, colNo - tokenBegin));
						}
						if (temp == '\0') {
							tokenBegin = 1;
							alreadyASpace = true;
						}
						else {
							tokenBegin = colNo + 1;
							alreadyASpace = true;
						}
					}
					else if (singleTokens.find(temp) != string::npos) {
						tokens.emplace_back(SourceLocation(inFile, lineNo, colNo, colNo), temp);
						alreadyASpace = false;
					}
					
					break;
				case COMMENT_BEGIN_WAIT:
					if (temp == '/') tokenState = LINE_COMMENT;
					else if (temp == '*') tokenState = BLOCK_COMMENT;
					else {
						tokenState = NORMAL;
						cout << "Warning: skipped invalid singular character \'/\' at line " << lineNo << endl;
					}
					break;
				case LINE_COMMENT:
					if (temp == '\0') {
						tokenState = NORMAL;
						tokenBegin = 1;
					}
					break;
				case BLOCK_COMMENT:
					if (temp == '*') tokenState = COMMENT_END_WAIT;
					break;
				case COMMENT_END_WAIT:
					if (temp == '/') {
						tokenState = NORMAL;
						tokenBegin = colNo + 1;
					}
					break;
				case STRING:
					if (temp == '\"') {
						tokenState = NORMAL;
						tokens.emplace_back(SourceLocation(inFile, lineNo, tokenBegin, colNo), it->substr(tokenBegin - 1, colNo - tokenBegin + 1));
					}
					break;
				
				/*   // Uhh whoops, this will always discard the character following the invalid one, whether I want it to or not.
				
				case (INVALID):
					if (invalid.find(temp) == string::npos && temp <= '~') tokenState = NORMAL;
					else {
						DiagnosticReporter.instance.warn(sourceLocation) << "Warning: skipped invalid character \'" << temp << "\' at line " << lineNo << endl;
						continue;
					}
					break;
				
				
				// So will this.
				
				case IGNORE:
					if (temp > ' ') tokenState = NORMAL;
					break;
				*/
				
				/*  // This just won't work.
				case SINGLE:
					tokens.emplace_back(SourceLocation(inFile, lineNo, colNo, colNo), string(temp));
					break;
				*/
			}
			colNo++;
		} while (temp != '\0');
		
		lineNo++;
		// State actions.
		/*
		switch (tokenState) {
			case NORMAL:
				break;
			case COMMENT_BEGIN_WAIT:
				break;
			case LINE_COMMENT:
				break;
			case BLOCK_COMMENT:
				break;
			case STRING:
				
		}
		*/
		colNo++;
		if (temp == '\0') {
			colNo = 1;
			lineNo++;
		}
	}
    return true;
}
