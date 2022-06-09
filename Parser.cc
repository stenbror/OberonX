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

// Rule: Expression { ',' Expression }
std::shared_ptr<ASTNode> Parser::ParseExpList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseExpression());
    while (m_Lexer->GetSymbol() == T_COMMA) {
        m_Lexer->Advance();
        nodes->push_back(ParseExpression());
    }

    return ASTNode::MakeExpressionListNode(line, col, nodes); 
}

// Rule: SimpleExpression [ ( '<' | '<=' | '=' | '>=' | '>' | '#' | 'IN' | 'IS' ) SimpleExpression ]
std::shared_ptr<ASTNode> Parser::ParseExpression() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseSimpleExpression();
    switch (m_Lexer->GetSymbol()) {
        case T_LESS:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeLessCompareNode(line, col, left, right);
            }
        case T_LESSEQUAL:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeLessEqualCompareNode(line, col, left, right);
            }
        case T_EQUAL:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeEqualCompareNode(line, col, left, right);
            }
        case T_GREATER:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeGreaterCompareNode(line, col, left, right);
            }
        case T_GREATEREQUAL:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeGreaterEqualCompareNode(line, col, left, right);
            }
        case T_HASH:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeNotEqualCompareNode(line, col, left, right);
            }
        case T_IN:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeInCompareNode(line, col, left, right);
            }
        case T_IS:
            {
                m_Lexer->Advance();
                auto right = ParseSimpleExpression();
                return ASTNode::MakeIsCompareNode(line, col, left, right);
            }
        default:    return left;
    }
}

// Rule: [ '+' | '-' ] Term { ( '+' | '-' | 'OR' ) Term }
std::shared_ptr<ASTNode> Parser::ParseSimpleExpression() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    std::shared_ptr<ASTNode> left = nullptr;

    if (m_Lexer->GetSymbol() == T_PLUS || m_Lexer->GetSymbol() == T_MINUS) {
        if (m_Lexer->GetSymbol() == T_PLUS) {
            m_Lexer->Advance();
            auto right = ParseTerm();
            left = ASTNode::MakeUnaryPlusNode(line, col, right);
        }
        else {
            m_Lexer->Advance();
            auto right = ParseTerm();
            left = ASTNode::MakeUnaryMinusNode(line, col, right);
        }
    }
    else {
        left = ParseTerm();
    }

    while (m_Lexer->GetSymbol() == T_PLUS || m_Lexer->GetSymbol() == T_MINUS || m_Lexer->GetSymbol() == T_OR) {
        switch (m_Lexer->GetSymbol()) {
            case T_PLUS:
                {
                    m_Lexer->Advance();
                    auto right1 = ParseTerm();
                    left = ASTNode::MakePlusNode(line, col, left, right1);
                }
                break;
            case T_MINUS:
                {
                    m_Lexer->Advance();
                    auto right2 = ParseTerm();
                    left = ASTNode::MakeOrNode(line, col, left, right2);
                }
                break;
            default:
                {
                    m_Lexer->Advance();
                    auto right3 = ParseTerm();
                    left = ASTNode::MakeOrNode(line, col, left, right3);
                }
                break;
        }
    }

    return left;
}

// Rule: Factor { ( '*' | '/' | 'DIV' | 'MOD' | '&' ) Factor }
std::shared_ptr<ASTNode> Parser::ParseTerm() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseFactor();
    while (m_Lexer->GetSymbol() == T_MUL || m_Lexer->GetSymbol() == T_SLASH || m_Lexer->GetSymbol() == T_DIV || m_Lexer->GetSymbol() == T_MOD || m_Lexer->GetSymbol() == T_AND) {
        switch (m_Lexer->GetSymbol()) {
            case T_MUL:
                {
                    m_Lexer->Advance();
                    auto right = ParseFactor();
                    left = ASTNode::MakeMulNode(line, col, left, right);
                }
                break;
            case T_SLASH:
                {
                    m_Lexer->Advance();
                    auto right = ParseFactor();
                    left = ASTNode::MakeSlashNode(line, col, left, right);
                }
                break;
            case T_DIV:
                {
                    m_Lexer->Advance();
                    auto right = ParseFactor();
                    left = ASTNode::MakeDivNode(line, col, left, right);
                }
                break;
            case T_MOD:
                {
                    m_Lexer->Advance();
                    auto right = ParseFactor();
                    left = ASTNode::MakeModNode(line, col, left, right);
                }
                break;
            default:
                {
                    m_Lexer->Advance();
                    auto right = ParseFactor();
                    left = ASTNode::MakeAndNode(line, col, left, right);
                }
                break;
        }
    }
    return left; 
}

