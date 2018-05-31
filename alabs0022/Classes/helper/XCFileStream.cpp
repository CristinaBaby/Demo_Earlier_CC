//
//  XCFileStream.cpp
//  ColorGarden
//
//  Created by houhuang on 16/3/11.
//
//
#include <fstream>
#include <algorithm>
#include "XCFileStream.h"


CXCFileStream::CXCFileStream()
{
}


CXCFileStream::~CXCFileStream(void)
{
}

//
//const bool CXCFileStream::IsCsvFile(const string& lpszFilename)
//{
//    /// 0、判断文件名是否有效
//    if (lpszFilename.empty() || 4 > strlen(lpszFilename))
//        return false;
//    /// 本地变量
//    string _strFileName(lpszFilename);
//    size_t _iLen = _strFileName.length();
//    string _strSuff(_strFileName.substr(_iLen - 4, 4));
//    /// 转变为小写，如果要转变为大写：tolower -> toupper 。
//    std::s
//    
//    /// 1、判断是否是CSV文件
//    return (0 == _strSuff.compare(".csv"));
//}
//
//
//const int CXCFileStream::ReadCsvData(LPCTSTR lpszFilename, vector<list<string> > &vlStr)
//{
//    /// 1、判断是否是CSV文件
//    if (! IsCsvFile(lpszFilename))
//        return XC_ERR_INVALID_FILE_NAME;
//    /// 2、打开CSV文件
//    ifstream _streamFromFile(lpszFilename);
//    ///    判断打开文件是否成功
//    if (NULL == _streamFromFile)
//        return (-errno);
//    /// 存储读取的文件内容
//    string _strIn("");
//    /// 3、读取一行
//    while (getline(_streamFromFile, _strIn)) {
//        /// 每行的源字符串
//        LPCTSTR _pcSrc = _strIn.c_str();
//        /// 存储一行‘,'分隔解析后的各个元素
//        list<string> _ltStr;
//        /// Parse values in this line
//        while (*_pcSrc != '\0') {
//            /// string to hold this value
//            string _strElem("");
//            /// 针对每个字符分析
//            if (*_pcSrc == '"') {
//                /// Bump past opening quote
//                _pcSrc++;
//                /// Parse quoted value
//                while (*_pcSrc != '\0') {
//                    /// Test for quote character
//                    if (*_pcSrc == '"') {
//                        /// Found one quote
//                        _pcSrc++;
//                        // If pair of quotes, keep one
//                        // Else interpret as end of value
//                        if (*_pcSrc != '"') {
//                            _pcSrc++;
//                            break;
//                        }
//                    }
//                    /// Add this character to value
//                    _strElem.push_back(*_pcSrc++);
//                }
//            }
//            else {
//                // Parse unquoted value
//                while (*_pcSrc != '\0' && *_pcSrc != ',')
//                    _strElem.push_back(*_pcSrc++);
//                // Advance to next character (if not already end of string)
//                if (*_pcSrc != '\0')
//                    _pcSrc++;
//            }
//            /// Add this string to container
//            _ltStr.push_back(_strElem);
//        }
//        /// 分析后的一行文件内容所得的元素列表添加到容器中
//        vlStr.push_back(_ltStr);
//        /// 归零，防止下次分析旧的数据。
//        _strIn.assign("");
//    }
//    
//    
//    return XC_ERR_NONE;
//}
//

const int CXCFileStream::WriteCsvData(const string& lpszFilename, const vector<list<string> > &vlStr)
{
    /// 1、判断是否是CSV文件
//    if (! IsCsvFile(lpszFilename))
//        return XC_ERR_INVALID_FILE_NAME;
    /// 2、打开CSV文件
    ofstream _streamToFile(lpszFilename);
    ///    判断打开文件是否成功
//    if (!_streamToFile.is_open())
//        return (-errno);
    /// 本地变量
    static char chQuote = '"';
    static char chComma = ',';
    /// Loop through each list of string in vector
    for (vector<list<string> >::const_iterator vIt = vlStr.begin(); vIt != vlStr.end(); vIt ++) {
        /// Loop through each string in list 
        for (list<string>::const_iterator lIt = vIt->begin(); lIt != vIt->end(); lIt ++) {
            /// Separate this value from previous
            if (vIt->begin() != lIt)
                _streamToFile.put(chComma);
            /// 考虑string中可能有,或"的情况，这就要特殊包装。
            bool bComma = (lIt->find(chComma) != lIt->npos);
            bool bQuote = (lIt->find(chQuote) != lIt->npos);
            /// 真的含有,或"的情况
            if (bComma || bQuote) {
                _streamToFile.put(chQuote);
                
                
                if (bQuote) {
                    for (string::const_iterator chIt = lIt->begin(); chIt != lIt->end(); chIt ++ ) {
                        // Pairs of quotes interpreted as single quote
                        if (chQuote == *chIt) 
                            _streamToFile.put(chQuote);
                        
                        
                        _streamToFile.put(*chIt);
                    }
                }
                else 
                    _streamToFile << *lIt;
                
                
                _streamToFile.put(chQuote);
            }
            else
                _streamToFile << *lIt;
        }
        /// 换行
        _streamToFile << endl;
    }
    /// 
    return XC_ERR_NONE;
}