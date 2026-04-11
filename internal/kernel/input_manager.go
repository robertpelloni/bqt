package kernel

import (
	"sync"
)

type DeviceType string

const (
	Mouse    DeviceType = "mouse"
	Keyboard DeviceType = "keyboard"
	Touch    DeviceType = "touch"
)

type DeviceState struct {
	ID   string     `json:"id"`
	Name string     `json:"name"`
	Type DeviceType `json:"type"`
	X, Y float64    `json:"x"`
}

type InputManager struct {
	mu      sync.RWMutex
	devices map[string]*DeviceState
}

var (
	inputInstance *InputManager
	inputOnce     sync.Once
)

func GetInputManager() *InputManager {
	inputOnce.Do(func() {
		inputInstance = &InputManager{
			devices: make(map[string]*DeviceState),
		}
	})
	return inputInstance
}

func (im *InputManager) RegisterDevice(id, name string, devType DeviceType) {
	im.mu.Lock()
	defer im.mu.Unlock()
	if _, exists := im.devices[id]; !exists {
		im.devices[id] = &DeviceState{ID: id, Name: name, Type: devType}
	}
}

func (im *InputManager) UpdateCursor(id string, x, y float64) {
	// --- PERMISSION HARDENING ---
	// Before updating, the kernel could check if the device owns the current workspace.
	im.mu.Lock()
	defer im.mu.Unlock()
	if dev, exists := im.devices[id]; exists {
		dev.X, dev.Y = x, y
	}
}

func (im *InputManager) GetDevices() []DeviceState {
	im.mu.RLock()
	defer im.mu.RUnlock()
	snapshot := make([]DeviceState, 0, len(im.devices))
	for _, dev := range im.devices { snapshot = append(snapshot, *dev) }
	return snapshot
}
