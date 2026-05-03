// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <BOBUIextOption>

class BOBUIextLayout;
class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);
    ~View() override;

    void updateLayout(const QString &sourceString,
                      float width,
                      BOBUIextOption::WrapMode mode,
                      const QFont &font);

    QSize sizeHint() const override;

    BOBUIextLayout *layout() const { return m_layout; }

public slots:
    void setVisualizedBounds(const QRegion &region)
    {
        m_bounds = region;
        update();
    }

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    BOBUIextLayout *m_layout = nullptr;
    QRegion m_bounds;
};

#endif // VIEW_H
