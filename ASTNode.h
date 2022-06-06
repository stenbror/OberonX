
#include <memory>
#include <string>
#include <vector>


#pragma once

class ASTNode
{
    public:
        ASTNode(unsigned int line, unsigned col);


        static std::shared_ptr<ASTNode> MakeIdentDefNode(unsigned int line, unsigned int col, std::string text, bool isreadOnlyExport, bool isExport);
        static std::shared_ptr<ASTNode> MakeIdentNode(unsigned int line, unsigned int col, std::string text);
        static std::shared_ptr<ASTNode> MakeQualidentNode(unsigned int line, unsigned int col, std::string text1, std::string text2);
        static std::shared_ptr<ASTNode> MakeAssignmentNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
        static std::shared_ptr<ASTNode> MakeProcedureCallNode(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
        static std::shared_ptr<ASTNode> MakeModuleNode(
                    unsigned int line, 
                    unsigned int col, 
                    std::string moduleText, std::shared_ptr<ASTNode> left, 
                    std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes, 
                    std::shared_ptr<ASTNode> right);
        static std::shared_ptr<ASTNode> MakeDeclarationSequence2Node(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes);
        static std::shared_ptr<ASTNode> MakeDeclarationNode(unsigned int line, unsigned int col, std::string name, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
        static std::shared_ptr<ASTNode> MakeImportListNode(unsigned int line, unsigned int col, std::shared_ptr<std::vector<std::shared_ptr<ASTNode>>> nodes);
        static std::shared_ptr<ASTNode> MakeImportAssignPathNode(unsigned int line, unsigned int col, std::string left, std::string right, std::string next, std::shared_ptr<ASTNode> last);
        static std::shared_ptr<ASTNode> MakeImportAssignNode(unsigned int line, unsigned int col, std::string left, std::string right, std::shared_ptr<ASTNode> next);
        static std::shared_ptr<ASTNode> MakeImportPathNode(unsigned int line, unsigned int col, std::string left, std::string right, std::shared_ptr<ASTNode> next);
        static std::shared_ptr<ASTNode> MakeImportNode(unsigned int line, unsigned int col, std::string left, std::shared_ptr<ASTNode> right);

    private:
        unsigned int m_Line;
        unsigned int m_Col;
};