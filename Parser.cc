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

// Rule: NamedType | EnumerationType | ArrayType | RecordType | PointerType | ProcedureType
std::shared_ptr<ASTNode> Parser::ParseType() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    switch (m_Lexer->GetSymbol()) {
        case T_IDENT:       return ParseNamedType();
        case T_LEFTPAREN:   return ParseEnumeration();
        case T_ARRAY:
        case T_LEFTBRACKET: return ParseArrayType();
        case T_RECORD:      return ParseRecordType();
        case T_ARROW:
        case T_POINTER:     return ParsePointerType();
        case T_PROCEDURE:
        case T_PROC:        return ParseProcedureType();
        default:            throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Illegal Type!");;
    }
}

// Rule: Qualident
std::shared_ptr<ASTNode> Parser::ParseNamedType() { 
    return ParseQualident(); 
}

// Rule: '(' ident { [ ','  ident ] } ')'
std::shared_ptr<ASTNode> Parser::ParseTypeParams() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::string>>();
    CheckSymbolAndAdvance(T_LEFTPAREN, "Expecting '(' in Type Params!");
    CheckSymbol(T_IDENT, "Expecting name literal in Type Params!");
    nodes->push_back(m_Lexer->GetText());
    m_Lexer->Advance();
    while (m_Lexer->GetSymbol() != T_RIGHTPAREN) {
        if (m_Lexer->GetSymbol() == T_COMMA) m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting name literal in Type Params!");
        nodes->push_back(m_Lexer->GetText());
        m_Lexer->Advance();
    }
    m_Lexer->Advance();
    return ASTNode::MakeTypeParamsNode(line, col, nodes); 
}

// Rule: '(' ident { [ ',' ] ident } ')'
std::shared_ptr<ASTNode> Parser::ParseEnumeration() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto nodes = std::make_shared<std::vector<std::string>>();
    CheckSymbol(T_IDENT, "Expecting name of enumeration element!");
    nodes->push_back(m_Lexer->GetText());
    m_Lexer->Advance();
    while (m_Lexer->GetSymbol() != T_RIGHTPAREN) {
        if (m_Lexer->GetSymbol() == T_COMMA) m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting name of enumeration element!");
        nodes->push_back(m_Lexer->GetText());
        m_Lexer->Advance();
    }
    m_Lexer->Advance(); // ')'
    return ASTNode::MakeEnumerationNode(line, col, nodes);
}

// Rule: 'ARRAY' '[' LengthList ']' 'OF' Type | '[' [ LengthList ] ']' Type
std::shared_ptr<ASTNode> Parser::ParseArrayType() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    if (m_Lexer->GetSymbol() == T_ARRAY) {
        m_Lexer->Advance();
        CheckSymbolAndAdvance(T_LEFTBRACKET, "Expecting '[' in 'ARRAY' type!");
        auto left = ParseLengthList();
        CheckSymbolAndAdvance(T_RIGHTBRACKET, "Expecting ']' in 'ARRAY' type!");
        CheckSymbolAndAdvance(T_OF, "Expecting 'OF' in 'ARRAY' type!");
        auto right = ParseType();
        return ASTNode::MakeArrayOfNode(line, col, left, right);
    }
    else {
        CheckSymbolAndAdvance(T_LEFTBRACKET, "Expecting '[' in 'ARRAY' type!");
        auto left = m_Lexer->GetSymbol() != T_RIGHTBRACKET ? ParseLengthList() : nullptr;
        CheckSymbolAndAdvance(T_RIGHTBRACKET, "Expecting ']' in 'ARRAY' type!");
        auto right = ParseType();
        return ASTNode::MakeArrayNode(line, col, left, right);
    }
}

// Rule: Length { ',' Length } | 'VAR' varlength { ',' varlength }
std::shared_ptr<ASTNode> Parser::ParseLengthList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    if (m_Lexer->GetSymbol() == T_VAR) {
        m_Lexer->Advance();
        nodes->push_back(ParseVarLength());
        while (m_Lexer->GetSymbol() == T_COMMA) {
            m_Lexer->Advance();
            nodes->push_back(ParseVarLength());
        }
        return ASTNode::MakeLengthList(line, col, true, nodes);
    }
    nodes->push_back(ParseLength());
    while (m_Lexer->GetSymbol() == T_COMMA) {
        m_Lexer->Advance();
        nodes->push_back(ParseLength());
    }
    return ASTNode::MakeLengthList(line, col, false, nodes);
}

// Rule: ConstExpression
std::shared_ptr<ASTNode> Parser::ParseLength() { 
    return ParseExpression(); 
}