// Rule: Number | String | HexString | HexChar | 'NIL' | 'TRUE' | 'FALSE' | Set
std::shared_ptr<ASTNode> Parser::ParseLiteral() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    switch (m_Lexer->GetSymbol()) {
        case T_NUMBER:
            {
                auto text = m_Lexer->GetText();
                m_Lexer->Advance();
                return ASTNode::MakeLiteralNumberNode(line, col, text);
            }
        case T_STRING:
            {
                auto text = m_Lexer->GetText();
                m_Lexer->Advance();
                return ASTNode::MakeLiteralStringNode(line, col, text);
            }
        case T_HEX_STRING:
            {
                auto text = m_Lexer->GetText();
                m_Lexer->Advance();
                return ASTNode::MakeLiteralHexStringNode(line, col, text);
            }
        case T_HEX_CHAR:
        {
                auto text = m_Lexer->GetText();
                m_Lexer->Advance();
                return ASTNode::MakeLiteralHexCharNode(line, col, text);
            }
        case T_NIL:
            {
                m_Lexer->Advance();
                return ASTNode::MakeLiteralNilNode(line, col);
            }
        case T_TRUE:
            {
                m_Lexer->Advance();
                return ASTNode::MakeLiteralTrueNode(line, col);
            }
        case T_FALSE:
            {
                m_Lexer->Advance();
                return ASTNode::MakeLiteralFalseNode(line, col);
            }
        case T_LEFTCURLY:
                return ParseSet();
        default:    throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Illegal literal!");
    }
}

// Rule: Literal | Designator [ ActualParameters ] | '(' Expression ')' | '~' Factor
std::shared_ptr<ASTNode> Parser::ParseFactor() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    switch (m_Lexer->GetSymbol()) {
        case T_IDENT:
            {
                auto left = ParseDesignator();
                if (m_Lexer->GetSymbol() != T_LEFTPAREN) return left;
                auto right = ParseActualParameters();
                return ASTNode::MakeCallNode(line, col, left, right);
            }
        case T_LEFTPAREN:
            {
                m_Lexer->Advance();
                auto right = ParseExpression();
                CheckSymbolAndAdvance(T_RIGHTPAREN, "Expecting ')' in expression!");
                return right;
            }
        case T_TILDE:
            {
                m_Lexer->Advance();
                auto right = ParseFactor();
                return ASTNode::MakeBitInvertNode(line, col, right);
            }
        default:    return ParseLiteral();
    } 
}

std::shared_ptr<ASTNode> Parser::ParseSet() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseElement() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseActualParameters() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }

// Rule: IfStatement | CaseStatement | WithStatement | LoopStatement | ExitStatement | ReturnStatement | WhileStatement | RepeatStatement | ForStatement | Assignment | ProcedureCall
std::shared_ptr<ASTNode> Parser::ParseStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    switch (m_Lexer->GetSymbol()) {
        case T_IF:      return ParseIfStatement();
        case T_CASE:    return ParseCaseStatement();
        case T_WITH:    return ParseWithStatement();
        case T_LOOP:    return ParseLoopStatement();
        case T_EXIT:    return ParseExitStatement();
        case T_RETURN:  return ParseReturnStatement();
        case T_WHILE:   return ParseWhileStatement();
        case T_REPEAT:  return ParseRepeatStatement();
        case T_FOR:     return ParseForStatement();
        case T_IDENT:
            {
                auto left = ParseDesignator();
                switch (m_Lexer->GetSymbol()) {
                    case T_ASSIGN:      return ParseAssignment(line, col, left);
                    case T_LEFTPAREN:   return ParseProcedureCall(line, col, left);
                    default:    return left;
                }
            }
        default:    throw SyntaxError(line, col, "Expecting statement!");
    }
}

// Rule: Designator ':=' Expression
std::shared_ptr<ASTNode> Parser::ParseAssignment(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left) { 
    m_Lexer->Advance();
    auto right = ParseExpression();
    return ASTNode::MakeAssignmentNode(line, col, left, right);
}

