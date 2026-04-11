#include "OmniVM.h"
#include "OmniNeuralEngine.h"
#include "OmniThemeManager.h"
#include <QDebug>
#include <QRegularExpression>

OmniVM* OmniVM::instance() {
    static OmniVM* vm = new OmniVM();
    return vm;
}

OmniVM::OmniVM(QObject *parent) : QObject(parent) {
    setObjectName("OmniVM");
    resetEnvironment();
}

OmniVM::~OmniVM() = default;

void OmniVM::resetEnvironment() {
    m_environment.clear();
    m_environment["os_name"] = "OmniOS v5.2.0";
}

QStringList OmniVM::tokenize(const QString& script) {
    QString s = script;
    s.replace("(", " ( ");
    s.replace(")", " ) ");
    
    // Basic whitespace split honoring string quotes is complex, 
    // for this lightweight VM we use a naive split, but we ensure structure.
    QStringList tokens;
    QString currentToken = "";
    bool inString = false;
    
    for (int i = 0; i < s.length(); ++i) {
        QChar c = s[i];
        if (c == '"') {
            inString = !inString;
            currentToken += c;
        } else if (c.isSpace() && !inString) {
            if (!currentToken.isEmpty()) {
                tokens.append(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    if (!currentToken.isEmpty()) tokens.append(currentToken);
    return tokens;
}

std::shared_ptr<OmniASTNode> OmniVM::parse(QStringList& tokens) {
    if (tokens.isEmpty()) return nullptr;
    
    QString token = tokens.takeFirst();
    auto node = std::make_shared<OmniASTNode>();
    
    if (token == "(") {
        node->type = "List";
        while (!tokens.isEmpty() && tokens.first() != ")") {
            node->children.append(parse(tokens));
        }
        if (!tokens.isEmpty()) tokens.takeFirst(); // Pop ")"
    } else if (token == ")") {
        emit scriptError("Unexpected ')'");
        return nullptr;
    } else {
        bool isNumber;
        token.toDouble(&isNumber);
        
        if (isNumber) {
            node->type = "Number";
            node->value = token;
        } else if (token.startsWith('"') && token.endsWith('"')) {
            node->type = "String";
            node->value = token.mid(1, token.length() - 2); // Strip quotes
        } else {
            node->type = "Symbol";
            node->value = token;
        }
    }
    return node;
}

QString OmniVM::evaluate(std::shared_ptr<OmniASTNode> node) {
    if (!node) return "";
    
    if (node->type == "Number" || node->type == "String") {
        return node->value;
    } else if (node->type == "Symbol") {
        if (m_environment.contains(node->value)) {
            return m_environment[node->value];
        }
        return node->value;
    } else if (node->type == "List") {
        if (node->children.isEmpty()) return "";
        
        QString op = node->children[0]->value;
        QList<std::shared_ptr<OmniASTNode>> args = node->children.mid(1);
        
        // Native OS Hooks
        if (op == "+") return builtin_add(args);
        if (op == "define") return builtin_define(args);
        if (op == "click") return builtin_click(args);
        if (op == "theme") return builtin_theme(args);
        if (op == "print") {
            QString out;
            for (auto& arg : args) { out += evaluate(arg) + " "; }
            emit scriptOutput(out.trimmed());
            return out.trimmed();
        }
        
        emit scriptError("Unknown function: " + op);
        return "Error";
    }
    
    return "";
}

QString OmniVM::evalScript(const QString& script) {
    qDebug() << "OmniVM: Parsing Script ->" << script;
    QStringList tokens = tokenize(script);
    
    QString lastResult = "";
    while (!tokens.isEmpty()) {
        auto ast = parse(tokens);
        if (ast) lastResult = evaluate(ast);
    }
    
    qDebug() << "OmniVM: Script Execution Completed. Result:" << lastResult;
    return lastResult;
}

// --- BUILT-IN FUNCTIONS ---

QString OmniVM::builtin_add(const QList<std::shared_ptr<OmniASTNode>>& args) {
    double sum = 0;
    for (auto& arg : args) {
        sum += evaluate(arg).toDouble();
    }
    return QString::number(sum);
}

QString OmniVM::builtin_define(const QList<std::shared_ptr<OmniASTNode>>& args) {
    if (args.size() < 2) return "Error: define requires symbol and value";
    QString sym = args[0]->value;
    QString val = evaluate(args[1]);
    m_environment[sym] = val;
    return val;
}

QString OmniVM::builtin_click(const QList<std::shared_ptr<OmniASTNode>>& args) {
    if (args.isEmpty()) return "Error: click requires targetId";
    QString target = evaluate(args[0]);
    
    // Hard-wired into the OS AI Command Engine
    bool success = OmniNeuralEngine::instance()->executeAction(target, "click");
    
    QString out = success ? "Success" : "Failed";
    emit scriptOutput(QString("OmniVM Hook [click]: %1 -> %2").arg(target, out));
    return out;
}

QString OmniVM::builtin_theme(const QList<std::shared_ptr<OmniASTNode>>& args) {
    if (args.isEmpty()) return "Error: theme requires mode string";
    QString theme = evaluate(args[0]).toLower();
    
    if (theme == "cyberpunk") OmniThemeManager::instance()->setCyberpunkMode();
    else if (theme == "glass") OmniThemeManager::instance()->setLiquidGlassMode();
    else if (theme == "chronos") OmniThemeManager::instance()->setChronosShiftMode();
    else OmniThemeManager::instance()->setDarkMode();
    
    return "Theme updated to " + theme;
}
