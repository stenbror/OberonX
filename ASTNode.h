
#include <memory>
#include <string>


#pragma once

class ASTNode
{
    public:
        ASTNode(unsigned int line, unsigned col);


        static std::shared_ptr<ASTNode> MakeIdentDefNode(unsigned int line, unsigned int col, std::string text, bool isreadOnlyExport, bool isExport);
        static std::shared_ptr<ASTNode> MakeIdentNode(unsigned int line, unsigned int col, std::string text);
        static std::shared_ptr<ASTNode> MakeQualidentNode(unsigned int line, unsigned int col, std::string text1, std::string text2);

    private:
        unsigned int m_Line;
        unsigned int m_Col;
};