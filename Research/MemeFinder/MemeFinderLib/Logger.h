//--------------------------------------------------
// Entity: A Logger for recording operations and parameters to the databse
//
// @author: Wild Boar
//
// @date: 2021-10-25
//--------------------------------------------------

#pragma once

#include <sstream>
#include <iostream>
using namespace std;

#include <NVLib/StringUtils.h>

namespace NVL_App 
{
    class Logger
    {
    private:
        bool _enabled;
        stringstream _stream;
    public:
        struct Save 
        { 
            friend ostream& operator << (ostream& stream, const Save& value) 
            {
                return stream;
            } 
        };
        struct Color 
        {
            int _color;
            Color(int color) : _color(color) {}
            friend ostream& operator << (ostream& stream, const Color& value) 
            {
                stream << "\033[" << value._color << "m";
                return stream;
            }
        };
    public:
        Logger() 
        {
            _enabled = true;
        }
        void Enable() { _enabled = true; }
		void Disable() { _enabled = false; }
        template <typename T>
        Logger& operator << (T param) 
        { 
            if (_enabled) 
            {
                if (is_same<T, Save>::value) 
                {
                    _stream << "\033[0m";
                    auto message = _stream.str();
                    _stream = stringstream();	
                    auto dateString = NVLib::StringUtils::GetDateTimeString();
                    cout << "[" << dateString << "] " << message << endl;;
                }
                else _stream << param;
            } 
                
            return *this;            
        }
        inline bool GetEnabled() { return _enabled; }
    };
}