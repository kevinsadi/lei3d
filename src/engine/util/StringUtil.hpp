#pragma once

#include <istream>
#include <sstream>
#include <string>

namespace lei3d
{
    inline bool strContains(const std::string& str, std::string val) {
        return str.find(val) != std::string::npos; 
    }

    inline bool strContains(const std::string& str, char c) {
        return str.find(c) != std::string::npos; 
    }

    inline std::string strTrim(const std::string& str,
        const std::string& whitespace = " \t")
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos)
            return ""; // no content

        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }

    inline std::vector<std::string> tokenize(std::string line, char delimiter = ' ')
    {
        std::vector<std::string> tokens;

        std::stringstream stream(line);

        std::string token;

        // Tokenizing w.r.t. space ' '
        while (std::getline(stream, token, delimiter))
        {
            if (token.length() < 1)
            {
                continue;
            }

            tokens.push_back(token);
        }

        return tokens;
    }

    /*
    * Parses an array starting with '[' and ending with ']' with comma separated elements
    *
    * Pass in stream when getline would get the line AFTER the one with the '['
    * Pass in the first line with the '[' as the second argument
    * Returns all the elements in string format up until the first ']'
    */
    inline std::vector<std::string> streamArrayParse(std::istream& stream, std::string firstLine)
    {
        std::vector<std::string> elements;

        auto tokens = tokenize(firstLine, ',');
        int tokensI = 0;
        while (tokensI < tokens.size() && tokens[tokensI].find('[') == std::string::npos)
        {
            tokensI++;
        }

        while (tokensI < tokens.size())
        {
            const std::string& token = tokens[tokensI];
            //Remove open brace if it is part of the token
            const size_t openPos = token.find('[');            //Remove open brace if it is part of the token
            if (openPos != std::string::npos)
            {
                const size_t startI = openPos + 1;
                std::string tokenWithoutBrace = token.substr(startI, token.length() - startI);
                if (tokenWithoutBrace.length() < 1)
                {
                    break;
                }

                elements.push_back(strTrim(tokenWithoutBrace));
            }
            else
            {
                elements.push_back(strTrim(token));
            }

            tokensI++;
        }

        std::string line;
        while (stream.good() && std::getline(stream, line))
        {
            auto tokens = tokenize(line, ',');
            bool foundClose = false;
            for (const std::string& token : tokens)
            {
                if (token.find(']') != std::string::npos)
                {
                    foundClose = true;
                    break;
                }

                //LEI_INFO("Found Face Path: {0}", facePath);
                elements.push_back(strTrim(token));
            }

            if (foundClose)
            {
                break;
            }
        }

        return elements;
    }
}