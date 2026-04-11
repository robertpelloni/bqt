#ifndef OMNIPROPERTY_H
#define OMNIPROPERTY_H

#include <QObject>
#include <QVariant>
#include <functional>
#include <QList>

/**
 * @brief The OmniProperty class (JavaFX Parity)
 * 
 * Provides native C++ reactive bindings. Allows a property to be "bound" to another.
 */
template <typename T>
class OmniProperty {
public:
    OmniProperty(T initialValue = T()) : m_value(initialValue), m_binding(nullptr) {}

    T get() const { return m_value; }

    void set(T newValue) {
        if (m_value != newValue) {
            m_value = newValue;
            notifyListeners();
            
            // Update anyone bound to us
            for (auto* other : m_boundProperties) {
                other->set(m_value);
            }
        }
    }

    // Reactive Binding: This property will now always track the 'other' property
    void bind(OmniProperty<T>& other) {
        m_binding = &other;
        other.m_boundProperties.append(this);
        set(other.get());
    }

    void unbind() {
        if (m_binding) {
            m_binding->m_boundProperties.removeAll(this);
            m_binding = nullptr;
        }
    }

    void addListener(std::function<void(T)> listener) {
        m_listeners.append(listener);
    }

private:
    void notifyListeners() {
        for (auto& listener : m_listeners) {
            listener(m_value);
        }
    }

    T m_value;
    OmniProperty<T>* m_binding;
    QList<OmniProperty<T>*> m_boundProperties;
    QList<std::function<void(T)>> m_listeners;
};

#endif // OMNIPROPERTY_H
