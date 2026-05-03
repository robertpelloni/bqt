#ifndef JUCEWIDGET_H
#define JUCEWIDGET_H

#include <QWidget>
#include <memory>

// Forward declaration of a JUCE component
namespace juce {
    class Component;
}

class JuceWidget : public QWidget {
    Q_OBJECT
public:
    explicit JuceWidget(QWidget *parent = nullptr);
    ~JuceWidget();

    // Sets the underlying JUCE component to be hosted
    void setJuceComponent(std::unique_ptr<juce::Component> component);
    juce::Component* juceComponent() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    std::unique_ptr<juce::Component> m_juceComponent;
};

#endif // JUCEWIDGET_H