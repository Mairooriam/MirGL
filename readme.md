# Mirkiel: Learning and Crafting Interpreters

Welcome to my repository dedicated to the journey of understanding and building programming language interpreters. In short [Crafting interpeters](https://craftinginterpreters.com/).

### learning points
I had seen this type of visitor pattern before. but this time i "got it".
```cpp
    using Literal = std::variant<std::nullptr_t, bool, double, std::string>;

    struct VariantPrinter {
        std::ostream& os;
        
        void operator()(std::nullptr_t) const { os << "nil"; }
        void operator()(bool value) const { os << (value ? "true" : "false"); }
        void operator()(double value) const { os << value; }
        void operator()(const std::string& value) const { os << "\"" << value << "\""; }
    };

    std::ostream& operator<<(std::ostream& os, const Literal& value) {
        std::visit(VariantPrinter{os}, value);
        return os;
    }

    Literal a;

    std::cout << a << "\n"
```

Elegant way to make switch cases cleaner.  dont know why I hadnt though about this one before. Just put the stuff in function!
```cpp
    void Scanner::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n')
                m_line++;
            advance();
        }
        if (isAtEnd()) {
            m_mirkiel->error(m_line, "Unterminated string.");
            return;
        }
        advance();

        std::string value = m_src.substr(m_start + 1, m_current - 1);
        addToken(TokenType::STRING, value);
    }

    void Scanner::scanToken() {
        char c = advance();
        switch (c) {
            case '/': comment(); break;
            case ' ':
            case '\r':
            case '\t': break;
            case '\n': m_line++; break;
            case '"': string(); break;  
        }
```