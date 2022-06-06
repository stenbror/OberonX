#include "ASTNode.h"

ASTNode::ASTNode(unsigned int line, unsigned col) {
    m_Line = line; m_Col = col;
}

std::shared_ptr<ASTNode> ASTNode::MakeIdentDefNode(unsigned int line, unsigned int col, std::string text, bool isreadOnlyExport, bool isExport) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeIdentNode(unsigned int line, unsigned int col, std::string text) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeQualidentNode(unsigned int line, unsigned int col, std::string text1, std::string text2) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeAssignmentNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeProcedureCallNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeModuleNode(
                    unsigned int line, 
                    unsigned int col, 
                    std::string moduleText, std::shared_ptr<ASTNode> left, 
                    std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes, 
                    std::shared_ptr<ASTNode> right) {
                        return std::make_shared<ASTNode>(line, col);
                    }

std::shared_ptr<ASTNode> ASTNode::MakeDeclarationSequence2Node(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);   
}

std::shared_ptr<ASTNode> ASTNode::MakeDeclarationNode(unsigned int line, unsigned int col, std::string name, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);   
}

std::shared_ptr<ASTNode> ASTNode::MakeImportListNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col); 
}