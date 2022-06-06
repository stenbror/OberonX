
#include <memory>
#include <string>

#pragma once

class ASTNode
{
    public:
        ASTNode(unsigned int line, unsigned col);


        static std::shared_ptr<ASTNode> MakeIdentDefNode(unsigned int line, unsigned int col, std::string text, bool isreadOnlyExport, bool isExport);

    private:
        unsigned int m_Line;
        unsigned int m_Col;
};