// Rule: Designator [ActualParameters ]
std::shared_ptr<ASTNode> Parser::ParseProcedureCall(unsigned int line, unsigned int col, std::shared_ptr<ASTNode> left) {
    auto right = ParseActualParameters();
    return ASTNode::MakeProcedureCallNode(line, col, left, right);
}

// Rule: Statement { [ ';' ] Statement }
std::shared_ptr<ASTNode> Parser::ParseStatementSequence() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseStatement());
    while (bool isLock = true) {
        if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance(); // Optional semicolon between statements!
        switch (m_Lexer->GetSymbol()) {
            case T_IF:
            case T_CASE:
            case T_WITH:
            case T_LOOP:
            case T_EXIT:
            case T_RETURN:
            case T_WHILE:
            case T_REPEAT:
            case T_FOR:
            case T_IDENT:
            case T_PROCEDURE:
            case T_PROC:
            case T_LEFTPAREN:
                nodes->push_back(ParseStatement());
                break;
            case T_SEMICOLON:   throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Unexpected ';' !");
            default:    isLock = false;
        }
    }

    return ASTNode::MakeStatementSequenceNode(line, col, nodes);
}

// Rule: 'IF' Expression 'THEN' StatementSequence { 'ELSIF' Expression 'THEN' StatementSequence } [ 'ELSE' StatementSequence ] 'END'
std::shared_ptr<ASTNode> Parser::ParseIfStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseExpression();
    CheckSymbolAndAdvance(T_THEN, "Expecting 'THEN' in 'IF' statement!");
    auto right = ParseStatementSequence();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>(); // 'ELSIF'
    while (m_Lexer->GetSymbol() == T_ELSIF) nodes->push_back(ParseElsifStatement());
    auto next = m_Lexer->GetSymbol() == T_ELSE ? ParseElseStatement() : nullptr;
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'IF' statement!");
    return ASTNode::MakeIfStatementNode(line, col, left, right, nodes, next); 
}

// Rule: 'ELSIF' Expression 'THEN' StatementSequence
std::shared_ptr<ASTNode> Parser::ParseElsifStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseExpression();
    CheckSymbolAndAdvance(T_THEN, "Expecting 'THEN' in 'ELSIF' statement!");
    auto right = ParseStatementSequence();
    return ASTNode::MakeElsifStatementNode(line, col, left, right);
}

// Rule: 'ELSE' StatementSequence
std::shared_ptr<ASTNode> Parser::ParseElseStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto right = ParseStatementSequence();
    return ASTNode::MakeElseStatementNode(line, col, right); 
}

// Rule: 'CASE' Expression 'OF' Case { '|' Case } [ 'ELSE' StatementSequence ] 'END'
std::shared_ptr<ASTNode> Parser::ParseCaseStatement() {
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseExpression();
    CheckSymbolAndAdvance(T_OF, "Expecting 'OF' in 'CASE' Statement!");
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseCase());
    while (m_Lexer->GetSymbol() == T_BAR) {
        m_Lexer->Advance();
        nodes->push_back(ParseCase());
    }
    auto right = m_Lexer->GetSymbol() == T_ELSE ? ParseElseStatement() : nullptr;
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'CASE' Statement!");
    return ASTNode::MakeCaseStatementNode(line, col, left, nodes, right);
}

// Rule: [ CaseLabel ':' StatementSequence ]
std::shared_ptr<ASTNode> Parser::ParseCase() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseCaseLabelList();
    CheckSymbolAndAdvance(T_COLON, "Expecting ':' in 'CASE' Statement!");
    auto right = ParseStatementSequence();
    return ASTNode::MakeCaseStatement(line, col, left, right); 
}

// Rule: CaseLabel { ',' Case Label }
std::shared_ptr<ASTNode> Parser::ParseCaseLabelList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseLabelRange());
    while (m_Lexer->GetSymbol() == T_COMMA) {
        m_Lexer->Advance();
        nodes->push_back(ParseLabelRange());
    }
    return ASTNode::MakeCaseLabelRangeNode(line, col, nodes); 
}

// Rule: Label [ '..' Label ]
std::shared_ptr<ASTNode> Parser::ParseLabelRange() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseConstExpression();
    if (m_Lexer->GetSymbol() == T_UPTO) {
        m_Lexer->Advance();
        auto right = ParseConstExpression();
        return ASTNode::MakeLabelRangeNode(line, col, left, right);
    }
    return left; 
}

