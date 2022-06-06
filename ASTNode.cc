#include "ASTNode.h"

ASTNode::ASTNode(unsigned int line, unsigned col) {
    m_Line = line; m_Col = col;
}

std::shared_ptr<ASTNode> ASTNode::MakeIdentDefNode(unsigned int line, unsigned int col, std::string text, bool isreadOnlyExport, bool isExport) {
    return std::make_shared<ASTNode>(line, col);
}