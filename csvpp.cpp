#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "/home/james/c/nix.c/nix.h"

#define FILENAME "/home/james/.local/bin/nix-db/db/upah-tukang_pryk-ns-lk_1.tsv"
#define FILE_STOCKS "/tmp/stocks-nix/stocks/Balancepos20221230.txt"
#define deb(THING) std::cout << THING << '\n'

#define THIS_EACH_KEY(VAR, VAR_LEN) \
	int VAR = 0, VAR_LEN = this->keysLen(); VAR < VAR_LEN; ++VAR

#define THIS_EACH_RECORD(VAR, VAR_LEN) \
	int VAR = 0, VAR_LEN = this->recordsLen(); VAR < VAR_LEN; ++VAR

#define THIS_EACH_VALUES(VAR, VAR_LEN, VAR_BEFORE) \
	int VAR = 0, VAR_LEN = this->records[VAR_BEFORE].values.size(); VAR < VAR_LEN; ++VAR

#define EACH_KEY(DATA, VAR, VAR_LEN) \
	int VAR = 0, VAR_LEN = DATA . keysLen(); VAR < VAR_LEN; ++VAR

#define EACH_RECORD(DATA, VAR, VAR_LEN) \
	int VAR = 0, VAR_LEN = DATA . recordsLen(); VAR < VAR_LEN; ++VAR

#define EACH_VALUES(DATA, VAR, VAR_LEN, VAR_BEFORE) \
	int VAR = 0, VAR_LEN = DATA . records[VAR_BEFORE].values.size(); VAR < VAR_LEN; ++VAR


namespace ccsv {
	class Data {
		public:
			class Record {
				public:
					std::vector<std::string> values;
			};

			const char *title;
			std::vector<std::string> keys;
			std::vector<Record> records;

			size_t recordsLen()
			{
				return this->records.size();
			}

			size_t keysLen()
			{
				return this->keys.size();
			}

			size_t iterKey(const char *key)
			{
				for (size_t i =0, j = this->keys.size(); i < j; ++i)
					if (!strcmp(this->keys[i].c_str(), key))
						return i;
				perror("");
				return 0;
			}

			void printKey(const char *key)
			{
				for (size_t i = 0, at = this->iterKey(key), vecLen = this->records[at].values.size(); i < vecLen; ++i)
					std::cout << this->records[i].values[at] << '\n';
			}

			void printKeys(char delim)
			{
				for (auto key : this->keys)
					std::cout << key << delim; 
				std::cout << '\n';
			}

			int valInRecord(const char *value, size_t i)
			{
				for (size_t j = 0, jLen = this->records[i].values.size(); j < jLen; ++j)
					if (!strcmp(this->records[i].values[j].c_str(), value))
						return 1;
				return 0;
			}

			size_t iterRecord(const char *value)
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					if (this->records[i].values.data()->find(value) != std::string::npos)
						return i;
				return 0;
			}

			int isEquity(int i)
			{
				if (!strcmp(this->records[i].values[iterKey("Type")].c_str(), "EQUITY"))
					return 1;
				return 0;
			}

			void printRecordsIfValue(const char *value, char delim)
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					if (this->valInRecord(value, i)) {
						if (isEquity(i)) {
							for (size_t j = 0, vecLen = this->records[i].values.size(); j < vecLen; ++j)
								std::cout << this->records[i].values[j] << delim;
							std::cout << '\n';
						}
					}
			}

			void printForeignVal()
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					if (isEquity(i))
						std::cout << this->records[i].values[this->iterKey("Code")] << ' ' << strtol(this->records[i].values[this->iterKey("Foreign Percent")].c_str(), NULL, 10) << ' ' << strtol(this->records[i].values[this->iterKey("Individual Percent")].c_str(), NULL, 10) << '\n';
			}

			void print(char delim)
			{
				for (size_t i = 0, iLen = this->records.size(); i < iLen; ++i) {
					for (size_t j = 0, jLen = this->records[i].values.size(); j < jLen; ++j)
						std::cout << this->records[i].values[j] << delim;
					std::cout << '\n';
				}
			}

			void correctKey()
			{
				this->keys.push_back("Total");
				this->keys.push_back("Individual Percent");
				this->keys.push_back("Foreign Percent");
				this->keys.push_back("Local Percent");
				for (int k = 0, kLen = this->keysLen(); k < kLen; ++k)
					if (this->keys[k].find("Total") != std::string::npos) {
						if (this->keys[k - 1].find("Local") != std::string::npos)
							this->keys[k].assign("Total Local");
						else
							this->keys[k].assign("Total Foreign");
					}
				for (int r = 0, rLen = this->records.size() - 1; r < rLen; ++r) {
					double localVal = strtod(this->records[r].values[this->iterKey("Total Local")].c_str(), NULL);
					double foreignVal = strtod(this->records[r].values[this->iterKey("Total Foreign")].c_str(), NULL);
					double individualVal = strtod(this->records[r].values[this->iterKey("Local ID")].c_str(), NULL);
					double totalVal = localVal + foreignVal;
					this->records[r].values.push_back(std::to_string(totalVal));
					if (localVal && foreignVal) {
						this->records[r].values.push_back(std::to_string(localVal / totalVal * 100));
						this->records[r].values.push_back(std::to_string(foreignVal / totalVal * 100));
						this->records[r].values.push_back(std::to_string(individualVal / totalVal * 100));
					} else {
						this->records[r].values.push_back("0");
						this->records[r].values.push_back("0");
						this->records[r].values.push_back("0");
					}
				}
			}

			int init(const char *filename, int delim)
			{
				do {
					size_t fileSize = nixSizeOfFile(filename);
					char *fileStr = (char *)malloc(nixSizeOfFile(filename) + 1);
					if (!nixCatFast(filename, fileSize, fileStr)) break;
					char *token;
					char delimPtr[] = {(char)delim, '\n'};
					char *savePtr = fileStr;
					size_t w;
					switch (delim) {
					case ' ':
						w = nixWcWordTilNl(fileStr);
						break;
					case '\t':
						w = nixWcWordTilNlTab(fileStr);
						break;
					case ',':
						w = nixWcWordTilNlComma(fileStr);
						break;
					case '.':
						w = nixWcWordTilNlDot(fileStr);
						break;
					case '|':
						w = nixWcWordTilNlPipe(fileStr);
						break;
					case '\'':
						w = nixWcWordTilNlQuote(fileStr);
						break;
					case '"':
						w = nixWcWordTilNlDoubleQuote(fileStr);
						break;
					default:
						continue;
					}
					this->keys.reserve(w);
					for (size_t i = 0, j = w; i < j; ++i)
						this->keys.push_back(strtok_r(savePtr, delimPtr, &savePtr));
					for (size_t line = 0; (token = strtok_r(savePtr, delimPtr, &savePtr)); ++line) {
						ccsv::Data::Record record;
						this->records.push_back(record);
						this->records[line].values.push_back(token);
						for (size_t i = 1; i < w && (token = strtok_r(savePtr, delimPtr, &savePtr)); ++i)
							this->records[line].values.push_back(token);
					}
					free(fileStr);
					this->correctKey();
					return 1;
				} while (0);
				perror("");
				return 0;
			}

	};
}

int main(int argc, char **argv)
{
	ccsv::Data data;
	data.init(FILE_STOCKS, '|');
	/* data.printKeys('|'); */
	data.printForeignVal();
	return 0;
}