// Rule: 'WHILE' Expression 'DO' StatementSequence { 'ELSIF' Expression 'DO' StatementSequence } 'END'
std::shared_ptr<ASTNode> Parser::ParseWhileStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseExpression();
    CheckSymbolAndAdvance(T_DO, "Expecting 'DO' in 'WHILE' statement!");
    auto right = ParseStatementSequence();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>(); // 'ELSIF'
    while (m_Lexer->GetSymbol() == T_ELSIF) nodes->push_back(ParseElsifStatement2());
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'WHILE' statement!");
    return ASTNode::MakeWhileStatementNode(line, col, left, right, nodes); 
}

// Rule: 'ELSIF' Expression 'DO' StatementSequence
std::shared_ptr<ASTNode> Parser::ParseElsifStatement2() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseExpression();
    CheckSymbolAndAdvance(T_DO, "Expecting 'DO' in 'ELSIF' statement!");
    auto right = ParseStatementSequence();
    return ASTNode::MakeElsifStatementNode(line, col, left, right);
}

// Rule: 'REPEAT' StatementSequence 'UNTIL' Expression
std::shared_ptr<ASTNode> Parser::ParseRepeatStatement() {
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto left = ParseStatementSequence();
    CheckSymbolAndAdvance(T_UNTIL, "Expected 'UNTIL'!");
    auto right = ParseExpression();
    return ASTNode::MakeRepeatStatementNode(line, col, left, right); 
}

// Rule: 'FOR' ident ':=' Expression 'TO' Expression [ 'BY' ConstExpression ] 'DO' StatementSequence 'END' 
std::shared_ptr<ASTNode> Parser::ParseForStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    CheckSymbol(T_IDENT, "Expecting literal name in 'FOR' Statement!");
    auto literalText = m_Lexer->GetText();
    m_Lexer->Advance();
    CheckSymbolAndAdvance(T_ASSIGN, "Expecting ':=' in 'FOR' Statement!");
    auto left = ParseExpression(); 
    CheckSymbolAndAdvance(T_TO, "Missing 'TO' in 'FOR' Statement!");
    auto right = ParseExpression();
    std::shared_ptr<ASTNode> next = nullptr;
    if (m_Lexer->GetSymbol() == T_BY) {
        m_Lexer->Advance();
        next = ParseConstExpression();
    }
    CheckSymbolAndAdvance(T_DO, "Expecting 'DO' in 'FOR' Statement!");
    auto seq = ParseStatementSequence();
    CheckSymbolAndAdvance(T_END, "Expecting 'END' in 'FOR' Statement!");
    return ASTNode::MakeForStatementNode(line, col, literalText, left, right, next, seq); 
}

// Rule: 'WITH' Guard 'DO' StatementSequence { '|' Guard 'DO' StatementSequence } [ 'ELSE' StatementSequence ] 'END'
std::shared_ptr<ASTNode> Parser::ParseWithStatement() {
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto GuardNodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    auto StatementBlockNodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    m_Lexer->Advance(); // 'WITH'
    GuardNodes->push_back(ParseGuard());
    CheckSymbolAndAdvance(T_DO, "Expecting 'DO' in 'WITH' Statement!");
    StatementBlockNodes->push_back(ParseStatementSequence());
    while (m_Lexer->GetSymbol() == T_BAR) {
        m_Lexer->Advance();
        GuardNodes->push_back(ParseGuard());
        CheckSymbolAndAdvance(T_DO, "Expecting 'DO' in 'WITH' Statement!");
        StatementBlockNodes->push_back(ParseStatementSequence());
    }
    auto elsePart = m_Lexer->GetSymbol() == T_ELSE ? ParseElseStatement() : nullptr;
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'WITH' Statement!");
    return ASTNode::WithStatementNode(line, col, GuardNodes, StatementBlockNodes, elsePart); 
}

// Rule: Qualident ':' Qualident
std::shared_ptr<ASTNode> Parser::ParseGuard() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseQualident();
    CheckSymbolAndAdvance(T_COLON, "Expecting ':' in guard part of 'WITH' Statement!");
    auto right = ParseQualident();
    return ASTNode::GuardNode(line, col, left, right); 
}

