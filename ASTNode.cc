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

std::shared_ptr<ASTNode> ASTNode::MakeCaseLabelRangeNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLabelRangeNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeRepeatStatementNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeForStatementNode(
                                            unsigned int line, 
                                            unsigned int col,
                                            std::string literalText,
                                            std::shared_ptr<ASTNode> left, 
                                            std::shared_ptr<ASTNode> right,
                                            std::shared_ptr<ASTNode> next,
                                            std::shared_ptr<ASTNode> seq) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::WithStatementNode(
                                            unsigned int line, 
                                            unsigned int col,
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> GuardNodes,
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> StatementBlockNodes,
                                            std::shared_ptr<ASTNode> elsePart) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::GuardNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeLoopStatementNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeExitStatementNode(unsigned int line, unsigned int col) {
    return std::make_shared<ASTNode>(line, col);   
}

std::shared_ptr<ASTNode> ASTNode::MakeReturnStatementNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col); 
}

std::shared_ptr<ASTNode> ASTNode::MakeFormalParametersNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeFPSectionNode(
                                            unsigned int line, 
                                            unsigned int col, 
                                            std::shared_ptr<std::vector<std::string>> nodes,
                                            std::shared_ptr<ASTNode> right,
                                            bool isVar,
                                            bool isIn) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeReciverNode(unsigned int line, unsigned int col, std::string left, std::string right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeProcedureHeading(
                                            unsigned int line, 
                                            unsigned int col, 
                                            bool isProc, 
                                            std::shared_ptr<ASTNode> reciver, 
                                            std::shared_ptr<ASTNode> name, 
                                            std::shared_ptr<ASTNode> parameters) {
                                                return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeProcedureDeclarationNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right, std::string name) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeProcedureBodyNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
    return std::make_shared<ASTNode>(line, col);
}

std::shared_ptr<ASTNode> ASTNode::MakeSetNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes) {
    return std::make_shared<ASTNode>(line, col);
}