// Rule: Expression
std::shared_ptr<ASTNode> Parser::ParseVarLength() { 
    return ParseExpression(); 
}

// Rule: '(' NamedType { [ ',' ] NamedType } ')'
std::shared_ptr<ASTNode> Parser::ParseTypeActuals() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    CheckSymbol(T_IDENT, "Expecting name of enumeration element!");
    nodes->push_back(ParseNamedType());
    while (m_Lexer->GetSymbol() != T_RIGHTPAREN) {
        if (m_Lexer->GetSymbol() == T_COMMA) m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting name of enumeration element!");
        nodes->push_back(ParseNamedType());
    }
    m_Lexer->Advance(); // ')'
    return ASTNode::MakeTypeActualsNode(line, col, nodes); 
}

// Rule: 'RECORD' [ '(' BaseType ')' ] [ FieldSequence ] 'END'
std::shared_ptr<ASTNode> Parser::ParseRecordType() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    CheckSymbolAndAdvance(T_RECORD, "Expecting 'RECORD'!");
    std::shared_ptr<ASTNode> left = nullptr; // BaseType
    if (m_Lexer->GetSymbol() == T_LEFTPAREN) {
        m_Lexer->Advance();
        left = ParseBaseType();
        CheckSymbolAndAdvance(T_RIGHTPAREN, "Expecting ')' in base 'RECORD' type!");
    }
    auto right = m_Lexer->GetSymbol() != T_END ? ParseFieldListSequence() : nullptr;
    CheckSymbolAndAdvance(T_END, "Expecting 'END' at end of 'RECORD' type!");
    return ASTNode::MakeRecordTypeNode(line, col, left, right); 
}

// Rule: NamedType
std::shared_ptr<ASTNode> Parser::ParseBaseType() { 
    return ParseNamedType(); 
}

// Rule: FieldList [ ';' ] { FieldList [ ';' ] }
std::shared_ptr<ASTNode> Parser::ParseFieldListSequence() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseFieldList());
    while (m_Lexer->GetSymbol() != T_END) {
        if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
        if (m_Lexer->GetSymbol() != T_END) nodes->push_back(ParseFieldList());
    }
    return ASTNode::MakeFieldListSequenceNode(line, col, nodes); 
}

// Rule: IdentList ':' Type
std::shared_ptr<ASTNode> Parser::ParseFieldList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseIdentList();
    CheckSymbolAndAdvance(T_COLON, "Expecting ':' in Field declaration of 'RECORD'!");
    auto right = ParseType();
    return ASTNode::MakeFieldListNode(line, col, left, right); 
}

// Rule: Identdef { [','] Identdef }
std::shared_ptr<ASTNode> Parser::ParseIdentList() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseIdentDef());
    while (m_Lexer->GetSymbol() != T_COLON) {
        if (m_Lexer->GetSymbol() == T_COMMA) m_Lexer->Advance();
        nodes->push_back(ParseIdentDef());
    }
    return ASTNode::MakeIdentListNode(line, col, nodes); 
}

// Rule: ( 'POINTER' 'TO' | '^' ) Type
std::shared_ptr<ASTNode> Parser::ParsePointerType() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    if (m_Lexer->GetSymbol() == T_POINTER) {
        m_Lexer->Advance();
        CheckSymbolAndAdvance(T_TO, "Expecting 'TO' in pointer declaration!");
        auto right = ParseType();
        return ASTNode::MakePointerNode(line, col, false, right);
    }
    else {
        CheckSymbolAndAdvance(T_ARROW, "Expecting '^' in pointer declaration!");
        auto right = ParseType();
        return ASTNode::MakePointerNode(line, col, true, right);
    }
}

std::shared_ptr<ASTNode> Parser::ParseProcedureType() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }
std::shared_ptr<ASTNode> Parser::ParseVariableDeclararation() { return std::make_shared<ASTNode>(ASTNode(1, 1)); }

// Rule: Qualident { Selector } 
std::shared_ptr<ASTNode> Parser::ParseDesignator() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseQualident();
    if (m_Lexer->GetSymbol() == T_DOT || m_Lexer->GetSymbol() == T_LEFTPAREN || m_Lexer->GetSymbol() == T_LEFTBRACKET || m_Lexer->GetSymbol() == T_ARROW) {
        auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
        while (m_Lexer->GetSymbol() == T_DOT || m_Lexer->GetSymbol() == T_LEFTPAREN || m_Lexer->GetSymbol() == T_LEFTBRACKET || m_Lexer->GetSymbol() == T_ARROW) 
            nodes->push_back(ParseSelector());
        return ASTNode::MakeDesignatorNode(line, col, left, nodes);
    }
    return left; 
}

