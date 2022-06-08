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

std::shared_ptr<ASTNode> ASTNode::MakeImportAssignPathNode(unsigned int line, unsigned int col, std::string left, std::string right, std::string next, std::shared_ptr<ASTNode> last) {
    return std::make_shared<ASTNode>(line, col); 
}

std::shared_ptr<ASTNode> ASTNode::MakeImportAssignNode(unsigned int line, unsigned int col, std::string left, std::string right, std::shared_ptr<ASTNode> next) {
    return std::make_shared<ASTNode>(line, col); 
}

std::shared_ptr<ASTNode> ASTNode::MakeImportPathNode(unsigned int line, unsigned int col, std::string left, std::string right, std::shared_ptr<ASTNode> next) {
    return std::make_shared<ASTNode>(line, col); 
}

std::shared_ptr<ASTNode> ASTNode::MakeImportNode(unsigned int line, unsigned int col, std::string left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col); 
}

std::shared_ptr<ASTNode> ASTNode::MakeLessCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLessEqualCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeEqualCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeGreaterEqualCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeGreaterCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeNotEqualCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeInCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeIsCompareNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeUnaryPlusNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeUnaryMinusNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakePlusNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeMinusNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeOrNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeMulNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeSlashNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeDivNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeModNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeAndNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralNumberNode(unsigned int line, unsigned int col, std::string text) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralStringNode(unsigned int line, unsigned int col, std::string text) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralHexStringNode(unsigned int line, unsigned int col, std::string text) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralHexCharNode(unsigned int line, unsigned int col, std::string text) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralNilNode(unsigned int line, unsigned int col) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralTrueNode(unsigned int line, unsigned int col) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLiteralFalseNode(unsigned int line, unsigned int col) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeCallNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeBitInvertNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeExpressionListNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeStatementSequenceNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeIfStatementNode(
                                        unsigned int line, 
                                        unsigned int col, 
                                        std::shared_ptr<ASTNode> left, 
                                        std::shared_ptr<ASTNode> right, 
                                        std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes,
                                        std::shared_ptr<ASTNode> next) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeElsifStatementNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
     return std::make_shared<ASTNode>(line, col);
}
std::shared_ptr<ASTNode> ASTNode::MakeElseStatementNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
     return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeWhileStatementNode(
                                            unsigned int line, 
                                            unsigned int col, 
                                            std::shared_ptr<ASTNode> left, 
                                            std::shared_ptr<ASTNode> right, 
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeCaseStatementNode(
                                            unsigned int line, 
                                            unsigned int col, 
                                            std::shared_ptr<ASTNode> left, 
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes, 
                                            std::shared_ptr<ASTNode> right) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeCaseStatement(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}
