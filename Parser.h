
#include "Tokenizer.h"
#include "ASTNode.h"

#include <memory>
#include <string>
#include <sstream>


class SyntaxError {
   public:
        SyntaxError(unsigned int line, unsigned int col, std::string text);
        std::string GetExceptionDetails();

    private:
        unsigned int m_Line;
        unsigned int m_Col;
        std::string m_Text;
};

class Parser
{
    public:
        Parser(std::shared_ptr<Tokenizer> lexer);

        std::shared_ptr<ASTNode> ParseOberon();

    private:
        std::shared_ptr<ASTNode> ParseQualident();
        std::shared_ptr<ASTNode> ParseIdentDef();
        std::shared_ptr<ASTNode> ParseConstDeclaration();
        std::shared_ptr<ASTNode> ParseConstExpression();
        std::shared_ptr<ASTNode> ParseTypeDeclaration();
        std::shared_ptr<ASTNode> ParseType();
        std::shared_ptr<ASTNode> ParseNamedType();
        std::shared_ptr<ASTNode> ParseTypeParams();
        std::shared_ptr<ASTNode> ParseTypeActuals();
        std::shared_ptr<ASTNode> ParseEnumeration();
        std::shared_ptr<ASTNode> ParseArrayType();
        std::shared_ptr<ASTNode> ParseLengthList();
        std::shared_ptr<ASTNode> ParseLength();
        std::shared_ptr<ASTNode> ParseVarLength();
        std::shared_ptr<ASTNode> ParserecordType();
        std::shared_ptr<ASTNode> ParseBaseType();
        std::shared_ptr<ASTNode> ParseFieldListSequence();
        std::shared_ptr<ASTNode> ParseFieldList();
        std::shared_ptr<ASTNode> ParseIdentList();
        std::shared_ptr<ASTNode> ParsePointerType();
        std::shared_ptr<ASTNode> ParseProcedureType();
        std::shared_ptr<ASTNode> ParseVariableDeclararation();
        std::shared_ptr<ASTNode> ParseDesignator();
        std::shared_ptr<ASTNode> ParseSelector();
        std::shared_ptr<ASTNode> ParseExpList();
        std::shared_ptr<ASTNode> ParseExpression();
        std::shared_ptr<ASTNode> ParseSimpleExpression();
        std::shared_ptr<ASTNode> ParseTerm();
        std::shared_ptr<ASTNode> ParseLiteral();
        std::shared_ptr<ASTNode> ParseFactor();
        std::shared_ptr<ASTNode> ParseSet();
        std::shared_ptr<ASTNode> ParseElement();
        std::shared_ptr<ASTNode> ParseActualParameters();
        std::shared_ptr<ASTNode> ParseStatement();
        std::shared_ptr<ASTNode> ParseAssignment(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left);
        std::shared_ptr<ASTNode> ParseProcedureCall(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left);
        std::shared_ptr<ASTNode> ParseStatementSequence();
        std::shared_ptr<ASTNode> ParseIfStatement();
        std::shared_ptr<ASTNode> ParseElsifStatement();
        std::shared_ptr<ASTNode> ParseElseStatement();
        std::shared_ptr<ASTNode> ParseCaseStatement();
        std::shared_ptr<ASTNode> ParseCase();
        std::shared_ptr<ASTNode> ParseCaseLabelList();
        std::shared_ptr<ASTNode> ParseLabelRange();
        std::shared_ptr<ASTNode> ParseWhileStatement();
        std::shared_ptr<ASTNode> ParseElsifStatement2();
        std::shared_ptr<ASTNode> ParseRepeatStatement();
        std::shared_ptr<ASTNode> ParseForStatement();
        std::shared_ptr<ASTNode> ParseWithStatement();
        std::shared_ptr<ASTNode> ParseGuard();
        std::shared_ptr<ASTNode> ParseLoopStatement();
        std::shared_ptr<ASTNode> ParseExitStatement();
        std::shared_ptr<ASTNode> ParseProcedureDeclaration();
        std::shared_ptr<ASTNode> ParseProcedureHeading();
        std::shared_ptr<ASTNode> ParseReciver();
        std::shared_ptr<ASTNode> ParseProcedureBody();
        std::shared_ptr<ASTNode> ParseDeclarationSequence(bool isDefinition = true);
        std::shared_ptr<ASTNode> ParseReturnStatement();
        std::shared_ptr<ASTNode> ParseFormalParameters();
        std::shared_ptr<ASTNode> ParseReturnType();
        std::shared_ptr<ASTNode> ParseFPSection();
        std::shared_ptr<ASTNode> ParseFormalType();
        std::shared_ptr<ASTNode> ParseModule();
        std::shared_ptr<ASTNode> ParseImportList();
        std::shared_ptr<ASTNode> ParseImport();
        std::shared_ptr<ASTNode> ParseDefinition();

        void CheckSymbol(TokenCode symbol, std::string msg);
        void CheckSymbolAndAdvance(TokenCode symbol, std::string msg);

    private:
        std::shared_ptr<Tokenizer> m_Lexer;

};
