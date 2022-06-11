
#include "Tokenizer.h"


auto reservedKeywords = std::map<std::string, TokenCode>() = {
    { "array", T_ARRAY },
    { "ARRAY", T_ARRAY },
    { "begin", T_BEGIN },
    { "BEGIN", T_BEGIN },
    { "by", T_BY },
    { "BY", T_BY },
    { "case", T_CASE },
    { "CASE", T_CASE },
    { "const", T_CONST },
    { "CONST", T_CONST },
    { "definition", T_DEFINITION },
    { "DEFINITION", T_DEFINITION },
    { "div", T_DIV },
    { "DIV", T_DIV },
    { "do", T_DO },
    { "DO", T_DO },
    { "else", T_ELSE },
    { "ELSE", T_ELSE },
    { "elsif", T_ELSIF },
    { "ELSIF", T_ELSIF },
    { "end", T_END },
    { "END", T_END },
    { "exit", T_EXIT },
    { "EXIT", T_EXIT },
    { "false", T_FALSE },
    { "FALSE", T_FALSE },
    { "for", T_FOR },
    { "FOR", T_FOR },
    { "if", T_IF },
    { "IF", T_IF },
    { "import", T_IMPORT },
    { "IMPORT", T_IMPORT },
    { "in", T_IN },
    { "IN", T_IN },
    { "is", T_IS },
    { "IS", T_IS },
    { "loop", T_LOOP },
    { "LOOP", T_LOOP },
    { "mod", T_MOD },
    { "MOD", T_MOD },
    { "module", T_MODULE },
    { "MODULE", T_MODULE },
    { "nil", T_NIL },
    { "NIL", T_NIL },
    { "of", T_OF },
    { "OF", T_OF } ,
    { "or", T_OR },
    { "OR", T_OR },
    { "pointer", T_POINTER },
    { "POINTER", T_POINTER },
    { "proc", T_PROC },
    { "PROC", T_PROC },
    { "procedure", T_PROCEDURE },
    { "PROCEDURE", T_PROCEDURE },
    { "record", T_RECORD },
    { "RECORD", T_RECORD },
    { "repeat", T_REPEAT },
    { "REPEAT", T_REPEAT },
    { "return", T_RETURN },
    { "RETURN", T_RETURN },
    { "then", T_THEN },
    { "THEN", T_THEN },
    { "to", T_TO },
    { "TO", T_TO },
    { "true", T_TRUE },
    { "TRUE", T_TRUE },
    { "type", T_TYPE },
    { "TYPE", T_TYPE },
    { "until", T_UNTIL },
    { "UNTIL", T_UNTIL },
    { "var", T_VAR },
    { "VAR", T_VAR },
    { "while", T_WHILE },
    { "WHILE", T_WHILE },
    { "with", T_WITH },
    { "WITH", T_WITH }
} ;



Tokenizer::Tokenizer(const std::shared_ptr<std::ifstream> fin) { 
    m_fin = fin;
    m_Symbol = T_EOF;
    m_Line = 1;
    m_Col = 1;
    m_ch = GetChar();
}

TokenCode Tokenizer::GetSymbol() { return m_Symbol; }

unsigned int Tokenizer::GetLine() { return m_Line; }

unsigned int Tokenizer::GetColumn() { return m_Col; }

std::string Tokenizer::GetText() { return m_Buffer; }

char Tokenizer::GetChar() {
    return m_ch = !m_fin->eof() ? m_fin->get() : '\0';   
}

// Get next valid symbol for parser
void Tokenizer::Advance() {

_whitespace: 
    /* Remove whitespace */
    while (m_ch == ' ' || m_ch == '\t') {
        m_Col++; m_ch = GetChar();
    }

    /* Handle newline */
    if (m_ch == '\r' || m_ch == '\n') {
        if (m_ch == '\r') {
            m_ch = GetChar();
            if (m_ch == '\n') {
                m_ch = GetChar();
            }
        }
        else m_ch = GetChar();
        m_Line++; m_Col = 1;
        goto _whitespace;
    }

    /* Operator or delimiters */
    switch (m_ch) {
        case '(' :
            {
                m_ch = GetChar();
                m_Col++;
                if (m_ch == '*') {
                    m_Col++;
                    m_ch = GetChar();
_comment:
                    while (m_ch != '*') {
                        if (m_ch == '\r' || m_ch == '\n') {
                            if (m_ch == '\r') {
                                m_ch = GetChar();
                                if (m_ch == '\n') {
                                    m_ch = GetChar();
                                }
                            }
                            else m_ch = GetChar();
                            m_Line++; m_Col = 1;
                            goto _comment;
                        }
                        if (m_ch == '\0') {
                            m_Symbol = T_EOF;
                            return;
                        }
                        m_Col++; m_ch = GetChar();
                    }
                    m_Col++; m_ch = GetChar();
                    if (m_ch == ')') {
                        m_Col++; m_ch = GetChar();
                        goto _whitespace;
                    }
                    else goto _comment;
                }
                m_Symbol = T_LEFTPAREN;
                return;
            }
            break;
        case ')' :
            m_Col++; m_ch = GetChar();
            m_Symbol = T_RIGHTPAREN;
            return;
        case '[' :
            m_Col++; m_ch = GetChar();
            m_Symbol = T_LEFTBRACKET;
            return;
        case ']' :
            m_Col++; m_ch = GetChar();
            m_Symbol = T_RIGHTBRACKET;
            return;
        case '{' :
            m_Col++; m_ch = GetChar();
            m_Symbol = T_LEFTCURLY;
            return;
        case '}' :
            m_Col++; m_ch = GetChar();
            m_Symbol = T_RIGHTCURLY;
            return;
    }
}