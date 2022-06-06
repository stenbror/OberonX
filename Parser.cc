#include "Parser.h"
#include "ASTNode.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Exception:  SyntaxError ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

SyntaxError::SyntaxError(unsigned int line, unsigned int col, std::string text) {
    m_Line = line; m_Col = col; m_Text = text;
}

std::string SyntaxError::GetExceptionDetails() {
    std::ostringstream ss;
    ss << "( " << m_Line << " : " << m_Col << " ) - " << m_Text << "\r\n";
    return ss.str();
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Exception:  Parser  ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


Parser::Parser(std::shared_ptr<Tokenizer> lexer)
{
    m_Lexer = lexer;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// PARSER RULES ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


// Rule: module | definition
std::shared_ptr<ASTNode> Parser::ParseOberon() {
    if (m_Lexer == nullptr) throw ;
    m_Lexer->Advance();
    switch (m_Lexer->GetSymbol()) {
        case T_MODULE:      return ParseModule();
        case T_DEFINITION:  return ParseDefinition();
        case T_EOF:         return nullptr;
        default:            throw std::make_shared<SyntaxError>(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Expecting 'MODULE' or 'DEFINITION' as start of file!");
    }
}

// Rule: [ ident '.' ] ident
std::shared_ptr<ASTNode> Parser::ParseQualident() {
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    CheckSymbol(TokenCode::T_IDENT, "Expecting name literal!");
    auto literalText = m_Lexer->GetText();
    m_Lexer->Advance();
    if (m_Lexer->GetSymbol() == TokenCode::T_DOT) {
        m_Lexer->Advance();
        CheckSymbol(TokenCode::T_IDENT, "Expecting name literal after '.' in qualident!");
        auto literalText2 = m_Lexer->GetText();
        m_Lexer->Advance();
        return ASTNode::MakeQualidentNode(line, col, literalText, literalText2);
    }
    return ASTNode::MakeIdentNode(line, col, literalText);
}

// Rule: ident [ '*' | '-' ]
std::shared_ptr<ASTNode> Parser::ParseIdentDef() 
{
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    CheckSymbol(TokenCode::T_IDENT, "Expecting name literal!");
    auto literalText = m_Lexer->GetText();
    m_Lexer->Advance();
    bool isReadOnlyExport = false, isExport = false;
    switch (m_Lexer->GetSymbol()) {
        case T_MUL:
            isExport = true;
            m_Lexer->Advance();
            break;
        case T_MINUS:
            isReadOnlyExport = true;
            m_Lexer->Advance();
            break;
        default:    break;
    }

    return ASTNode::MakeIdentDefNode(line, col, literalText, isReadOnlyExport, isExport); 
}

std::shared_ptr<ASTNode> Parser::ParseConstDeclaration() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseConstExpression() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseTypeDeclaration() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseNamedType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseTypeParams() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseEnumeration() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseArrayType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLengthList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLength() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseVarLength() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseTypeActuals() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParserecordType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseBaseType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFieldListSequence() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFieldList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseIdentList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParsePointerType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseVariableDeclararation() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseDesignator() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseSelector() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseExpList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseExpression() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseRelation() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseSimpleExpression() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseAddOperator() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseTerm() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseMulOperator() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLiteral() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFactor() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseSet() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseElement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseActualParameters() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseAssignment() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureCall() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseStatementSequence() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseIfStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseElsifStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseElseStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseCaseStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseCase() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseCaseLabelList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLabelRange() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLabel() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseWhileStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseElsifStatement2() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseRepeatStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseForStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseWithStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseGuard() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseLoopStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseExitStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureDeclaration() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureHeading() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseReciver() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureBody() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseDeclarationSequence() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParsereturnStatement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFormalParameters() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseReturnType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFPSection() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFormalType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseModule() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseImportList() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseImport() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseImportPath() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseDefinition() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseDeclarationSequence2() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }


///////////////////////////////////////////////////////////////////////////////////////////////////
// UTILITIES //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void Parser::CheckSymbol(TokenCode symbol, std::string msg) {
    if (m_Lexer->GetSymbol() != symbol) {
        throw std::make_shared<SyntaxError>(m_Lexer->GetLine(), m_Lexer->GetColumn(), msg);
    }
}

void Parser::CheckSymbolAndAdvance(TokenCode symbol, std::string msg) {
    if (m_Lexer->GetSymbol() != symbol) {
        throw std::make_shared<SyntaxError>(m_Lexer->GetLine(), m_Lexer->GetColumn(), msg);
    }
    m_Lexer->Advance();
}