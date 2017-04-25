// WordChecker.cpp
//
// ICS 46 Spring 2015
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    // Call the contains function from the words class
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> suggestions;
    bool isContained;
    // Swapping each adacent pair of characters in the word.
    for (int j1 = 0; j1 < word.length()-1; j1++)
    {
        std::string str1 = "";
        for (int k1 = 0; k1 < word.length(); k1++)
        {
            if (j1 == k1)
            {
                str1 += word[k1+1];
            }
            else if (j1+1 == k1)
            {
                str1 += word[k1-1];
            }
            else
            {
                str1 += word[k1];
            }
        }
        isContained = false;
        if (words.contains(str1))
        {
            for (int l1 = 0; l1 < suggestions.size(); l1++)
            {
                if (str1 == suggestions[l1])
                {
                    isContained = true;
                }
            }
            if(!isContained)
            {
                suggestions.push_back(str1);
            }
        }
    }

    // In between each adfacent pair of Characters in the word
    for (int j2 = 0; j2 < word.length()+1; j2++)
    {
        for (int k2 = 65; k2 <= 90; k2++)
        {
            std::string str2 = "";
            char c = k2;
            for (int l2 = 0; l2 < word.length(); l2++)
            {
                if (j2 == l2)
                {
                    str2 += c;
                }
                str2 += word[l2];
            }
            isContained = false;
            if (words.contains(str2))
            {
                for(int i2 = 0; i2 < suggestions.size(); i2++)
                {
                    if (str2 == suggestions[i2])
                    {
                        isContained = true;
                    }
                }
                if (!isContained)
                {
                    suggestions.push_back(str2);
                }
            }
        }
    }

    // deleting each char from the word
    for (int j3 = 0; j3 < word.length(); j3++)
    {
        std::string str3 = "";
        for (int k3 = 0; k3 < word.length(); k3++)
        {
            if (j3 != k3)
            {
                str3 += word[k3];
            }
        }
        isContained = false;
        if (words.contains(str3))
        {
            for (int l3 = 0; l3 < suggestions.size(); l3++)
            {
                if (str3 == suggestions[l3])
                {
                    isContained = true;
                }
            }
            if (!isContained)
            {
                suggestions.push_back(str3);
            }
        }
    }

    // replacing each character in the word
    for (int j4 = 0; j4 < word.length(); j4++)
    {
        for (int k4 = 65; k4 <= 90; k4++)
        {
            std::string str4 = word;
            str4[j4] = k4;
            isContained = false;
            if (words.contains(str4))
            {
                for (int l4 = 0; l4 < suggestions.size(); l4++)
                {
                    if(str4 == suggestions[l4])
                    {
                        isContained = true;
                    }
                }
                if (!isContained)
                {
                    suggestions.push_back(str4);
                }
            }
        }
    }

    // splitting the word into a pair of words by adding a space
    for (int j5 = 0; j5 < word.length()-1; j5++)
    {
        std::string str5 = "";
        for (int k5 = 0; k5 < word.length(); k5++)
        {
            if (k5 == j5+1)
            {
                str5 += " ";
            }
            str5 += word[k5];
        }
        isContained = false;
        if (words.contains(str5))
        {
            for (int l5 = 0; l5 < suggestions.size(); l5++)
            {
                if (str5 == suggestions[l5])
                {
                    isContained = true;
                }
            }
            if (!isContained)
            {
                suggestions.push_back(str5);
            }
        }
    }

    return suggestions;
}


