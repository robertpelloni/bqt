package kernel

import (
	"fmt"
	"sync"
)

// PluginInterface defines the contract for Go-native OS extensions.
type PluginInterface interface {
	Name() string
	Version() string
	Initialize() error
}

type PluginManager struct {
	mu      sync.RWMutex
	plugins map[string]PluginInterface
}

var (
	pluginInstance *PluginManager
	pluginOnce     sync.Once
)

func GetPluginManager() *PluginManager {
	pluginOnce.Do(func() {
		pluginInstance = &PluginManager{
			plugins: make(map[string]PluginInterface),
		}
	})
	return pluginInstance
}

// RegisterPlugin injects a new Go module into the OS kernel dynamically.
func (pm *PluginManager) RegisterPlugin(p PluginInterface) error {
	pm.mu.Lock()
	defer pm.mu.Unlock()

	if _, exists := pm.plugins[p.Name()]; exists {
		return fmt.Errorf("plugin %s already registered", p.Name())
	}

	if err := p.Initialize(); err != nil {
		return err
	}

	pm.plugins[p.Name()] = p
	return nil
}

func (pm *PluginManager) GetLoadedPlugins() []string {
	pm.mu.RLock()
	defer pm.mu.RUnlock()

	names := make([]string, 0, len(pm.plugins))
	for name := range pm.plugins {
		names = append(names, name)
	}
	return names
}
