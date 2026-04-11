package kernel

import (
	"sync"
)

// Property provides native Go reactive bindings using Generics (JavaFX Parity).
type Property[T any] struct {
	mu        sync.RWMutex
	value     T
	listeners []func(T)
	binding   *Property[T]
}

func NewProperty[T any](initial T) *Property[T] {
	return &Property[T]{value: initial}
}

func (p *Property[T]) Get() T {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.value
}

func (p *Property[T]) Set(val T) {
	p.mu.Lock()
	p.value = val
	listeners := p.listeners
	p.mu.Unlock()

	for _, listener := range listeners {
		listener(val)
	}
}

// Bind links this property to another. When the other changes, this one follows.
func (p *Property[T]) Bind(other *Property[T]) {
	p.mu.Lock()
	p.binding = other
	p.mu.Unlock()

	other.AddListener(func(val T) {
		p.Set(val)
	})
	p.Set(other.Get())
}

func (p *Property[T]) AddListener(f func(T)) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.listeners = append(p.listeners, f)
}
