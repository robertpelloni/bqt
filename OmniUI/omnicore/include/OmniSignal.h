#ifndef OMNISIGNAL_H
#define OMNISIGNAL_H

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

// Header-only, MOC-free typed signal primitive inspired by the advantages
// CopperSpice demonstrates over generator-driven metaobject systems.
//
// Goals:
// - no QObject requirement
// - no Q_OBJECT / MOC dependency
// - strongly typed callback signatures
// - lightweight enough for framework-owned services

template <typename... Args>
class OmniSignal {
public:
    using Slot = std::function<void(Args...)>;

    class Connection {
    public:
        Connection() = default;
        Connection(size_t id, OmniSignal *owner) : m_id(id), m_owner(owner) {}

        void disconnect() {
            if (m_owner) {
                m_owner->disconnect(m_id);
                m_owner = nullptr;
            }
        }

        bool isConnected() const { return m_owner != nullptr; }

    private:
        size_t m_id = 0;
        OmniSignal *m_owner = nullptr;
    };

    Connection connect(Slot slot) {
        const size_t id = ++m_nextId;
        m_slots.push_back({id, std::move(slot)});
        return Connection{id, this};
    }

    void emit(Args... args) {
        for (const auto &entry : m_slots) {
            if (entry.slot) {
                entry.slot(args...);
            }
        }
    }

    void operator()(Args... args) {
        emit(args...);
    }

    bool empty() const { return m_slots.empty(); }
    size_t size() const { return m_slots.size(); }

private:
    struct Entry {
        size_t id;
        Slot slot;
    };

    void disconnect(size_t id) {
        m_slots.erase(
            std::remove_if(m_slots.begin(), m_slots.end(),
                [id](const Entry &entry) { return entry.id == id; }),
            m_slots.end());
    }

    size_t m_nextId = 0;
    std::vector<Entry> m_slots;
};

#endif // OMNISIGNAL_H