// Rule: 'LOOP' StatementSequence 'END'
std::shared_ptr<ASTNode> Parser::ParseLoopStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto right = ParseStatementSequence();
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'LOOP' Statement!");
    return ASTNode::MakeLoopStatementNode(line, col, right); 
}

// Rule: 'EXIT'
std::shared_ptr<ASTNode> Parser::ParseExitStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    return ASTNode::MakeExitStatementNode(line, col); 
}

std::shared_ptr<ASTNode> Parser::ParseProcedureDeclaration() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureHeading() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseReciver() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseProcedureBody() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseDeclarationSequence() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }

// Rule: 'RETURN' [ Expression ]
std::shared_ptr<ASTNode> Parser::ParseReturnStatement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    std::shared_ptr<ASTNode> right = nullptr;
    switch (m_Lexer->GetSymbol()) {
        case T_SEMICOLON:
        case T_END:
        case T_IF:
        case T_CASE:
        case T_WITH:
        case T_LOOP:
        case T_EXIT:
        case T_RETURN:
        case T_WHILE:
        case T_REPEAT:
        case T_FOR:
        case T_IDENT:
        case T_PROCEDURE:
        case T_PROC:
        case T_LEFTPAREN:   break;
        default:
            right = ParseExpression();

    }
    return ASTNode::MakeReturnStatementNode(line, col, right); 
}

std::shared_ptr<ASTNode> Parser::ParseFormalParameters() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseReturnType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFPSection() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseFormalType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }

// Rule: 'MODULE' Ident [ TypeParams ] [ ';' ] { ImportSequence | DeclarationSequence } [ 'BEGIN' StatementSequence ] 'END' Ident [ '.' ]
std::shared_ptr<ASTNode> Parser::ParseModule() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance(); // 'MODULE'
    CheckSymbol(T_IDENT, "Name of module is missing!");
    auto moduleText = m_Lexer->GetText();
    m_Lexer->Advance();
    auto typeParams = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseTypeParams() : nullptr;
    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance(); // Optional ';'

    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    while (bool isLock = true) {
        switch (m_Lexer->GetSymbol()) {
            case T_IMPORT:  nodes->push_back(ParseImportList()); break;
            case T_CONST:
            case T_TYPE:
            case T_VAR:
            case T_PROCEDURE:
            case T_PROC:
            case T_LEFTPAREN:
                nodes->push_back(ParseDeclarationSequence());
                break;
            default:    isLock = false;
        }
    }

    std::shared_ptr<ASTNode> block = nullptr;
    if (m_Lexer->GetSymbol() == T_BEGIN) {
        m_Lexer->Advance();
        block = ParseStatementSequence();
    }

    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of module!");
    CheckSymbol(T_IDENT, "Missing module name at end of module!");
    if (moduleText != m_Lexer->GetText()) throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Module name is inconsistant in module!");
    m_Lexer->Advance();

    if (m_Lexer->GetSymbol() == T_DOT) m_Lexer->Advance(); // optional '.' at end of module
    if (m_Lexer->GetSymbol() != T_EOF) throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Expecting End of file!");

    return ASTNode::MakeModuleNode(line, col, moduleText, typeParams, nodes, block); 
}

// Rule: 'IMPORT' Import { [ ', '  Import ] } [ ';' ] 
std::shared_ptr<ASTNode> Parser::ParseImportList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance(); // 'IMPORT'
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseImport());
    while (m_Lexer->GetSymbol() == T_COMMA) {
        m_Lexer->Advance();
        nodes->push_back(ParseImport());
    }
    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();

    return ASTNode::MakeImportListNode(line, col, nodes); 
}

