#ifndef OMNIVM_H
#define OMNIVM_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <memory>

class OmniNeuralEngine;
class OmniThemeManager;

struct OmniASTNode {
    QString type; // "List", "Symbol", "Number", "String"
    QString value;
    QList<std::shared_ptr<OmniASTNode>> children;
};

// The OmniScript Hypervisor: A native LISP-like interpreter built entirely
// in C++ to provide a Turing-complete, WASM-safe execution sandbox.
class OmniVM : public QObject {
    Q_OBJECT

public:
    static OmniVM* instance();

    // Evaluates a raw OmniScript string and returns the output
    Q_INVOKABLE QString evalScript(const QString& script);

    // Resets the virtual machine's memory context
    Q_INVOKABLE void resetEnvironment();

signals:
    void scriptOutput(const QString& output);
    void scriptError(const QString& errorMsg);

private:
    explicit OmniVM(QObject *parent = nullptr);
    ~OmniVM() override;

    QStringList tokenize(const QString& script);
    std::shared_ptr<OmniASTNode> parse(QStringList& tokens);
    QString evaluate(std::shared_ptr<OmniASTNode> node);

    // The VM's local RAM (Symbol Table)
    QMap<QString, QString> m_environment;

    // Built-in OS Hooks
    QString builtin_add(const QList<std::shared_ptr<OmniASTNode>>& args);
    QString builtin_define(const QList<std::shared_ptr<OmniASTNode>>& args);
    QString builtin_click(const QList<std::shared_ptr<OmniASTNode>>& args);
    QString builtin_theme(const QList<std::shared_ptr<OmniASTNode>>& args);
};

#endif // OMNIVM_H