// Rule: '.' ident | '[' ExpList '] | '^' | '(' Qualident ')
std::shared_ptr<ASTNode> Parser::ParseSelector() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    switch (m_Lexer->GetSymbol()) {
        case T_DOT:
            {
                m_Lexer->Advance();
                CheckSymbol(T_IDENT, "Expecting name literal after '.'");
                auto text = m_Lexer->GetText();
                m_Lexer->Advance();
                return ASTNode::MakeDotNameNode(line, col, text);
            }
        case T_LEFTPAREN:
            {
                m_Lexer->Advance();
                auto right = ParseQualident();
                CheckSymbolAndAdvance(T_RIGHTPAREN, "Expecting ')' in selector!");
                return ASTNode::MakeCallQualidentNode(line, col, right);
            }
        case T_LEFTBRACKET:
            {
                m_Lexer->Advance();
                auto right = ParseExpList();
                CheckSymbolAndAdvance(T_RIGHTBRACKET, "Expected ']' in indexing!");
                return ASTNode::MakeIndexNode(line, col, right);
            }
        default:    // T_ARROW:
            m_Lexer->Advance();
            return ASTNode::MakeArrowNode(line, col);
    }
}

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

// Rule: '{' [ Element { ',' Element } ] '}'
std::shared_ptr<ASTNode> Parser::ParseSet() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance();
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    if (m_Lexer->GetSymbol() != T_RIGHTCURLY) {
        nodes->push_back(ParseElement());
        while (m_Lexer->GetSymbol() == T_COMMA) {
            m_Lexer->Advance();
            nodes->push_back(ParseElement());
        }
    }
    CheckSymbolAndAdvance(T_RIGHTCURLY, "Expecting '}' at end of set!");
    return ASTNode::MakeSetNode(line, col, nodes); 
}

// Rule: Expression [ '..' Expression ]
std::shared_ptr<ASTNode> Parser::ParseElement() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseExpression();
    if (m_Lexer->GetSymbol() == T_UPTO) {
        m_Lexer->Advance();
        auto right = ParseExpression();
        return ASTNode::MakeElementNode(line, col, left, right);
    }
    return left; 
}

// Rule: '(' [ ExpList ] ')'
std::shared_ptr<ASTNode> Parser::ParseActualParameters() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    CheckSymbolAndAdvance(T_LEFTPAREN, "Expecting '(' in Parameters!");
    auto right = m_Lexer->GetSymbol() != T_RIGHTPAREN ? ParseExpList() : nullptr;
    CheckSymbolAndAdvance(T_RIGHTPAREN, "Expecting ')' in Parameters!");
    return ASTNode::MakeActualParametersNode(line, col, right); 
}

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

// Rule: ProcedureHeading [ ';' ] ProcedureBody 'END' ident 
std::shared_ptr<ASTNode> Parser::ParseProcedureDeclaration() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseProcedureHeading();
    if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
    auto right = ParseProcedureBody();
    CheckSymbolAndAdvance(T_END, "Expecting 'END' in 'PROCEDURE' or 'PROC' declaration!");
    CheckSymbol(T_IDENT, "Missing name literal at end of 'PROCEDURE' or 'PROC' declaration!");
    auto name = m_Lexer->GetText();
    m_Lexer->Advance();
    return ASTNode::MakeProcedureDeclarationNode(line, col, left, right, name); 
}

// Rule: ( 'PROCEDURE' | 'PROC' ) [ Reciver ] IdentDef [ FormalParameters ]
std::shared_ptr<ASTNode> Parser::ParseProcedureHeading() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto isProc = false;
    if (m_Lexer->GetSymbol() == T_PROCEDURE) m_Lexer->Advance();
    else {
        isProc = true;
        m_Lexer->Advance();
    }
    auto reciver = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseReciver() : nullptr;
    auto name = ParseIdentDef();
    auto formal = m_Lexer->GetSymbol() == T_LEFTPAREN ? ParseFormalParameters() : nullptr;
    return ASTNode::MakeProcedureHeading(line, col, isProc, reciver, name, formal); 
}

