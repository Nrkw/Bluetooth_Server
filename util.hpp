#pragma once
#include <vector>
#include <string>
#include <cmath>

namespace util
{
    std::vector<std::string> split_naive(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::string item;
        for (char ch: s) {
            if (ch == delim) {
                if (!item.empty())
                    elems.push_back(item);
                item.clear();
            }
            else {
                item += ch;
            }
        }
        if (!item.empty())
            elems.push_back(item);
        return elems;
    }
    #include <cmath>

    /* 小数点n以下で四捨五入する */
    float round_n(float number, double n)
    {
        number = number * pow(10,n-1); //四捨五入したい値を10の(n-1)乗倍する。
        number = round(number); //小数点以下を四捨五入する。
        number /= pow(10, n-1); //10の(n-1)乗で割る。
        return number;
    }

    bool canConvertToint(const std::string &s)
    {
        try
        {
            std::stoi(s);
            return true;
        }catch(const std::exception &e)
        {
            return false;
        }
    }

    /*
        std::string separate_at_semicolon()
        x,0.354,y,0.353,z,0.243;attitudeといった具合の文字列が来ていたら、
        セミコロン以前の文字だけ切り取って返す
    */
    std::string separate_at_semicolon(std::string str)
    {
        std::string result = "";
        for(auto ch : str)
        {
            if(ch == ';')
            {
                break;
            }
            result += ch;
        }

        return result;
    }

    bool isIncludeChar(std::string str, char ch)
    {
        for(auto letter : str)
        {
            if(letter == ch)
            {
                return true;
            }
        }
        return false;
    }
    bool isIncludeStr(std::string str, std::string search_str)
    {
        if (str.find(search_str) != std::string::npos) 
        {
            return true;
        }
        return false;
    }
}