// Rule:
std::shared_ptr<ASTNode> Parser::ParseImport() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    CheckSymbol(T_IDENT, "Expecting name of 'IMPORT' statement!");
    auto queryText = m_Lexer->GetText();
    m_Lexer->Advance();
    if (m_Lexer->GetSymbol() == T_ASSIGN) {
        auto left = queryText;
        m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting name literal after ':=' in import Statement!");
        queryText = m_Lexer->GetText();
        m_Lexer->Advance();
        auto right = queryText;
        if (m_Lexer->GetSymbol() == T_DOT) {
            m_Lexer->Advance();
            CheckSymbol(T_IDENT, "Expecting name literal after '.' in import Statement!");
            auto next = m_Lexer->GetText();
            m_Lexer->Advance();
            auto last = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseTypeActuals() : nullptr;
            return ASTNode::MakeImportAssignPathNode(line, col, left, right, next, last);
        }
        else {
            auto next = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseTypeActuals() : nullptr;
            return ASTNode::MakeImportAssignNode(line, col, left, right, next);
        }
    }
    else if (m_Lexer->GetSymbol() == T_DOT) { // ImportPath
        auto left = queryText;
        m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting name literal after '.' in import Statement!");
        auto right = m_Lexer->GetText();
        m_Lexer->Advance();
        auto next = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseTypeActuals() : nullptr;
        return ASTNode::MakeImportPathNode(line, col, left, right, next);
    }
    else {
        auto left = queryText;
        auto right = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseTypeActuals() : nullptr;
        return ASTNode::MakeImportNode(line, col, left, right);
    }
}


// Rule: 'DEFINITION' Ident [ ';' ] [ ImportList ] DeclarationSequence2 'END' Ident [ '.' ]
std::shared_ptr<ASTNode> Parser::ParseDefinition() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance(); // 'DEFINITION'
    CheckSymbol(T_IDENT, "Missing definition name!");
    auto defName = m_Lexer->GetText();
    m_Lexer->Advance();
    auto left = m_Lexer->GetSymbol() == T_IMPORT ? ParseImportList() : nullptr;
    auto right = ParseDeclarationSequence2();
    CheckSymbolAndAdvance(T_END, "Expecting 'END' in defintion!");
    CheckSymbol(T_IDENT, "Missing ident at end of declaration sequence!");
    if (defName != m_Lexer->GetText()) throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Inconsitant name of definition Sequence!");
    m_Lexer->Advance();
    if (m_Lexer->GetSymbol() == T_DOT) m_Lexer->Advance();

    return ASTNode::MakeDeclarationNode(line, col, defName, left, right); 
}

// Rule: { CONST { ConstDeclaration [ '; ] } | TYPE { TypeDeclaration [ '; ] } | VAR { VariableDeclaration [ '; ] } | ProcedureHeading [ '; ] }
std::shared_ptr<ASTNode> Parser::ParseDeclarationSequence2() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    while (bool isLock = true) {
        switch (m_Lexer->GetSymbol()) {
            case T_CONST:
                {
                    m_Lexer->Advance();
                    nodes->push_back(ParseConstDeclaration());
                    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                    while (bool isLock2 = true) {
                        switch (m_Lexer->GetSymbol()) {
                            case T_CONST:
                            case T_TYPE:
                            case T_VAR:
                            case T_PROCEDURE:
                            case T_PROC:
                            case T_LEFTPAREN:
                            case T_END:
                                isLock2 = false;
                                break;
                            default:
                                nodes->push_back(ParseConstDeclaration());
                                if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                        }
                    }
                }
                break;
            case T_TYPE:
                {
                    m_Lexer->Advance();
                    nodes->push_back(ParseTypeDeclaration());
                    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                    while (bool isLock2 = true) {
                        switch (m_Lexer->GetSymbol()) {
                            case T_CONST:
                            case T_TYPE:
                            case T_VAR:
                            case T_PROCEDURE:
                            case T_PROC:
                            case T_LEFTPAREN:
                            case T_END:
                                isLock2 = false;
                                break;
                            default:
                                nodes->push_back(ParseTypeDeclaration());
                                if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                        }
                    }
                }
                break;
            case T_VAR:
                {
                    m_Lexer->Advance();
                    nodes->push_back(ParseVariableDeclararation());
                    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                    while (bool isLock2 = true) {
                        switch (m_Lexer->GetSymbol()) {
                            case T_CONST:
                            case T_TYPE:
                            case T_VAR:
                            case T_PROCEDURE:
                            case T_PROC:
                            case T_LEFTPAREN:
                            case T_END:
                                isLock2 = false;
                                break;
                            default:
                                nodes->push_back(ParseVariableDeclararation());
                                if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                        }
                    }
                }
                break;
            case T_PROCEDURE:
            case T_PROC:
            case T_LEFTPAREN:
                nodes->push_back(ParseProcedureHeading());
                if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
                break;
            default:
                isLock = false;
        }
    }

    return ASTNode::MakeDeclarationSequence2Node(line, col, nodes); 
}


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