// Rule: '(' [ 'VAR' | 'IN' ] ident ':' ident ')'
std::shared_ptr<ASTNode> Parser::ParseReciver() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance(); // '(')
    bool isVar = false, isIn = false;
    if (m_Lexer->GetSymbol() == T_VAR) {
        m_Lexer->Advance();
        isVar = true;
    } 
    else if (m_Lexer->GetSymbol() == T_IN) {
        m_Lexer->Advance();
        isIn = true;
    }
   
    CheckSymbol(T_IDENT, "Expecting name literal in reciver's first column!");
    auto left = m_Lexer->GetText();
    m_Lexer->Advance();
   
    CheckSymbolAndAdvance(T_COLON, "Expecting ':' in reciver!");
   
    CheckSymbol(T_IDENT, "Expecting name literal in reciver's first column!");
    auto right = m_Lexer->GetText();
    m_Lexer->Advance();

    CheckSymbolAndAdvance(T_RIGHTPAREN, "Expecting ')' in reciver!");
    return ASTNode::MakeReciverNode(line, col, left, right); 
}

// Rule: DeclarationSequence [ 'BEGIN' StatementSequence | 'returnStatement [ ';' ] ]
std::shared_ptr<ASTNode> Parser::ParseProcedureBody() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    auto left = ParseDeclarationSequence(false);
    std::shared_ptr<ASTNode> right = nullptr;
    if (m_Lexer->GetSymbol() == T_BEGIN) {
        m_Lexer->Advance();
        right = ParseStatementSequence();
    }
    else {
        right = ParseReturnStatement();
        if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
    }
    return ASTNode::MakeProcedureBodyNode(line, col, left, right); 
}

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
        case T_PROCEDURE:
        case T_PROC:
        case T_LEFTPAREN:   break;
        default:
            right = ParseExpression();

    }
    return ASTNode::MakeReturnStatementNode(line, col, right); 
}

// Rule: '(' FPSection { [ ';' ] FPSection } ')'
std::shared_ptr<ASTNode> Parser::ParseFormalParameters() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    m_Lexer->Advance(); // '('
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();
    nodes->push_back(ParseFPSection());
    while (m_Lexer->GetSymbol() != T_RIGHTPAREN) {
        if (m_Lexer->GetSymbol() == T_SEMICOLON) m_Lexer->Advance();
        nodes->push_back(ParseFPSection());
    }
    m_Lexer->Advance(); // ')'
    return ASTNode::MakeFormalParametersNode(line, col, nodes); 
}

// Rule: Type
std::shared_ptr<ASTNode> Parser::ParseReturnType() { 
    return ParseType(); // Possible remove this rule later! 
}

// Rule: [ 'VAR' | 'IN' ] ident { [ ',' ] ident } ':' FormalType
std::shared_ptr<ASTNode> Parser::ParseFPSection() { 
    auto line = m_Lexer->GetLine(); auto col = m_Lexer->GetColumn();
    bool isVar = false, isIn = false;
    if (m_Lexer->GetSymbol() == T_VAR) {
        m_Lexer->Advance();
        isVar = true;
    }
    else if (m_Lexer->GetSymbol() == T_IN) {
        m_Lexer->Advance();
        isIn = true;
    }
    auto nodes = std::make_shared<std::vector<std::string>>();
    nodes->push_back(m_Lexer->GetText());
    m_Lexer->Advance();
    while (m_Lexer->GetSymbol() != T_COLON) {
        if (m_Lexer->GetSymbol() == T_COMMA) m_Lexer->Advance();
        CheckSymbol(T_IDENT, "Expecting literal name in arguments!");
        nodes->push_back(m_Lexer->GetText());
        m_Lexer->Advance();
    }
    m_Lexer->Advance(); // ':'
    auto formalType = ParseFormalType();
    return ASTNode::MakeFPSectionNode(line, col, nodes, formalType, isVar, isIn); 
}

// Rule: Type
std::shared_ptr<ASTNode> Parser::ParseFormalType() { 
    return ParseType(); // ossible remove this rule later! 
}

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
    auto right = ParseDeclarationSequence();
    CheckSymbolAndAdvance(T_END, "Expecting 'END' in defintion!");
    CheckSymbol(T_IDENT, "Missing ident at end of declaration sequence!");
    if (defName != m_Lexer->GetText()) throw SyntaxError(m_Lexer->GetLine(), m_Lexer->GetColumn(), "Inconsitant name of definition Sequence!");
    m_Lexer->Advance();
    if (m_Lexer->GetSymbol() == T_DOT) m_Lexer->Advance();

    return ASTNode::MakeDeclarationNode(line, col, defName, left, right); 
}

// Rule: { CONST { ConstDeclaration [ '; ] } | TYPE { TypeDeclaration [ '; ] } | VAR { VariableDeclaration [ '; ] } | ProcedureHeading [ '; ] }
std::shared_ptr<ASTNode> Parser::ParseDeclarationSequence(bool isDefinition) { 
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