#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <assert.h>
#include <getopt.h>
#include <stdio.h>

class CutterSettings
{
private:
    std::string _delimiter = "\0";
    std::vector<int> _fields;

public:
    CutterSettings() {}
    ~CutterSettings() = default;

    std::string delimiter() const { return _delimiter; }
    std::vector<int> fields() const { return _fields; }

    void delimiter(std::string delimiter) { _delimiter = delimiter; }
    void fields(std::vector<int> fields) { _fields = fields; }
};

class Cutter
{
private:
    CutterSettings settings;

public:
    Cutter() {}
    ~Cutter() = default;

    void cut(std::istream &input)
    {
        using namespace std;

        regex reg{settings.delimiter()};
        auto requestedFields = settings.fields();
        sregex_token_iterator end;

        string line;
        while (getline(input, line))
        {
            sregex_token_iterator d(line.begin(), line.end(), reg, -1);
            for (int i = 1; d != end; d++, i++)
            {
                if (find(requestedFields.begin(), requestedFields.end(), i) !=
                    requestedFields.end())
                {
                    cout << *d << " ";
                }
            }
            cout << endl;
        }
    }

    void setSettings(CutterSettings &settings) { this->settings = settings; }
};

/**
 *  Expands the string provided as an argument into a vector of integers
 *
 *  E.g., original = "1,2,4-6", output vector = {1,2,4,5,6}
 */
std::vector<int> generateFieldsList(const std::string &original)
{
    using namespace std;

    vector<int> result;
    regex regexComma{","};
    regex regexDash{"\\-"};
    sregex_token_iterator fields(original.begin(), original.end(), regexComma, -1);
    sregex_token_iterator end;
    for (; fields != end; fields++)
    {
        string field = fields->str();
        if (field.find("-") != string::npos)
        {
            sregex_token_iterator boundaries(field.begin(), field.end(), regexDash, -1);
            int lower = stoi(*(boundaries++));
            int upper = stoi(*(boundaries++));
            for (int i = lower; i <= upper; i++)
            {
                result.push_back(i);
            }
        }
        else
        {
            result.push_back(stoi(field));
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    Cutter cutter;
    CutterSettings settings;

    int optc;
    while ((optc = getopt(argc, argv, "d:f:")) != -1)
    {
        switch (optc)
        {
        case 'f':
            settings.fields(std::move(generateFieldsList(std::string(optarg))));
            break;
        case 'd':
            settings.delimiter(std::string(optarg));
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    cutter.setSettings(settings);

    // Read either from stdin or from the list of provided files
    if (optind == argc)
    {
        cutter.cut(std::cin);
    }
    else
    {
        for (; optind < argc; optind++)
        {
            auto input = std::ifstream(argv[optind]);
            if (input)
                cutter.cut(input);
        }
    }

    return EXIT_SUCCESS;
}
